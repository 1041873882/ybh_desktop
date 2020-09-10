#ifndef __YBH_W_SETUP_SIP_LABEL_H__
#define __YBH_W_SETUP_SIP_LABEL_H__

#include "mButton.h"
#include "mText.h"
#include "mEdit.h"
#include "mWindow.h"

class ybh_wSetupSipLabel : public mWindow {
public:
    ybh_wSetupSipLabel();
    virtual ~ybh_wSetupSipLabel();

    void doEvent(mEvent *e);
    void loadToolbal(void);
    void doTimer(void);

    void save(void);

private:
    mText m_enable_txt;
    mButton m_enable;
    mText m_proxy_txt;
    mEdit m_proxy;
    mText m_realm_txt;
    mEdit m_realm;
    mText m_user_txt;
    mEdit m_user;
    mText m_password_txt;
    mEdit m_password;
    mText m_h264_txt;
    mEdit m_h264;
    mText m_stun_ip_txt;
    mEdit m_stun_ip;
    mText m_stun_port_txt;
    mEdit m_stun_port;

    int m_enable_st;

    mButton m_ok;
};


#endif 