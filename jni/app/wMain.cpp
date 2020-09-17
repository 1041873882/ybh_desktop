
#include "sys.h"
#include "sMisc.h"
#include "SysCaller.h"
#include "SysLogger.h"
#include "SysSecurity.h"
#include "wSetup.h"
#include "ybh_wSetup.h"
#include "wIntercom.h"
#include "wMessage.h"
#include "wSecurity.h"
#include "wSmart.h"
#include "wSetup.h"
#include "wCalibrate.h"
#include "wIntercomCallLabel.h"
#include "wIntercomMonitorLabel.h"
#include "wSmartElevLabel.h"
#include "wMain.h"
#include "ybh_wRecords.h"
#include "ybh_wCameras.h"
#include "ybh_wIntercom.h"
#include "ybh_wKeys.h"
#include "ybh_wWallPaper.h"
#include "wTimeSetup.h"
#include "wTimeSetup.h"

//mWindow *SysMain = NULL;

int sys_ipwd_err = 0;
std::string sys_ipwd_ip = "";
std::string sys_ipwd_mac = "";

void sys_ipwd_result(int result, const char *ip, const char *mac)
{
	sys_ipwd_err = result;
	sys_ipwd_ip = ip;
	sys_ipwd_mac = mac;
	if (sys_ipwd_err)
		fb.enable(1);
}

wMain::wMain() : mWindow("ybh_main")
{
	int x;
	x = sys.user.bkg();

	const char *y_main_wall[] = {"bkg0", "bkg1", "bkg2", "bkg3"};

	m_bkg.setParent(this);
	if ( y_main_wall[x] != NULL) 
		m_bkg.load(m_style, y_main_wall[x]);
	else
		m_bkg.load(m_style, y_main_wall[0]);
		
	m_bkg.load(m_style, y_main_wall[x]);
	m_logo.setParent(this);
	m_logo.load(m_style, "logo");
	m_bkg_time.setParent(this);
	m_bkg_time.load(m_style, "bkg_time");
	m_bkg_weather.setParent(this);
	m_bkg_weather.load(m_style, "bkg_weather");
	m_weather.setParent(this);
	m_weather.load(m_style, "weather");
	m_weather_text.setParent(this);
	m_weather_text.load(m_style, "weather_text");	
	m_time_hour.setParent(this);
	m_time_hour.load(m_style, "time_hour");

	int logo = 0;
	FILE *fp = fopen("/dnake/etc/logo", "r");
	if (fp) {
		fscanf(fp, "%d", &logo);
		fclose(fp);
	}
	if (!logo)
		m_logo.hide();

	m_keys.setParent(this);
	m_keys.load(m_style, "keys");

	m_setup.setParent(this);
	m_setup.load(m_style, "setup");

    m_intercom.setParent(this);
    m_intercom.load(m_style, "intercom");

    m_cameras.setParent(this);
    m_cameras.load(m_style, "cameras");

    m_records.setParent(this);
    m_records.load(m_style, "records");

    m_date.setParent(this);
    m_date.load(m_style, "date");

	m_time.setParent(this);
	m_time.load(m_style, "time");

	m_wallpaper.setParent(this);
	m_wallpaper.load(m_style, "wallpaper");

	m_timesetup.setParent(this);
	m_timesetup.load(m_style, "timesetup");

	m_bar_mute.setParent(this);
	m_bar_eth.setParent(this);

    /* ip冲突显示ip地址和mac地址 */
	m_ip_err.setParent(this);
	m_ip_err.load(m_style, "ip_err");
	m_ip_err.hide();
	m_ip_err2.setParent(this);
	m_ip_err2.load(m_style, "ip_err2");
	m_ip_err2.hide();

	m_ts_st = -1;
	m_ts_st_h = -1;
	m_eth_st = -1;
	m_proxy_st = -1;
	m_mute_st = -1;
	m_msg_st = -1;
	m_defence_st = -1;
	m_ip_st = -1;

	m_point = 0;
	m_x = 10000;
	m_y = 10000;
	m_x2 = 0;
	m_y2 = 0;

	this->loadToolbar();
}

wMain::~wMain()
{
	//SysMain = NULL;
}

void wMain::doEvent(mEvent *e)
{
	mWindow::doEvent(e);
	if (e->type == mEvent::TouchEnd) {
		if (e->wParam == m_setup.id()) {
			ybh_wSetup *w = new ybh_wSetup();
			w->show();
		} else if (e->wParam == m_records.id()) {
			ybh_wRecords *w = new ybh_wRecords();
			w->show();
		} else if (e->wParam == m_cameras.id()) {
			ybh_wCameras *w = new ybh_wCameras();
			w->show();
		} else if (e->wParam == m_intercom.id()) {
			ybh_wIntercom *w = new ybh_wIntercom();
			w->show();
		} else if (e->wParam == m_keys.id()) {
			ybh_wKeys *w = new ybh_wKeys();
			w->show();
		} else if (e->wParam == m_wallpaper.id()) {
			ybh_wWallPaper *w = new ybh_wWallPaper();
			w->show();
		} else if (e->wParam == m_timesetup.id()) {
			wTimeSetup *w = new wTimeSetup;
			w->show();
		} else if (e->wParam == m_bar_mute.id()) {
			if (sys.mute.m_enable) {
				sys.mute.m_enable = 0;
			} else {
				sys.mute.m_enable = 1;
				sys.mute.m_ts = time(NULL);
			}
		}
	} else if (e->type == mEvent::TouchRaw) {
		int m = e->wParam;
		int x = e->lParam&0xFFFF;
		int y = (e->lParam>>16)&0xFFFF;
		if (m == 0) {
			int ok = 0;
			if (m_point > 20) {
				if (m_x2-m_x > 2400 && m_y2-m_y < 300) {
					ok = 1;
				} else if (m_y2-m_y > 2400 && m_x2-m_x < 300) {
					ok = 1;
				}
			}
			m_point = 0;
			m_x = 10000;
			m_y = 10000;
			m_x2 = 0;
			m_y2 = 0;
			if (ok) {
				wCalibrate *w = new wCalibrate();
				w->show();
			}
		} else {
			m_point++;
			if (m_x > x)
				m_x = x;
			if (m_y > y)
				m_y = y;
			if (m_x2 < x)
				m_x2 = x;
			if (m_y2 < y)
				m_y2 = y;
		}
	}
}

void wMain::loadToolbar(void)
{
	time_t now = time(NULL);
	struct tm *tm = localtime(&now);

	if (m_ts_st != tm->tm_min || m_ts_st_h != tm->tm_hour) {
		char s[128];
		strftime(s, sizeof(s), "%P", tm);
		m_time_hour.setText(s);		
		
		if (settime.m_settime) {
			strftime(s, sizeof(s), "%H : %M", tm);
			m_time.setText(s);
		} else {
			strftime(s, sizeof(s), "%I : %M", tm);
			m_time.setText(s);
		} 

		if (sys.settime.date() == 0) {
			strftime(s, sizeof(s), "%d %b %Y", tm);
			m_date.setText(s);
		} else if ( sys.settime.date() == 1 ) {
			strftime(s, sizeof(s), "%b %d %Y", tm);
			m_date.setText(s);
		}

		m_ts_st = tm->tm_min;
		m_ts_st_h = tm->tm_hour;	
	}
	if (m_eth_st != ethtool_get_link()) {
		m_eth_st = ethtool_get_link();
		if (m_eth_st)
			m_bar_eth.load(m_style, "toolbar/eth2");
		else
			m_bar_eth.load(m_style, "toolbar/eth");
	}
	if (m_mute_st != sys.mute.m_enable) {
		m_mute_st = sys.mute.m_enable;
		if (m_mute_st)
			m_bar_mute.load(m_style, "toolbar/mute2");
		else
			m_bar_mute.load(m_style, "toolbar/mute");
	}
	if (m_ip_st != sys_ipwd_err) {
		m_ip_st = sys_ipwd_err;
		if (m_ip_st) {
			char s[1024];
			sprintf(s, "%s %s", m_style.getText("/style/text/ip_err"), sys_ipwd_ip.c_str());
			m_ip_err.setText(s);
			m_ip_err.show();
			sprintf(s, "MAC: %s", sys_ipwd_mac.c_str());
			m_ip_err2.setText(s);
			m_ip_err2.show();
		} else {
			m_ip_err.setText("");
			m_ip_err.hide();
			m_ip_err2.setText("");
			m_ip_err2.hide();
		}
	}
}

void wMain::doTimer(void)
{
	mWindow::doTimer();
	this->loadToolbar();
}
