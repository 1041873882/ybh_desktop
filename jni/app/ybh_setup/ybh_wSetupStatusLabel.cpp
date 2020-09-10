#include "sys.h"
#include "dxml.h"
#include "dmsg.h"
#include "sMisc.h"
#include "ybh_wSetupStatusLabel.h"

ybh_wSetupStatusLabel::ybh_wSetupStatusLabel() : mWindow("ybh_setup/status")
{
    char s[128];

    m_version_text.setParent(this);
    m_version_text.load(m_style, "version_text");
    m_version.setParent(this);
    m_version.load(m_style, "version");
    sys.version(s);
    m_version.setText(s);

    m_ip_text.setParent(this);
    m_ip_text.load(m_style, "ip_text");
    m_ip.setParent(this);
    m_ip.load(m_style, "ip");
    struct sockaddr_in addr;
    if (local_if_addr(&addr))
        m_ip.setText("0.0.0.0");
    else
        m_ip.setText(inet_ntoa(addr.sin_addr)); /* inet_ntoa函数用来将网络地址转换成“.”点隔的字符串格式。 */
    
    m_mac_text.setParent(this);
    m_mac_text.load(m_style, "mac_text");
    m_mac.setParent(this);
    m_mac.load(m_style, "mac");
    m_mac.setText(sys.net.mac());

    m_sip_text.setParent(this);
    m_sip_text.load(m_style, "sip_text");
    m_sip.setParent(this);
    m_sip.load(m_style, "sip");
    
    m_user_text.setParent(this);
    m_user_text.load(m_style, "user_text");
    m_user.setParent(this);
    m_user.load(m_style, "user");

    m_reboot.setParent(this);
    m_reboot.load(m_style, "reboot");

     
}

ybh_wSetupStatusLabel::~ybh_wSetupStatusLabel()
{
}

void ybh_wSetupStatusLabel::doEvent(mEvent *e)
{
    mWindow::doEvent(e);
}

