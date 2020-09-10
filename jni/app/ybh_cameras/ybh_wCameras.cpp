#include "SysCaller.h"
#include "SysLogger.h"
#include "mFB.h"
#include "SysCaller.h"
#include "sys.h"
#include "wMain.h"
#include "ybh_wCameras.h"

ybh_wCameras::ybh_wCameras() : mWindow("ybh_cameras")
{
    m_bkg.setParent(this);
    m_bkg.load(m_style, "bkg");

    m_exit.setParent(this);
    m_exit.load(m_style, "exit");

    m_prev.setParent(this);
    m_prev.load(m_style, "prev"); 

    m_list.setParent(this);
    m_list.load(m_style, "list"); 

    m_stop.setParent(this);
    m_stop.load(m_style, "stop");    

    m_start.setParent(this);
    m_start.load(m_style, "start"); 

    m_next.setParent(this);
    m_next.load(m_style, "next"); 

    m_prompt.setParent(this);
    m_prompt.load(m_style, "prompt");
}

ybh_wCameras::~ybh_wCameras()
{
}

void ybh_wCameras::doEvent(mEvent *e)
{
    mWindow::doEvent(e);
    if(e->type == mEvent::TouchEnd) {
        if (e->wParam == m_exit.id()) {
            wMain *w = new wMain();
            w->show();
        }
    }
}