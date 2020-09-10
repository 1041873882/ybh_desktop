#ifndef __YBH_W_SETUP_STATUS_LABEL_H__
#define __YBH_W_SETUP_STATUS_LABEL_H__

#include "mWindow.h"
#include "mText.h"
#include "mButton.h"

class ybh_wSetupStatusLabel : public mWindow {
public:
    ybh_wSetupStatusLabel();
    virtual ~ybh_wSetupStatusLabel();

    void doEvent(mEvent *e);

private:
    mText m_version_text;
    mText m_version;
    mText m_ip_text;
    mText m_ip;
    mText m_mac_text;
    mText m_mac;
    mText m_sip_text;
    mText m_sip;
    mText m_user_text;
    mText m_user; 

    mButton m_reboot;
};

#endif