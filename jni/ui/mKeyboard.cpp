
#include "mFB.h"
#include "mKeyboard.h"

mKeyboard *keyboard = NULL;

static const char key_map[MAX_KEY_SZ] = {
	'1', '2', '3',
	'4', '5', '6',
	'7', '8', '9',
	'.', '0', 0x7F,
};

mKeyboard::mKeyboard() : mWindow("keyboard")
{
	m_bkg.setParent(this);
	m_bkg.load(m_style, "bkg");

	this->setLevel(1000);
	for(int i=0; i<MAX_KEY_SZ; i++) {
		char s[128];
		sprintf(s, "key%d", i);
		m_key[i].setParent(this);
		m_key[i].load(m_style, s);
		m_key[i].setLevel(1001);
	}

	keyboard = this;
	if (window)
		mObject::setParent(window);
}

mKeyboard::~mKeyboard()
{
	keyboard = NULL;
	if (m_parent)
		m_parent->remove(this);
}

extern sFifo mTouchFifo;
void mKeyboard::doTouch(int x, int y, int m)
{
	int xx = m_bkg.x();
	int yy = m_bkg.y();
	int ww = m_bkg.width();
	int hh = m_bkg.height();
	if (x >= xx && y >= yy && x <= (xx+ww) && y <= (yy+hh)) {
		mTouchFifo.flush(); //���̷�Χ�ڣ�ǿ����������ؼ��¼�
		mObject::doTouch(x, y, m);
	} else {
		if (m > 0) { //����֮��ĵط��д���
			mEvent::doFifo(this);
			mEvent e(mEvent::KeyboardEnd);
			e.wParam = (uint32_t)this;
			mEvent::post(&e);
		}
	}
}

void mKeyboard::setParent(mObject *obj)
{
	m_edit = obj;
}

void mKeyboard::resize(void)
{
	int offset = 0;
	if (m_edit && m_edit->y() < fb.height()/2)
		offset = fb.height()-m_bkg.height();
	m_bkg.move(m_bkg.x(), m_bkg.y()+offset);
	for(int i=0; i<MAX_KEY_SZ; i++) {
		m_key[i].move(m_key[i].x(), m_key[i].y()+offset);
	}
}

void mKeyboard::doEvent(mEvent *e)
{
	if (e->type == mEvent::TouchEnd) {
		for(int i=0; i<MAX_KEY_SZ; i++) {
			if (e->wParam == m_key[i].id() && key_map[i] != ' ') {
				m_edit->key(key_map[i]);
				break;
			}
		}
	}
	mWindow::doEvent(e);
}
