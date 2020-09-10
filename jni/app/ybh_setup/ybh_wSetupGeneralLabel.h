#ifndef __YBH_W_SETUP_GENERAL_LABEL_H__
#define __YBH_W_SETUP_GENERAL_LABEL_H__

#include "mWindow.h"
#include "mCombox.h"
#include "mSeekBar.h"
#include "mEdit.h"
#include "mButton.h"

class ybh_wSetupGeneralLabel : public mWindow {
public:
    ybh_wSetupGeneralLabel();
    virtual ~ybh_wSetupGeneralLabel();

    volatile bool press_ring = false;

    void doEvent(mEvent *e);
    //void save(void);
    void doTimer(void);

    void loadToolbal(void);

    static int m_press;

private:
    mText m_bright_text;
    mSeekBar m_bright;

    mText m_sound_text;
    mSeekBar m_sound;

    mText m_talk_text;
    mSeekBar m_talk;

    mText m_ringtong_text;
    mCombox m_ringtong;

    mText m_time_text;
    mEdit m_time;
    mEdit m_date;

    mText m_answer_text;
    mText m_touchtone_text;
   
    mButton m_ok;
    mButton m_answer;
    mButton m_reset;
    mButton m_touchtone;

    int m_answer_st;
    int m_touchtone_st;

};


#endif