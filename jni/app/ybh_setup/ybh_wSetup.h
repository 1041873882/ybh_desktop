
#ifndef __YBH_W_SETUP_H__
#define __YBH_W_SETUP_H__

#include "mWindow.h"
#include "mButton.h"
#include "mEdit.h"

class ybh_wSetup : public mWindow {
public:
	ybh_wSetup();
	virtual ~ybh_wSetup();

	void select(int menu);

	void doEvent(mEvent *e);

private:
	mWindow *m_label;

	mPixmap m_bkg;
	mButton m_status;
	mButton m_network;
	mButton m_sip;
	mButton m_password;
	mButton m_intercom;
	mButton m_general;


	mText m_login_text;
	mEdit m_login;

	int m_select;

	mButton m_exit;
};

#endif
