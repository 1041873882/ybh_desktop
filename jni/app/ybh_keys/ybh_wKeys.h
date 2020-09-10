#ifndef __YBH_W_KEYS_H__
#define __YBH_W_KEYS_H__

#include "mWindow.h"
#include "mButton.h"
#include "mText.h"

class ybh_wKeys : public mWindow {
public:
    ybh_wKeys();
    virtual ~ybh_wKeys();

    void doEvent(mEvent *e);

private:
    mPixmap m_bkg;
    mText m_title;

    mButton m_exit;



};

#endif