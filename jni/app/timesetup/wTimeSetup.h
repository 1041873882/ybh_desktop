#ifndef __W_TIME_SETUP_H__
#define __W_TIME_SETUP_H__

#include "mWindow.h"
#include "mCombox.h"
#include "mSeekBar.h"
#include "mEdit.h"
#include "mButton.h"

class wTimeSetup : public mWindow {
public:
    wTimeSetup();
    virtual ~wTimeSetup();

    bool ntp = false;

    // volatile bool press_ring = false;
    void doEvent(mEvent *e);
    void doTimer(void);
    void enable(void);
    void save(void);

    static int m_setntp;
    static int m_settime;
    static int m_setdate;
    static int m_setdst;

private:
    mPixmap m_bkg;

    mText m_ntp_text;
    mButton m_ntp;

    mText m_ntp_ip_text;
    mEdit m_ntp_ip;

    mText m_hour_text;
    mButton m_hour;

    mText m_timezone_text;
    mEdit m_timezone;

    mText m_time_text;
    mText m_time_text1;
    mText m_time_text2;
    mEdit m_time_h;
    mEdit m_time_m;
    mEdit m_time_s;

    mText m_dateformat_text;
    mCombox m_dateformat;

    mText m_date_text;
    mText m_date_text1;
    mText m_date_text2;
    mEdit m_date_y;
    mEdit m_date_m;
    mEdit m_date_d;    


    mText m_dst_text;
    mButton m_dst;

    mText m_dst_start;
    mText m_start_text[4];
    mCombox m_start_mon;
    mCombox m_start_week;
    mCombox m_start_date;  
    mEdit m_start_time;  

    mText m_dst_end;
    mText m_end_text[4];
    mCombox m_end_mon;
    mCombox m_end_week;
    mCombox m_end_date;  
    mEdit m_end_time;  

    mButton m_ok;
    mButton m_exit;
    
    int m_ntp_st;
    int m_time_set;
    int m_dst_set;

};


#endif