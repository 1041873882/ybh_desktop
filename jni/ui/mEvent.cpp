
#include "sys.h"
#include "sFifo.h"
#include "SysSound.h"
#include "mFB.h"
#include "mWindow.h"
#include "mEvent.h"

#include "ybh_wSetupGeneralLabel.h"

#define ABS_MT_SLOT             0x2f    /* MT slot being modified */
#define ABS_MT_TOUCH_MAJOR      0x30    /* Major axis of touching ellipse */
#define ABS_MT_TOUCH_MINOR      0x31    /* Minor axis (omit if circular) */
#define ABS_MT_WIDTH_MAJOR      0x32    /* Major axis of approaching ellipse */
#define ABS_MT_WIDTH_MINOR      0x33    /* Minor axis (omit if circular) */
#define ABS_MT_ORIENTATION      0x34    /* Ellipse orientation */
#define ABS_MT_POSITION_X       0x35    /* Center X ellipse position */
#define ABS_MT_POSITION_Y       0x36    /* Center Y ellipse position */
#define ABS_MT_TOOL_TYPE        0x37    /* Type of touching device */
#define ABS_MT_BLOB_ID          0x38    /* Group a set of packets as a blob */
#define ABS_MT_TRACKING_ID      0x39    /* Unique ID of initiated contact */
#define ABS_MT_PRESSURE         0x3a    /* Pressure on contact area */

static sFifo eFifo;
static int ePipe[2] = {-1, -1};
static int eKey = -1, eTp = -1;

static struct timeval sys_mute_tv;

int TpMode = 0; //0:电容屏 1:电阻屏
int TpPointercal[7];

static float mTpScaleX = 1.0;
static float mTpScaleY = 1.0;

static void sys_tp_detect(void)
{
	FILE *fp = fopen("/sys/class/input/event2/device/name", "r");
	if (fp) {
		char s[1024];
		fscanf(fp, "%s", s);
		fclose(fp);
		if (!strcmp(s, "NS2009"))
			TpMode = 1;
	}
	for(int i=0; i<7; i++)
		TpPointercal[i] = 0;

	if (TpMode == 1) {
		fp = fopen("/dnake/cfg/pointercal", "r");
		if (fp) {
			for(int i=0; i<7; i++)
				fscanf(fp, "%d", &TpPointercal[i]);
			fclose(fp);
		}
		if (TpPointercal[6] == 0) {
			char url[128];
			if (fb.m_width == 800)
				strcpy(url, "/dnake/etc/pointercal_800x480");
			else
				strcpy(url, "/dnake/etc/pointercal");
			fp = fopen(url, "r");
			if (fp) {
				for(int i=0; i<7; i++)
					fscanf(fp, "%d", &TpPointercal[i]);
				fclose(fp);
			}
		}
	}
}

mEvent::mEvent(Type t) : type(t)
{
	wParam = 0;
	lParam = 0;
}

mEvent::~mEvent()
{
}

#include "SysSecurity.h"

void mEvent::doKey(struct input_event *ev)
{
	//ev.value 0:放开 1:按下
	if (ev->value) {
		//sound.y_press.press();
		sound.press();  
	}

	if (sys.m_limit == 220 && __ts(sys_mute_tv) < 2*1000)
		return;

	mEvent e(ev->value ? KeyPress : KeyRelease);
	switch(ev->code) {
	case KEY_M_UNLOCK: //开锁
		e.wParam = KEY_M_UNLOCK;
		break;

	case KEY_M_ANSWER: //摘机
		e.wParam = KEY_M_ANSWER;
		break;

	case KEY_M_MENU: //预留
		e.wParam = KEY_M_MENU;
		break;

	case KEY_M_CALL: //呼叫
		e.wParam = KEY_M_CALL;
		break;

	case KEY_M_MONITOR: //监视
		e.wParam = KEY_M_MONITOR;
		break;

	default:
		return;
	}
	mEvent::post(&e);
}

typedef struct {
	int x;
	int y;
	int m;
} tp_data_t;

static tp_data_t mTp = {0, 0, 0};
static tp_data_t mLast = {0, 0, 0};
sFifo eEventData;

int mEvent::y_m_press = 1;

void mEvent::doTouchEvent(struct input_event *ev)
{
	static int have = 0;
	if (ev->type == EV_ABS) {
		switch (ev->code) {
		case ABS_MT_POSITION_X:
			if (mTpScaleX != 1.0)
				mTp.x = mTpScaleX*ev->value;
			else
				mTp.x = ev->value;
			break;

		case ABS_MT_POSITION_Y:
			if (mTpScaleY != 1.0)
				mTp.y = mTpScaleY*ev->value;
			else
				mTp.y = ev->value;
			break;

		case ABS_MT_TOUCH_MAJOR:
			mTp.m = ev->value;
			have = 1;
			break;
		}
	} else if (ev->type == EV_SYN) {
		if (have) {
			mEvent::doTouch();
		}
		have = 0;
	}
}

extern int debug_x, debug_y;

void mEvent::doTouch(void)
{
	//ybh_wSetupGeneralLabel general;
	tp_data_t tp = mTp;

	if (TpMode) {
		int x = tp.x;
		int y = tp.y;
		tp.x = (TpPointercal[2]+TpPointercal[0]*x + TpPointercal[1]*y)/TpPointercal[6];
		tp.y = (TpPointercal[5]+TpPointercal[3]*x + TpPointercal[4]*y)/TpPointercal[6];
	} else {
		if (fb.height() == 480 || mTpScaleX != 1.0) {
			//触摸屏线太稀,边沿做特殊处理
			int yy = 12*mTpScaleY;
			if (tp.y+yy > fb.height()) {
				tp.y -= yy;
			} else if (tp.y < yy) {
				tp.y += yy;
			}
		}
	}

	if (sys_factory) {
		debug_x = tp.x;
		debug_y = tp.y;
		mSysPaint = 1;
	}

	mEvent e(TouchRaw);
	e.wParam = tp.m;
	e.lParam = mTp.x | (mTp.y<<16);
	e.lParam2 = tp.x | (tp.y<<16);
	mEvent::post(&e);

	if (tp.m == mLast.m)
		return;

	if (window) {
		window->doTouch(tp.x, tp.y, tp.m);
	}
	mObject *p = NULL;
	while (mTouchFifo.used()) {
		mObject *p2;
		mTouchFifo.get(&p2, sizeof(p2));
		if (p == NULL || p2->level() >= p->level())
			p = p2;
	}
	if (p) {
		if (tp.m) {
			if (mObject::focus() != p) {
				mEvent e(TouchBegin);
				e.wParam = (uint32_t)p;
				mEvent::post(&e);
				if (y_m_press) {
					sound.press();        //ybh 2020.8.17					
				}
				// sound.press();        //ybh 2020.8.17					
				p->setFocus(1);
			}
		} else {
			if (mObject::focus() == p) {
				mEvent e(TouchEnd);
				e.wParam = (uint32_t)p;
				mEvent::post(&e);
				p->setFocus(0);
			} else {
				if (window)
					window->setFocus(1);
			}
		}
	} else {
		if (window)
			window->setFocus(1);
	}
	//printf("xy: %d %d %d\n", tp.x, tp.y, tp.m);
	mLast = mTp;
}

void mEvent::doInputRead(int fd)
{
	while (1) {
		struct input_event ev;
		int r = ::read(fd, &ev, sizeof(ev));
		if (r > 0) {
			int ok = 0;
			if (fb.enabled() && __ts(sys_mute_tv) > 500)
				ok = 1;
			else if (!fb.enabled() && __ts(sys_mute_tv) > 1500)
				ok = 1;
			if (ok) {
				eEventData.put((uint8_t *)&ev, sizeof(ev));
				fb.enable(1);
			}
		} else
			break;
	}
}

void mEvent::doFifo(mObject *obj)
{
	mTouchFifo.put(&obj, sizeof(obj));
}

void ui_main_process(void);

#define test_bit(bit, array)    (array[bit/8] & (1<<(bit%8)))

static void *sys_input_thread(void *)
{
	struct timeval tv;
	fd_set rfds;
	while(1) {
		int max = -1;
		FD_ZERO(&rfds);

		if (eKey < 0) {
			eKey = ::open("/dev/input/event0", O_RDONLY);
			if (eKey > 0)
				fcntl(eKey, F_SETFL, O_NONBLOCK);
		}
		if (eTp < 0) {
			eTp = ::open("/dev/input/event2", O_RDONLY);
			if (eTp > 0) {
				fcntl(eTp, F_SETFL, O_NONBLOCK);
				sys_tp_detect();

				if (TpMode == 0) {
					uint8_t ab[(ABS_MAX + 1) / 8];
					ioctl(eTp, EVIOCGBIT(EV_ABS, sizeof(ab)), ab);
					if (test_bit(ABS_MT_POSITION_X, ab) && test_bit(ABS_MT_POSITION_Y, ab)) {
						struct input_absinfo d;
						float x = 1, y = 1;
						if(!ioctl(eTp, EVIOCGABS(ABS_MT_POSITION_X), &d)) {
							x = d.maximum-d.minimum;
						}
            					if(!ioctl(eTp, EVIOCGABS(ABS_MT_POSITION_Y), &d)) {
            						y = d.maximum-d.minimum;
						}
						if (x != fb.m_width)
							mTpScaleX = (float)fb.m_width/x;
						if (y != fb.m_height)
							mTpScaleY = (float)fb.m_height/y;
            				}
				}
			}
		}

		if (eKey > 0) {
			FD_SET(eKey, &rfds);
			if (eKey > max) max = eKey;
		}
		if (eTp > 0) {
			FD_SET(eTp, &rfds);
			if (eTp > max) max = eTp;
		}

		tv.tv_sec = 0;
		tv.tv_usec = 300*1000;
		int r = ::select(max+1, &rfds, NULL, NULL, &tv);
		if (r == 0) {
			if (mLast.m) { //未收到放开事件
				struct input_event e;
				e.type = EV_ABS;
				e.code = ABS_MT_TOUCH_MAJOR;
				e.value = 0;
				eEventData.put((uint8_t *)&e, sizeof(e));

				e.type = EV_SYN;
				eEventData.put((uint8_t *)&e, sizeof(e));
			}
		} else {
			if (FD_ISSET(eKey, &rfds))
				mEvent::doInputRead(eKey);
			if (FD_ISSET(eTp, &rfds))
				mEvent::doInputRead(eTp);
		}
	}
	return NULL;
}

void mEvent::process(void)
{
{
	pthread_t pid;
	if (pthread_create(&pid, NULL, sys_input_thread, NULL) != 0)
		perror("pthread_create sys_input_thread!\n");
}

	if (::pipe(ePipe) < 0)
		exit(-1);
	::write(ePipe[1], "W", 1);

	while(1) {
		struct timeval tv;
		fd_set rfds;

		FD_ZERO(&rfds);
		FD_SET(ePipe[0], &rfds);

		tv.tv_sec = 0;
		tv.tv_usec = 40*1000;
		::select(ePipe[0]+1, &rfds, NULL, NULL, &tv);

		if (FD_ISSET(ePipe[0], &rfds)) {
			char d[16];
			::read(ePipe[0], d, sizeof(d));
		}

		while (1) {
			struct input_event ev;
			int r = eEventData.get((uint8_t *)&ev, sizeof(ev));
			if (r <= 0)
				break;
			if (ev.type == EV_KEY)
				mEvent::doKey(&ev);
			else if (ev.type == EV_ABS || ev.type == EV_SYN)
				mEvent::doTouchEvent(&ev);
		}

		while(eFifo.used() >= sizeof(mEvent)) {
			mEvent e(None);
			eFifo.get((uint8_t *)&e, sizeof(e));
			if (window != NULL)
				window->doEvent(&e);
		}

		mWindow::process();
		if (window != NULL)
			window->doPaint();
		if (window != NULL)
			window->doTimer();

		ui_main_process();
	}
}

void mEvent::post(mEvent *e)
{
	if (ePipe[1] > 0) {
		eFifo.put((uint8_t *)e, sizeof(mEvent));
		::write(ePipe[1], "W", 1);
	}
}

void mEvent::mute(void)
{
	gettimeofday(&sys_mute_tv, NULL);
	eEventData.flush();
}
