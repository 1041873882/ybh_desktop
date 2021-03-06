
#include "sys.h"
#include "SysSound.h"

SysSound sound;

SysSound::SysSound()
{
	m_press = "/dnake/sound/press.wav";
	m_ring[0] = "/dnake/sound/ring1.wav";
	m_ring[1] = "/dnake/sound/ring2.wav";
	m_ring[2] = "/dnake/sound/ring3.wav";
	m_ring[3] = "/dnake/sound/ring4.wav";
	m_ringback = "/dnake/sound/ringback.wav";
	m_setup_ok = "/dnake/sound/setup_ok.wav";
	m_setup_err = "/dnake/sound/setup_err.wav";
	m_passwd_err = "/dnake/sound/passwd_err.wav";
	m_defence_on = "/dnake/sound/defence_on.wav";
	m_defence_off = "/dnake/sound/defence_off.wav";
	m_defence_delay = "/dnake/sound/defence_delay.wav";

	m_bell = "/dnake/sound/bell.wav";

	m_alarm_delay = "/dnake/sound/alarm_delay.wav";
	m_alarm = "/dnake/sound/alarm.wav";



#if 0
	dxml p;
	dmsg req;
	if (req.request("/media/player/query", NULL) != 200) {
		this->play(m_press, 0);
	}
	p.setText("/params/url", m_press);
	y_press.press(p.getInt("/params/url", 1));
	p.setInt("/params/cycle", y_press.press());
	req.request("/media/player/start", p.data());
#endif


}

SysSound::~SysSound()
{
}

void SysSound::load(void)
{
	if (sys.user.language()) {
		//�Ǽ�������
		m_setup_ok = "/dnake/sound/en/setup_ok.wav";
		m_setup_err = "/dnake/sound/en/passwd_err.wav";
		m_passwd_err = "/dnake/sound/en/passwd_err.wav";
		m_defence_on = "/dnake/sound/en/defence_on.wav";
		m_defence_off = "/dnake/sound/en/defence_off.wav";
		m_defence_delay = "/dnake/sound/en/defence_delay.wav";
		m_alarm_delay = "/dnake/sound/en/alarm_delay.wav";
	} else {
		m_setup_ok = "/dnake/sound/setup_ok.wav";
		m_setup_err = "/dnake/sound/setup_err.wav";
		m_passwd_err = "/dnake/sound/passwd_err.wav";
		m_defence_on = "/dnake/sound/defence_on.wav";
		m_defence_off = "/dnake/sound/defence_off.wav";
		m_defence_delay = "/dnake/sound/defence_delay.wav";
		m_alarm_delay = "/dnake/sound/alarm_delay.wav";
	}
}

void SysSound::play(const char *url, int cycle)
{
	dxml p;
	dmsg req;
	p.setText("/params/url", url);
	p.setInt("/params/cycle", cycle);
	req.request("/media/player/start", p.data());
}

#if 1
void SysSound::press(void)
{
	dmsg req;
	if (req.request("/media/player/query", NULL) != 200) {
		this->play(m_press, 0);
	}
}
#endif

void SysSound::ringing(void)
{
	this->stop();

	int n = sys.user.ring();
	if (n >= 0 && n < 4) {
		this->play(m_ring[n], 1);
	} else {
		this->play(m_ring[0], 1);
	}
}

void SysSound::ringback(void)
{
	this->stop();
	this->play(m_ringback, 1);
}

void SysSound::setup_ok(void)
{
	this->stop();
	this->play(m_setup_ok, 0);
}

void SysSound::setup_err(void)
{
	this->stop();
	this->play(m_setup_err, 0);
}

void SysSound::passwd_err(void)
{
	this->play(m_passwd_err, 0);
}

void SysSound::defence_on(void)
{
	this->play(m_defence_on, 0);
}

void SysSound::defence_off(void)
{
	this->play(m_defence_off, 0);
}

void SysSound::defence_delay(void)
{
	this->play(m_defence_delay, 0);
}

void SysSound::bell(void)
{
	dmsg req;
	if (req.request("/media/player/query", NULL) != 200) {
		this->play(m_bell, 0);
	}
}

void SysSound::alarm_delay(void)
{
	this->play(m_alarm_delay, 0);
}

void SysSound::alarm(void)
{
	this->play(m_alarm, 1);
}

void SysSound::stop(void)
{
	dmsg req;
	req.request("/media/player/stop", NULL);
}
