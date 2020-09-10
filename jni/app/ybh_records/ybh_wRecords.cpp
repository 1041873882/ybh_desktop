#include "sys.h"
#include "wMain.h"
#include "ybh_wRecords.h"

ybh_wRecords::ybh_wRecords() : mWindow("ybh_records")
{
    m_bkg.setParent(this);
    m_bkg.load(m_style, "bkg");

    m_exit.setParent(this);
    m_exit.load(m_style, "exit");

    for (int i = 0; i < 4; i++) {
        char s[32];
        sprintf(s, "title%d", i);
        m_title[i].setParent(this);
        m_title[i].load(m_style, s);
    }

    m_call.setParent(this);
    m_call.load(m_style, "call");

    m_up.setParent(this);
    m_up.load(m_style, "up");

    m_down.setParent(this);
    m_down.load(m_style, "down");

    m_del.setParent(this);
    m_del.load(m_style, "del");

}

ybh_wRecords::~ybh_wRecords()
{
}

void ybh_wRecords::doEvent(mEvent *e)
{
    mWindow::doEvent(e);

    if(e->type == mEvent::TouchEnd) {
        if (e->wParam == m_exit.id()) {
            wMain *w = new wMain();
            w->show();
        }
    }
}