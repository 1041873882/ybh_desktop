#include "sys.h"
#include "wMain.h"
#include "ybh_wKeys.h"
#include <net/if.h>

ybh_wKeys::ybh_wKeys() : mWindow("ybh_keys")
{
    m_bkg.setParent(this);
    m_bkg.load(m_style, "bkg");

    m_exit.setParent(this);
    m_exit.load(m_style, "exit");

    m_title.setParent(this);
    m_title.load(m_style, "title");
    

}

ybh_wKeys::~ybh_wKeys()
{
}

void ybh_wKeys::doEvent(mEvent *e)
{
    mWindow::doEvent(e);

    if(e->type == mEvent::TouchEnd) {
        if (e->wParam == m_exit.id()) {
            wMain *w = new wMain();
            w->show();
        }
    }
}