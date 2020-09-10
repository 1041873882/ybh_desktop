#ifndef __YBH_W_INTERCOM_H__
#define __YBH_W_INTERCOM_H__

#include "mWindow.h"
#include "mButton.h"
#include "mText.h"

#define MAX_LIST_DATA 8

class ybh_wIntercom : public mWindow {
public:
    ybh_wIntercom();
    virtual ~ybh_wIntercom();

    void doEvent(mEvent *e);
    void doTimer(void);
    void __mute(void);
    void __err(void);
    void list_data(void);

    // void start(void);
    // void stop(void);

    void doStart(void);  //被通话？
    void doStop(void);

    // void showData(void);
    // void showPrompt(void);

    volatile bool list = false;


private:
    int m_mute_st;

    mPixmap m_bkg;
    mPixmap m_err;
    mPixmap m_bkg_list;
    mPixmap m_osd;
    mText m_title[4];

    mButton m_exit;
    mButton m_list;
    mButton m_next;
    mButton m_prev;
    mButton m_unlock1;
    mButton m_unlock2;    
    mButton m_start;
    mButton m_stop;
    mButton m_mute;
    mButton m_list_data[MAX_LIST_DATA];

    int m_mode;  //0:大门口机 1:小门口机
    int m_index; //门口机编号

    time_t m_ts;
};

#endif