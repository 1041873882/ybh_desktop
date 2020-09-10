
#ifndef __YBH_W_CAMERAS_H__
#define __YBH_W_CAMERAS_H__

#include "mButton.h"
#include "mWindow.h"

class ybh_wCameras : public mWindow {
public:
    ybh_wCameras();
    virtual ~ybh_wCameras();

    void doEvent(mEvent *e);



private:
    mPixmap m_bkg;

    mButton m_exit;
    mButton m_list;
    mButton m_next;
    mButton m_prev;
    mButton m_stop;
    mButton m_start;

    mText m_prompt;

};

#endif