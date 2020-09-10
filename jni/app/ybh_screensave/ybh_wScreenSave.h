#ifndef __YBH_W_SCREENSAVE_H__
#define __YBH_W_SCREENSAVE_H__

#include "mWindow.h"
#include "mButton.h"
#include "mText.h"
#include <unistd.h>
#include <pthread.h> 

//#define MAX_BKG 4


class ybh_wScreenSave : public mWindow {
public:
    ybh_wScreenSave();
    virtual ~ybh_wScreenSave();

    void doEvent(mEvent *e);
    void scr_process(void);     //创建线程
    void not_scr_process(void); //结束线程

    void ybh_screen(void);

    volatile bool exit = false;
    

    
    //void doTimer(void);

    //void screen(void);
    

private:
    pthread_t pid;     

    mText m_text;
    mText m_date;
  
    mButton m_exit;

    uint32_t m_ts_st;
	uint32_t m_ts_st_h;
};

extern mWindow *SysMain;

#endif