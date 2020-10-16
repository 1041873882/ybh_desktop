#ifndef __W_MAIN_H__
#define __W_MAIN_H__

#include "mWindow.h"
#include "mButton.h"
#include "wTimeSetup.h"

class wMain : public mWindow {
public:
	wMain();
	virtual ~wMain();

	void doEvent(mEvent *e);
	void doTimer(void);

	void loadToolbar(void);

	mPixmap m_bkg;

private:
	//mPixmap m_bkg;
	mPixmap m_logo;
	mPixmap m_bkg_weather;
	mPixmap m_bkg_time;
	mPixmap m_weather;

#if 0 
	mButton m_talk;
	mButton m_security;
	mButton m_smart;
	mButton m_apps;
	mButton m_setup;
#endif

    mButton m_keys;
    mButton m_intercom;
    mButton m_cameras;
    mButton m_records;
    mButton m_setup;
	mButton m_wallpaper;
	mButton m_timesetup;


    mText m_time;
	mText m_date;
	mText m_ip_err;
	mText m_ip_err2;
	mText m_weather_text;
	mText m_time_hour;

	mButton m_bar_mute;
	mPixmap m_bar_eth;

	uint32_t m_ts_st;
	uint32_t m_ts_st_h;
	int m_eth_st;
	int m_proxy_st;
	int m_mute_st;
	int m_msg_st;
	int m_defence_st;
	int m_ip_st;

	int m_point;
	int m_x, m_y;
	int m_x2, m_y2;
};

//extern mWindow *SysMain;

#endif
