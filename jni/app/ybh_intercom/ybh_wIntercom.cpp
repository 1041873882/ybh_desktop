#include "sys.h"
#include "SysCaller.h"
#include "mFB.h"
#include "wMain.h"
#include "ybh_wIntercom.h"

ybh_wIntercom::ybh_wIntercom() : mWindow("ybh_intercom")
{
    m_mute_st = -1;

    m_bkg.setParent(this);
    m_bkg.load(m_style, "bkg");

    m_osd.setParent(this);
    m_osd.load(m_style, "osd");

    m_bkg_list.setParent(this);
    m_bkg_list.load(m_style, "bkg_list");
    m_bkg_list.hide();

    m_exit.setParent(this);
    m_exit.load(m_style, "exit");

    m_prev.setParent(this);
    m_prev.load(m_style, "prev"); 

    m_list.setParent(this);
    m_list.load(m_style, "list"); 

    m_next.setParent(this);
    m_next.load(m_style, "next"); 

    m_unlock1.setParent(this);
    m_unlock1.load(m_style, "unlock1");  

    m_unlock2.setParent(this);
    m_unlock2.load(m_style, "unlock2");  

    m_stop.setParent(this);
    m_stop.load(m_style, "stop");    

    m_start.setParent(this);
    m_start.load(m_style, "start"); 

    m_mute.setParent(this);
    
    m_err.setParent(this);
    m_err.load(m_style, "err");
    m_err.hide();

    int x = m_osd.x() + 4;
    int y = m_osd.y() + 4;
    int w = m_osd.width() - 8;
    int h = m_osd.height() - 8;
    fb.osd(x, y, w, h);

    m_index = 0;

    this->__mute();
}

ybh_wIntercom::~ybh_wIntercom()
{
	fb.osd(0, 0, 0, 0);
}

// void ybh_wIntercom::start(void)
// {
//     int x = m_osd.x() + 4;
//     int y = m_osd.y() + 4;
//     int w = m_osd.width() - 8;
//     int h = m_osd.height() - 8;

//     dmsg req;
//     dxml p;
//     p.setInt("/param/x", x);
//     p.setInt("/param/y", y);
//     p.setInt("/param/width", w);
//     p.setInt("/param/height", h);
//     req.request("/talk/vo_start", p.data());
//     sCaller.mute(0, 1);
// }

// void ybh_wIntercom::stop(void)
// {
//     if (sCaller.mode() == sCaller.NONE)
//         return;
// }

void ybh_wIntercom::doStart(void)
{
    if (sCaller.mode() != sCaller.NONE)
        return;

    char s[128], s2[128];
	sprintf(s, "%d%04d%02d%02d%02d", m_index+2, sys.talk.build(), sys.talk.unit(), sys.talk.floor(), sys.talk.family());
	sprintf(s2, "H%04d%02d%02d%02d%c", sys.talk.build(), sys.talk.unit(), sys.talk.floor(), sys.talk.family(), '0'+m_index);

    printf("%s\n", s);

	sCaller.query(s);
	usleep(100*1000);
	sCaller.q600(s2);
	// this->showPrompt(1);
    
}

void ybh_wIntercom::doStop(void)
{
    sCaller.stop();
}

void ybh_wIntercom::doEvent(mEvent *e)
{
    mWindow::doEvent(e);

    if(e->type == mEvent::TouchEnd) {
        if (e->wParam == m_exit.id()) {
            wMain *w = new wMain();
            w->show();
        } else if (e->wParam == m_mute.id()) {
			if (sys.mute.m_enable) {
				sys.mute.m_enable = 0;
			} else {
				sys.mute.m_enable = 1;
				sys.mute.m_ts = time(NULL);
			} 
		} else if (e->wParam == m_start.id()) {
            //this->__err();
            this->doStart();
        } else if (e->wParam == m_stop.id()) {
            this->doStop();
        } else if (e->wParam == m_list.id()) {
            this->list_data();
            list = !list;
        }
    } 
}

void ybh_wIntercom::__mute(void)
{
	if (m_mute_st != sys.mute.m_enable) {
		m_mute_st = sys.mute.m_enable;
		if (m_mute_st)
			m_mute.load(m_style, "mute2");
		else
			m_mute.load(m_style, "mute");
	}    
}

void ybh_wIntercom::doTimer(void)
{
    
    this->__mute();

    if (sCaller.mode() == sCaller.QUERY) {
        if (sCaller.sip.url) {
            sCaller.m700(sCaller.sip.url);
        } else if (sCaller.ts() >= 1 && sCaller.d600.host) {
            sCaller.m600(sCaller.d600.host, sCaller.d600.ip);
        } else if (sCaller.ts() >= 2) {
            sCaller.stop();
        }
    }


    if (m_ts && labs(time(NULL) - m_ts) > 2) {
        m_ts = 0;
        m_err.hide();
    }
#if 0
    /* 这样虽然也可以，但是会造成系统阻塞，运行起来会很卡 */
    sleep(6);
    m_err.hide();
#endif

    mWindow::doTimer();
}

void ybh_wIntercom::__err(void)
{
    m_ts = time(NULL);
    m_err.show(); 
}

void ybh_wIntercom::list_data(void)
{
    if(!list) 
        m_bkg_list.show();
    else
        m_bkg_list.hide();
}