
#include "sys.h"
#include "SysSound.h"
#include "wSetupRoomLabel.h"
#include "wSetupNetworkLabel.h"
#include "wSetupNormalLabel.h"
#include "wSetupVersionLabel.h"
#include "wSetupPasswdLabel.h"
#include "wMain.h"
#include "ybh_wSetup.h"
#include "ybh_wSetupStatusLabel.h"
#include "ybh_wSetupSipLabel.h"
#include "ybh_wSetupGeneralLabel.h"

ybh_wSetup::ybh_wSetup() : mWindow("ybh_setup")
{
	m_bkg.setParent(this);
	m_bkg.load(m_style, "bkg");

	m_status.setParent(this);
	m_network.setParent(this);
	m_sip.setParent(this);
	m_password.setParent(this);
	m_intercom.setParent(this);
	m_general.setParent(this);

	m_login_text.setParent(this);
	m_login_text.load(m_style, "login_text");
	m_login.setParent(this);
	m_login.load(m_style, "login");
	m_login.setMode(mEdit::PASSWD);
	m_login.setMax(10);

	m_exit.setParent(this);
	m_exit.load(m_style, "exit");

	m_label = NULL;
	this->select(0); /* 指定一开始显示switch里的case 0内容 */
#if 0
	m_status.load(m_style, "status");
	m_network.load(m_style, "network");
	m_sip.load(m_style, "voip");
	m_password.load(m_style, "password");
	m_intercom.load(m_style, "intercom");
	m_general.load(m_style, "general");
#endif

}

ybh_wSetup::~ybh_wSetup()
{
	if (m_label) {
		delete m_label;
		m_label = NULL;
	}
}

static int factory_debug = 0;

#if 1
void ybh_wSetup::select(int menu)
{
	m_status.load(m_style, "status");
	m_network.load(m_style, "network");
	m_sip.load(m_style, "voip");
	m_password.load(m_style, "password");
	m_intercom.load(m_style, "intercom");
	m_general.load(m_style, "general");

	if (m_label) {
		delete m_label;
		m_label = NULL;
	}
	m_select = menu;

	m_login_text.hide();
	m_login.hide();

	switch(menu) {
	case 0:
		m_status.load(m_style, "status2");
		m_label = new ybh_wSetupStatusLabel();
		m_label->setParent(this);
		break;
	case 1:
		m_network.load(m_style, "network2");
		if (sys.admin.login) {
			m_label = new wSetupNetworkLabel();
			m_label->setParent(this);
		} else {
			m_login_text.show();
			m_login.show();
		}
		break;
	case 2:
		m_password.load(m_style, "password2");
		m_label = new wSetupPasswdLabel();
		m_label->setParent(this);
		break;
	case 3:
		m_sip.load(m_style, "voip2");
		m_label = new ybh_wSetupSipLabel();
		m_label->setParent(this);
		break;
	case 4:
		m_general.load(m_style, "general2");
		m_label = new ybh_wSetupGeneralLabel();
		m_label->setParent(this);
		break;
	}
}

void ybh_wSetup::doEvent(mEvent *e)
{
	mWindow::doEvent(e);

	if (e->type == mEvent::KeyPress) {
		if (e->wParam == KEY_M_MENU) {
			wMain *w = new wMain();
			w->show();
		}
	}
	if (e->type == mEvent::TouchEnd) {
		if (e->wParam == m_status.id()) {
			this->select(0);
		} else if (e->wParam == m_network.id()) {
			this->select(1);
		} else if (e->wParam == m_password.id()) {
			this->select(2);
		} else if (e->wParam == m_sip.id()) {
			this->select(3);
		} else if (e->wParam == m_general.id()) {
			this->select(4);
		} else if (e->wParam == m_exit.id()) {
			wMain *w = new wMain();
			w->show();
		} 
	} else if (e->type == mEvent::EditEnd) {
		if (e->wParam == m_login.id() && strlen(m_login.text()) > 0) {
			if (!strcmp(sys.admin.passwd(), m_login.text())) {
				sys.admin.login = 1;
				this->select(m_select);
			} else {
				sys.admin.login = 0;
				sound.passwd_err();
			}
		}
	}
}
#endif
