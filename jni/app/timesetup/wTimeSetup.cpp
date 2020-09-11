#include "sys.h"
#include "wTimeSetup.h"
#include "SysSound.h"
#include "mFB.h"
#include "wMain.h"
#include <stdio.h>

int wTimeSetup::m_settime = 0;
int wTimeSetup::m_setdate = 0;
int wTimeSetup::m_setntp = 0;
int wTimeSetup::m_setdst = 0;

wTimeSetup::wTimeSetup() : mWindow("ybh_timesetup")
{
    m_setntp = sys.net.m_ntp_enable;
    time_t now = time(NULL);
	struct tm *tm = localtime(&now);
    char s[128];
    int i;
    m_time_set = -1;
    m_ntp_st = -1;
    m_dst_set = -1;

    m_bkg.setParent(this);
    m_bkg.load(m_style, "bkg"); 

    m_ntp_text.setParent(this);
    m_ntp_text.load(m_style, "ntp_text");  

    m_ntp.setParent(this); 
    // m_ntp.load(m_style, "ntp2");

    m_ntp_ip_text.setParent(this);
    m_ntp_ip_text.load(m_style, "ntp_ip_text");
    m_ntp_ip.setParent(this);
    m_ntp_ip.load(m_style, "ntp_ip");
    m_ntp_ip.setText(sys.net.ntp());
    m_ntp_ip.setMode(mEdit::NONE);

    m_hour_text.setParent(this);
    m_hour_text.load(m_style, "hour_text");
    m_hour.setParent(this);

    m_timezone_text.setParent(this);
    m_timezone_text.load(m_style, "timezone_text");  
    m_timezone.setParent(this);
	m_timezone.load(m_style, "tz");
	m_timezone.setMode(mEdit::TZ);
	m_timezone.setText(sys.user.tz());
    
    m_time_text.setParent(this);
    m_time_text.load(m_style, "time_text");      
    m_time_text1.setParent(this);
    m_time_text1.load(m_style, "time_text1");    
    m_time_text2.setParent(this);
    m_time_text2.load(m_style, "time_text2");
    sprintf(s, "%02d", tm->tm_hour);    
    m_time_h.setParent(this);
    m_time_h.load(m_style, "time_h");
    m_time_h.setMax(2);
    m_time_h.setMode(mEdit::SETTIME);
    // m_time_h.setText(sys.settime.time_h());
    m_time_h.setText(s);
    sprintf(s, "%02d", tm->tm_min);  
    m_time_m.setParent(this);
    m_time_m.load(m_style, "time_m");
    m_time_m.setMax(2);
    m_time_m.setMode(mEdit::SETTIME);
    m_time_m.setText(s);
    sprintf(s, "%02d", tm->tm_sec);  
    m_time_s.setParent(this);
    m_time_s.load(m_style, "time_s");
    m_time_s.setMax(2);
    m_time_s.setMode(mEdit::SETTIME);
    m_time_s.setText(s);
    
    m_dateformat_text.setParent(this);
    m_dateformat_text.load(m_style, "dateformat_text");   
    m_dateformat.setParent(this);
    m_dateformat.load(m_style, "date");  
    m_dateformat.insert(m_style.getText("style/text/date1"));
    m_dateformat.insert(m_style.getText("style/text/date2"));
    m_dateformat.select(sys.settime.date());

    m_date_text.setParent(this);
    m_date_text.load(m_style, "date_text");   
    m_date_text1.setParent(this);
    m_date_text1.load(m_style, "date_text1");    
    m_date_text2.setParent(this);
    m_date_text2.load(m_style, "date_text2");  
    sprintf(s, "%04d", tm->tm_year+1900);  
    m_date_y.setParent(this);
    m_date_y.load(m_style, "date_y");
    m_date_y.setMax(4);
    m_date_y.setMode(mEdit::SETTIME);
    m_date_y.setText(s);
    sprintf(s, "%02d", tm->tm_mon+1);
    m_date_m.setParent(this);
    m_date_m.load(m_style, "date_m");
    m_date_m.setMax(2);
    m_date_m.setMode(mEdit::SETTIME);
    m_date_m.setText(s);
    sprintf(s, "%02d", tm->tm_mday);
    m_date_d.setParent(this);
    m_date_d.load(m_style, "date_d");
    m_date_d.setMax(2);
    m_date_d.setMode(mEdit::SETTIME);
    m_date_d.setText(s);

    m_dst_text.setParent(this);
    m_dst_text.load(m_style, "dst_text");    
    m_dst.setParent(this);

    m_dst_start.setParent(this);
    m_dst_start.load(m_style, "dst_start");   
    m_dst_end.setParent(this);
    m_dst_end.load(m_style, "dst_end");   

    for (i = 0; i < 4; i++) {
        m_start_text[i].setParent(this);
        m_end_text[i].setParent(this);
        sprintf(s, "start_text%d", i);
        m_start_text[i].load(m_style, s);     
        sprintf(s, "end_text%d", i);
        m_end_text[i].load(m_style, s);            
    }

    m_start_mon.setParent(this);
    m_start_mon.load(m_style, "start_mon");  
    m_end_mon.setParent(this);
    m_end_mon.load(m_style, "end_mon"); 
    for (i = 1; i <= 12; i++) {
        sprintf(s, "style/dsttext/mon%d", i);
        m_start_mon.insert(m_style.getText(s));
        m_end_mon.insert(m_style.getText(s));        
    }
    m_start_mon.select(sys.dst.start_mon()-1);
    m_end_mon.select(sys.dst.end_mon()-1);

    m_start_week.setParent(this);
    m_start_week.load(m_style, "start_week"); 
    m_start_date.setParent(this);
    m_start_date.load(m_style, "start_date");    
    m_end_week.setParent(this);
    m_end_week.load(m_style, "end_week"); 
    m_end_date.setParent(this);
    m_end_date.load(m_style, "end_date");    
    for (i = 1; i <= 7; i++) {
        sprintf(s, "style/dsttext/date%d", i);
        
        m_start_date.insert(m_style.getText(s));
        m_end_date.insert(m_style.getText(s));
        if (i < 6) {
            sprintf(s, "style/dsttext/week%d", i);
            m_start_week.insert(m_style.getText(s));
            m_end_week.insert(m_style.getText(s));
        }
    }
    m_start_week.select(sys.dst.start_week()-1);
    m_start_date.select(sys.dst.start_date());
    m_end_week.select(sys.dst.end_week()-1);
    m_end_date.select(sys.dst.end_date());

    m_start_time.setParent(this);
    m_start_time.load(m_style, "start_time");
    m_start_time.setMax(5);
    m_start_time.setMode(mEdit::DSTTIME);
    m_start_time.setText(sys.dst.start_time());

    m_end_time.setParent(this);
    m_end_time.load(m_style, "end_time");
    m_end_time.setMax(5);
    m_end_time.setMode(mEdit::DSTTIME);
    m_end_time.setText(sys.dst.end_time());

    m_ok.setParent(this);
    m_ok.load(m_style, "ok");

    m_exit.setParent(this);
    m_exit.load(m_style, "exit"); 

    this->enable();     
}

wTimeSetup::~wTimeSetup()
{
}

void wTimeSetup::doEvent(mEvent *e)
{
    mWindow::doEvent(e);
    if(e->type == mEvent::TouchEnd) {
        if (e->wParam == m_ntp.id()) {
            m_setntp = !m_setntp;
        } else if (e->wParam == m_exit.id()) {
            wMain *w = new wMain();
            w->show();
        } else if (e->wParam == m_ok.id()) {
            this->save();
        } else if (e->wParam == m_hour.id()) {
            m_settime = !m_settime;
        } else if (e->wParam == m_dst.id()) {
            m_setdst = !m_setdst;
        }
    }
}

void wTimeSetup::enable(void)
{
    // printf("okokoko\n");
    if (m_ntp_st != m_setntp) {
        m_ntp_st = m_setntp;
        if (m_ntp_st) {
			m_ntp.load(m_style, "ntp2");
        } else {
			m_ntp.load(m_style, "ntp1");
        }
    }

    if (m_time_set != m_settime) {
        m_time_set = m_settime;

        if (m_time_set) {
            m_hour.load(m_style, "hour2");            
        } else {
            m_hour.load(m_style, "hour1");              
        }
    }

    if (m_dst_set != m_setdst) {
        m_dst_set = m_setdst;
        if (m_dst_set) {
            m_dst.load(m_style, "dst2");            
        } else {
            m_dst.load(m_style, "dst1");              
        }
    }

}

void wTimeSetup::doTimer(void)
{
    mWindow::doTimer();
    this->enable();
}

void wTimeSetup::save(void)
{
    int err = 0;
	const char *old_ntp = m_ntp_ip.text();
    const char *old_tz = m_timezone.text();
    const char *old_time_h = m_time_h.text();
    const char *old_time_m = m_time_m.text();
    const char *old_time_s = m_time_s.text();
    const char *old_date_y = m_date_y.text();
    const char *old_date_m = m_date_m.text();
    const char *old_date_d = m_date_d.text();
    const char *dst_start_time = m_start_time.text();
    const char *dst_end_time = m_end_time.text();
	if (strlen(old_tz) < 6) {
		err = 1;
	} else {
		char s1[3], s2[3];
		memcpy(s1, old_tz+1, 2); s1[2] = 0;
		memcpy(s2, old_tz+4, 2); s2[2] = 0;
		if (atoi(s1) > 13 || atoi(s2) > 59)
		err = 1;
	}

    int y = atoi(old_date_y);
    int m = atoi(old_date_m);
    int d = atoi(old_date_d);

    /* 年份最大只能设置到2037年 */
    if (atoi(old_time_h) > 23 || atoi(old_time_m) > 59 || atoi(old_time_s) > 59 ||
            y > 2037 || y ==0  || m > 12 || m == 0 || d == 0) {
        err = 1;
    }

    if (old_date_y == "/0" || old_date_m == "/0" || old_date_d == "/0") {
        err = 1;
    }

    /* 判断是否为闰年 */
    if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (d > 30) 
            err = 1;
    } else if (m != 2) {
        if (d > 31)
            err = 1;
    }
    if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)) {
        if (m == 2) {
            if (d > 29) 
               err = 1;
        }
    } else {
        if (m == 2) {
            if (d > 28) 
                err = 1;
        }
    }

    if (!err) {
        sys.user.tz(old_tz);
        sys.net.ntp(old_ntp);
        sys.settime.time_h(old_time_h);
        sys.settime.time_m(old_time_m);
        sys.settime.time_s(old_time_s);
        sys.settime.date_y(old_date_y);
        sys.settime.date_m(old_date_m);
        sys.settime.date_d(old_date_d);
        sys.dst.start_time(dst_start_time);
        sys.dst.end_time(dst_end_time);
        sys.settime.date(m_dateformat.select());
        sys.dst.start_mon(m_start_mon.select()+1);
        sys.dst.start_week(m_start_week.select()+1);
        sys.dst.start_date(m_start_date.select());
        sys.dst.end_mon(m_end_mon.select()+1);
        sys.dst.end_week(m_end_week.select()+1);
        sys.dst.end_date(m_end_date.select());

        sys.net.ntp_enable(m_setntp);
        sys.dst.enable(m_setdst);
        sys.save();
        printf("%d\n", sys.settime.date());
        // sys.load();
        sound.setup_ok();
    } else {
        sound.setup_err();
    }
}

 





 