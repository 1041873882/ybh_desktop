
#include <fcntl.h>
#include "SysSound.h"
#include "d600.h"
#include "sMisc.h"
#include "sys.h"

#define SYS_CFG		"/dnake/cfg/sys.xml"
#define SYS_CFG2	"/dnake/cfg/sys2.xml"

int sys_mode = SYS_NORMAL;
int sys_factory = 0; //生产模式

__sys sys;

__sys::__sys()
{
	mute.m_enable = 0;
	user.login = 0;
	admin.login = 0;

	int fd = ::open("/dev/urandom", 0);
	if (fd < 0) {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		::srand(clock()+tv.tv_usec);
	} else {
		unsigned long seed;
		::read(fd, &seed, sizeof(seed));
		::srand(seed);
		::close(fd);
	}

	m_limit = 0;
	FILE *fp = fopen("/dnake/etc/limit", "r");
	if (fp) {
		fscanf(fp, "%d", &m_limit);
		fclose(fp);
	}
}

__sys::~__sys()
{
}

void __sys::load(void)
{
	dxml p;
	bool ok = p.loadFile(SYS_CFG);
	if (!ok)
		p.loadFile(SYS_CFG2);

	user.passwd(p.getText("/sys/user/passwd", "1234"));
	user.tz(p.getText("/sys/user/tz", "+08:00"));
	user.language(p.getInt("/sys/user/language", 0));
	user.ring(p.getInt("/sys/user/ring", 0));
	user.ban(p.getInt("/sys/user/ban", 0));

	settime.date(p.getInt("/sys/settime/date", 0));
	settime.time_h(p.getText("/sys/settime/time_h", "00"));	
	settime.time_m(p.getText("/sys/settime/time_m", "00"));	
	settime.time_s(p.getText("/sys/settime/time_s", "00"));	
	settime.date_y(p.getText("/sys/settime/date_y", "2020"));
	settime.date_m(p.getText("/sys/settime/date_m", "01"));
	settime.date_d(p.getText("/sys/settime/date_d", "01"));

	dst.enable(p.getInt("/sys/dst/enable", 0));
	dst.start_mon(p.getInt("/sys/dst/start_mon", 1));	
	dst.start_week(p.getInt("/sys/dst/start_week", 1));	
	dst.start_date(p.getInt("/sys/dst/start_date", 0));	//从星期天开始算，星期天表示0.星期一表示1
	dst.start_time(p.getText("/sys/dst/start_time", "00:00"));
	dst.end_mon(p.getInt("/sys/dst/end_mon", 1));	
	dst.end_week(p.getInt("/sys/dst/end_week", 1));	
	dst.end_date(p.getInt("/sys/dst/end_date", 0));	
	dst.end_time(p.getText("/sys/dst/end_time", "01:01"));


	admin.passwd(p.getText("/sys/admin/passwd", "123456"));

	char s[32];
	sprintf(s, "BC:F8:11:%02X:%02X:%02X", (uint8_t)(rand()%15), (uint8_t)(rand()%255), (uint8_t)(rand()%255));
	net.mac(p.getText("/sys/network/mac", s));
	net.dhcp(p.getInt("/sys/network/dhcp", 0));
	net.ip(p.getText("/sys/network/ip", "192.168.68.90"));
	net.mask(p.getText("/sys/network/mask", "255.255.255.0"));
	net.gateway(p.getText("/sys/network/gateway", "192.168.68.1"));
	net.dns(p.getText("/sys/network/dns", "8.8.8.8"));

	net.ntp_enable(p.getInt("/sys/network/ntp_enable", 1));
	net.ntp(p.getText("/sys/network/ntp", "202.120.2.101"));
	talk.build(p.getInt("/sys/talk/building", 1));
	talk.unit(p.getInt("/sys/talk/unit", 1));
	talk.floor(p.getInt("/sys/talk/floor", 11));
	talk.family(p.getInt("/sys/talk/family", 11));
	talk.dcode(p.getInt("/sys/talk/dcode", 0));
	sprintf(s, "%d%04d", rand()%100, rand()%10000);
	talk.sync(p.getText("/sys/talk/sync", s));
	talk.server(p.getText("/sys/talk/server", "192.168.68.1"));
	talk.passwd(p.getText("/sys/talk/passwd", "123456"));
	talk.arp(p.getInt("/sys/talk/arp", 0));
	talk.answer(p.getInt("/sys/talk/answer", 0));

	sip.enable(p.getInt("/sys/sip/enable", 0));
	sip.proxy(p.getText("/sys/sip/proxy", "sip:192.168.68.1"));
	sip.realm(p.getText("/sys/sip/realm", "192.168.68.1"));
	sip.user(p.getText("/sys/sip/user", "100"));
	sip.passwd(p.getText("/sys/sip/passwd", "123456"));
	sip.host2id(p.getInt("/sys/sip/host2id", 1));
	sip.timeout(p.getInt("/sys/sip/timeout", 300));

	stun.ip(p.getText("/sys/stun/ip", "192.168.68.1"));
	stun.port(p.getInt("/sys/stun/port", 5060));

	volume.talk(p.getInt("/sys/volume/talk", 0));
	volume.music(p.getInt("/sys/volume/music", 0));

	payload.h264(p.getInt("/sys/payload/h264", 102));

	lcd.bright(p.getInt("/sys/lcd/bright", 200));

	dtmf.enable(p.getInt("/sys/dtmf/enable", 0));
	dtmf.data(p.getText("/sys/dtmf/data", "#"));

	mqtt.enable(p.getInt("/sys/mqtt/enable", 0));
	mqtt.url(p.getText("/sys/mqtt/url", "192.168.11.40"));
	mqtt.port(p.getInt("/sys/mqtt/port", 1883));
	mqtt.user(p.getText("/sys/mqtt/user", "admin"));
	mqtt.password(p.getText("/sys/mqtt/password", "password"));
	mqtt.group(p.getText("/sys/mqtt/group", "123456"));

	if (!ok)
		this->save();
	this->setHttpUser();
	this->setTZ();
	this->setTime();
	c600.setid();

	if (m_limit == 220) {
		//220暂时只做简体中文
		user.language(0);
	}
}

void __sys::save(void)
{
	dxml p;
	p.setText("/sys/user/passwd", user.passwd());
	p.setText("/sys/user/tz", user.tz());
	p.setInt("/sys/user/language", user.language());
	p.setInt("/sys/user/ring", user.ring());
	p.setInt("/sys/user/ban", user.ban());

	p.setInt("/sys/settime/date", settime.date());
	p.setText("/sys/settime/time_h", settime.time_h());	//YBH 2020 8/24
	p.setText("/sys/settime/time_m", settime.time_m());	//YBH 2020 8/24
	p.setText("/sys/settime/time_s", settime.time_s());	//YBH 2020 8/24
	p.setText("/sys/settime/date_y", settime.date_y());
	p.setText("/sys/settime/date_m", settime.date_m());
	p.setText("/sys/settime/date_d", settime.date_d());

	p.setInt("/sys/dst/enable", dst.enable());
	p.setInt("/sys/dst/start_mon", dst.start_mon());
	p.setInt("/sys/dst/start_week", dst.start_week());
	p.setInt("/sys/dst/start_date", dst.start_date());
	p.setText("/sys/dst/start_time", dst.start_time());
	p.setInt("/sys/dst/end_mon", dst.end_mon());
	p.setInt("/sys/dst/end_week", dst.end_week());
	p.setInt("/sys/dst/end_date", dst.end_date());
	p.setText("/sys/dst/end_time", dst.end_time());

	p.setText("/sys/admin/passwd", admin.passwd());

	p.setText("/sys/network/mac", net.mac());
	p.setInt("/sys/network/dhcp", net.dhcp());
	p.setText("/sys/network/ip", net.ip());
	p.setText("/sys/network/mask", net.mask());
	p.setText("/sys/network/gateway", net.gateway());
	p.setText("/sys/network/dns", net.dns());
	p.setText("/sys/network/ntp", net.ntp());
	p.setInt("/sys/network/ntp_enable", net.ntp_enable());

	p.setInt("/sys/talk/building", talk.build());
	p.setInt("/sys/talk/unit", talk.unit());
	p.setInt("/sys/talk/floor", talk.floor());
	p.setInt("/sys/talk/family", talk.family());
	p.setInt("/sys/talk/dcode", talk.dcode());
	p.setText("/sys/talk/sync", talk.sync());
	p.setText("/sys/talk/server", talk.server());
	p.setText("/sys/talk/passwd", talk.passwd());
	p.setInt("/sys/talk/arp", talk.arp());
	p.setInt("/sys/talk/answer", talk.answer());

	p.setInt("/sys/sip/enable", sip.enable());
	p.setText("/sys/sip/proxy", sip.proxy());
	p.setText("/sys/sip/realm", sip.realm());
	p.setText("/sys/sip/user", sip.user());
	p.setText("/sys/sip/passwd", sip.passwd());
	p.setInt("/sys/sip/host2id", sip.host2id());
	p.setInt("/sys/sip/timeout", sip.timeout());

	p.setText("/sys/stun/ip", stun.ip());
	p.setInt("/sys/stun/port", stun.port());

	p.setInt("/sys/volume/talk", volume.talk());
	p.setInt("/sys/volume/music", volume.music());

	p.setInt("/sys/payload/h264", payload.h264());

	p.setInt("/sys/lcd/bright", lcd.bright());

	p.setInt("/sys/dtmf/enable", dtmf.enable());
	p.setText("/sys/dtmf/data", dtmf.data());

	p.setInt("/sys/mqtt/enable", mqtt.enable());
	p.setText("/sys/mqtt/url", mqtt.url());
	p.setInt("/sys/mqtt/port", mqtt.port());
	p.setText("/sys/mqtt/user", mqtt.user());
	p.setText("/sys/mqtt/password", mqtt.password());
	p.setText("/sys/mqtt/group", mqtt.group());

	p.saveFile(SYS_CFG);
	p.saveFile(SYS_CFG2);
	system("sync");

	this->setHttpUser();
	this->setTZ();
	this->setTime();
	c600.setid();
	sound.load();

	dmsg req;
	req.request("/talk/setid", NULL);
	req.request("/media/setup", NULL);

	system("stop dhcpcd_eth1");
	system("stop hostapd");
}

static const char *gdate_table[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static const char *GCC_DATE(void)
{
	static char s[128];
	char m[32];
	int y, d;

	sscanf(__DATE__, "%s %d %d", m, &d, &y);
	for (int i=0; i<12; i++) {
		if (!strcasecmp(m, gdate_table[i])) {
			sprintf(s, "%04d%02d%02d", y, i+1, d);
			return s;
		}
	}
	return "err";
}

void __sys::setEthIp(void)
{
	char s[128];
	system("ifconfig eth0 0.0.0.0");
	sprintf(s, "ifconfig eth0 %s netmask %s up", net.ip(), net.mask());
	system(s);
	sprintf(s, "route add default gw %s dev eth0", net.gateway());
	system(s);
	sprintf(s, "setprop net.dns1 %s", net.dns());
	system(s);
	system("setprop net.dns2 114.114.114.114");
	if (net.dhcp()) {
		system("start dhcpcd_eth0");
	} else {
		system("stop dhcpcd_eth0");
	}
}

void __sys::setEthMac(void)
{
	char s[128];
	system("busybox ifconfig eth0 down");
	sprintf(s, "busybox ifconfig eth0 hw ether %s", net.mac());
	system(s);
	system("busybox ifconfig eth0 up");
	usleep(400*1000);
	this->setEthIp();
}

void __sys::setHttpUser(void)
{
	FILE *fp = fopen("/var/etc/httppasswd", "w+");
	if (fp) {
		fprintf(fp, "%s:%s\n", "admin", admin.passwd());
		fprintf(fp, "%s:%s\n", "special", admin.passwd());
		fprintf(fp, "%s:%s\n", "user", user.passwd());
		fclose(fp);
	}
	fp = fopen("/var/etc/language", "w+");
	if (fp) {
		fprintf(fp, "%s\n", user.language() ? "EN" : "CHS");
		fclose(fp);
	}
}

void __sys::setTZ(void)
{
	char tz[64];
	char dst[128];
	char s1[3];
	char s2[3];
	int h;
	int m;
	const char *p = user.tz();
	memcpy(s1, p+1, 2); s1[2] = 0;
	memcpy(s2, p+4, 2); s2[2] = 0;
	h = atoi(s1);
	m = atoi(s2);
	
	if (sys.dst.m_enable) {
		if (*p == '-') {
			h -= 1;
			sprintf(dst, "GMT+%s:00NZDT+%02d:%02d:00,M%d.%d.%d/%s:00,M%d.%d.%d/%s:00", 
						p+1, h, m, sys.dst.m_start_mon, sys.dst.m_start_week, sys.dst.m_start_date, sys.dst.start_time(), 
							sys.dst.m_end_mon, sys.dst.m_end_week, sys.dst.m_end_date, sys.dst.end_time());
		} else {
			h += 1;
			sprintf(dst, "GMT-%s:00DST-%02d:%02d:00,M%d.%d.%d/%s:00,M%d.%d.%d/%s:00", 
						p+1, h, m, sys.dst.m_start_mon, sys.dst.m_start_week, sys.dst.m_start_date, sys.dst.start_time(), 
							sys.dst.m_end_mon, sys.dst.m_end_week, sys.dst.m_end_date, sys.dst.end_time());
		}
		setenv("TZ", dst, 1);		//orign YBH 2020 8/27
		printf("sys dst = %s\n",dst);		
	} else {
		if (*p == '-')
			sprintf(tz, "GMT+%s", p+1);
		else
			sprintf(tz, "GMT-%s", p+1);	
		setenv("TZ", tz, 1);		//orign YBH 2020 8/27	
		printf("sys tz = %s\n",tz);
	}
}

void __sys::setTime(void)
{
	int ret = 0;
	struct tm time_tm;
	struct timeval time_tv;
	time_t timep;
	char p[128];

	sprintf(p, "%s-%s-%s %s:%s:%s", settime.date_y(), settime.date_m(), settime.date_d(), settime.time_h(), settime.time_m(), settime.time_s());

    sscanf(p, "%d-%d-%d %d:%d:%d", &time_tm.tm_year, &time_tm.tm_mon, &time_tm.tm_mday, &time_tm.tm_hour, &time_tm.tm_min, &time_tm.tm_sec);
    time_tm.tm_year -= 1900;
    time_tm.tm_mon -= 1;
    time_tm.tm_wday = 0;
    time_tm.tm_yday = 0;
    time_tm.tm_isdst = -1;	/* 0不使用夏令时 -1由系统判断是否使用夏令时， 正数使用夏令时 */
	timep = mktime(&time_tm);
	time_tv.tv_sec = timep;
	time_tv.tv_usec = 0;

	ret = settimeofday(&time_tv, NULL); /* 设置时间和当前时区 */
	if (ret != 0) {
		fprintf(stderr, "settimeofday failed\n");
	}
	printf("settime = %s\n", p);
}

void __sys::resetMac(void)
{
	char s[32];
	sprintf(s, "BC:F8:11:%02X:%02X:%02X", (uint8_t)(rand()%15), (uint8_t)(rand()%255), (uint8_t)(rand()%255));
	net.mac(s);
}

void __sys::version(char *s)
{
	sprintf(s, "%d.%d.%d %s", SYS_MAJOR, SYS_MINOR, SYS_MINOR2, GCC_DATE());
}

const char *__sys::style(void)
{
	if (user.language() == LANG_EN)
		return "style_en.xml";
	else if (user.language() == LANG_CHT)
		return "style_cht.xml";
	else if (user.language() == LANG_RUS)
		return "style_rus.xml";
	else if (user.language() == LANG_PL)
		return "style_pl.xml";
	else if (user.language() == LANG_FR)
		return "style_fr.xml";
	else if (user.language() == LANG_TUR)
		return "style_tur.xml";
	return "style.xml";
}

#include <sys/system_properties.h>

static int VmRSS(void)
{
	int result = 0;
	char s[2*1024];
	sprintf(s, "/proc/%d/status", getpid());
	FILE *fp = fopen(s, "r");
	if (fp) {
		int r = fread(s, 1, sizeof(s)-1, fp);
		if (r > 0) {
			s[r] = 0;
			char *p = strstr(s, "VmRSS:");
			if (p) {
				p += strlen("VmRSS:");
				result = atoi(p);
			}
		}
		fclose(fp);
	}
	return result;
}

void __sys::process(void)
{
	static time_t ts = 0;
	if (time(NULL) != ts) {
		if (ethtool_get_link()) {
			char result[PROP_VALUE_MAX];
			__system_property_get("init.svc.dhcpcd_eth0", result);
			int running = strcmp(result, "running") ? 0 : 1;
			if (net.dhcp()) {
				if (!running)
					system("start dhcpcd_eth0");
			} else {
				if (running)
					system("stop dhcpcd_eth0");
			}
		}
		// if (VmRSS() >= 12*1024) //内存占用太大
		// 	exit(-1);
		ts = time(NULL);
	}
}