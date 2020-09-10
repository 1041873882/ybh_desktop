#ifndef __YBH_W_RECORDS_H__
#define __YBH_W_RECODES_H__

#include "mWindow.h"
#include "mButton.h"
#include "mText.h"

class ybh_wRecords : public mWindow {
public:
    ybh_wRecords();
    virtual ~ybh_wRecords();

    void doEvent(mEvent *e);

private:
    mPixmap m_bkg;
    mText m_title[4];

    mButton m_exit;
    mButton m_up;
    mButton m_call;
    mButton m_down;
    mButton m_del;


};

#endif