#include "sys.h"
#include "SysSound.h"
#include "ybh_wSetupSipLabel.h"

ybh_wSetupSipLabel::ybh_wSetupSipLabel() : mWindow("ybh_setup/voip")
{
    char s[12];
    
    m_enable_txt.setParent(this);
    m_enable_txt.load(m_style, "enable_text");
    m_enable.setParent(this);

    m_ok.setParent(this);

    m_proxy_txt.setParent(this);
    m_proxy_txt.load(m_style, "proxy_text");
    m_proxy.setParent(this);
    m_proxy.load(m_style, "proxy");
    m_proxy.setText(sys.sip.proxy());
    //m_proxy.setMode(mEdit::IP);
    //fprintf(stderr,"%s \n",sys.sip.proxy());

    m_realm_txt.setParent(this);
    m_realm_txt.load(m_style, "realm_text");
    m_realm.setParent(this);
    m_realm.load(m_style, "realm");
    m_realm.setText(sys.sip.realm());
    m_realm.setMode(mEdit::IP);

    m_user_txt.setParent(this);
    m_user_txt.load(m_style, "user_text");
    m_user.setParent(this);
    m_user.load(m_style, "user");
    m_user.setText(sys.sip.user());

    m_password_txt.setParent(this);
    m_password_txt.load(m_style, "password_text");
    m_password.setParent(this);
    m_password.load(m_style, "password");
    m_password.setMode(mEdit::PASSWD);
    m_password.setText(sys.sip.passwd());
    m_password.setMax(10);

    m_h264_txt.setParent(this);
    m_h264_txt.load(m_style, "h264_text");
    m_h264.setParent(this);
    m_h264.load(m_style, "h264");
    sprintf(s, "%d", sys.payload.h264());
    m_h264.setText(s);

    m_stun_ip_txt.setParent(this);
    m_stun_ip_txt.load(m_style, "stun_ip_text");
    m_stun_ip.setParent(this);
    m_stun_ip.load(m_style, "stun_ip");
    m_stun_ip.setText(sys.stun.ip());
    m_stun_ip.setMode(mEdit::IP);

    m_stun_port_txt.setParent(this);
    m_stun_port_txt.load(m_style, "stun_port_text");
    m_stun_port.setParent(this);
    m_stun_port.load(m_style, "stun_port");
    sprintf(s, "%d", sys.stun.port());
    m_stun_port.setText(s);

    m_enable_st = -1;

    this->loadToolbal();
}

ybh_wSetupSipLabel::~ybh_wSetupSipLabel()
{
}

void ybh_wSetupSipLabel::doEvent(mEvent *e)
{
    mWindow::doEvent(e);
    if (e->type == mEvent::TouchEnd) {
        if (e->wParam == m_enable.id()) {
            if (sys.sip.m_enable) {
                sys.sip.m_enable = 0;
            } else {
                sys.sip.m_enable = 1;
            }
        }
    }



}

void ybh_wSetupSipLabel::loadToolbal(void)
{
    if (m_enable_st != sys.sip.m_enable) {
        m_enable_st = sys.sip.m_enable;
        if (m_enable_st)
            m_enable.load(m_style, "enable");
        else
            m_enable.load(m_style, "enable1"); 
    }
}

void ybh_wSetupSipLabel::doTimer()
{
    mWindow::doTimer();
    this->loadToolbal();
}