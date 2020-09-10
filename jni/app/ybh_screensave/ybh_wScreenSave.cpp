#include "sys.h"
#include "wMain.h"
#include "wIntercomCallLabel.h"
#include "ybh_wScreenSave.h"

mWindow *SysMain = NULL;
//mWindow srcc;

/* ybh 2020 8/7 */
//ybh_wScreenSave src;  //这里如果定义了，构造函数会在执行一次，错误语句

static void *screen(void *view)
{
    ybh_wScreenSave * wScreenSave = (ybh_wScreenSave *)view;
    wScreenSave->ybh_screen();
    return NULL;
}

ybh_wScreenSave::ybh_wScreenSave() : mWindow("ybh_screensave")
{
    SysMain = this;

    m_exit.setParent(this);
    this->scr_process();

    m_text.setParent(this);
    m_text.load(m_style, "bkg_text");

    m_date.setParent(this);
    m_date.load(m_style, "date");

    m_ts_st = -1;
    m_ts_st_h = -1;

    time_t now = time(NULL);
	struct tm *tm = localtime(&now);
	if (m_ts_st != tm->tm_min || m_ts_st_h != tm->tm_hour) {
		char s[128];
		strftime(s, sizeof(s), "%d %b %Y %H : %M", tm);
		m_date.setText(s);
		m_ts_st = tm->tm_min;
        m_ts_st_h = tm->tm_hour;
	}
}

ybh_wScreenSave::~ybh_wScreenSave()
{
    SysMain = NULL;
}

void ybh_wScreenSave::doEvent(mEvent *e)
{
    void *ret = NULL;
    mWindow::doEvent(e);
    if(e->type == mEventHook) {
        this->not_scr_process();
        wIntercomCallLabel *w = new wIntercomCallLabel();   //被呼叫时显示呼叫界面
        w->show();
        return;
    } else if(e->type == mEvent::TouchEnd) {
        if (e->wParam == m_exit.id()) {
            this->not_scr_process();
            wMain *w = new wMain();
            w->show();   
        }
    }
}

void ybh_wScreenSave::scr_process(void)
{
    if (pthread_create(&pid, NULL, screen, this) != 0) {
        perror("pthread_create srceen!\n");
    }
}

#if 1
void ybh_wScreenSave::not_scr_process(void)
{
    void *ret = NULL;
    exit = true;
    pthread_join(pid, &ret);
    printf("thread 3 exit code %d\n", (int)ret);
}
#endif


void ybh_wScreenSave::ybh_screen(void)
{
    int i = 0;
    time_t now = 0;
    while (!exit) {
        // for (int i = 0; i < 4; i++) {
        //     if (exit) {
        //         break;
        //     }
        //     char s[128];
        //     sprintf(s, "bkg%d", i);
        //     m_exit.load(m_style, s);
        //     //fprintf(stderr, "%s\n", s);
        //     for (int j = 0; j < 500; j++) { //壁纸显示时间5S
        //         if (exit) {
        //             break;
        //         } else {
        //             usleep(10 * 1000);  //延时10ms
        //         }
        //     }
        // }
        if (time(NULL) - now >= 5) {
            char s[128];
            sprintf(s, "bkg%d", i);
            // y_m_bkg.setParent(this);
            m_exit.load(m_style, s);
            i++;
            if (i >= 4) {
                i = 0;
            }
            now = time(NULL);
        }
        usleep(10 * 1000);
    }
}

