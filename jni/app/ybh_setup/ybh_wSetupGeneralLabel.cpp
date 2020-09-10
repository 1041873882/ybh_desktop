#include "sys.h"
#include "ybh_wSetupGeneralLabel.h"
#include "SysSound.h"
#include "mFB.h"
#include <stdio.h>

ybh_wSetupGeneralLabel::ybh_wSetupGeneralLabel() : mWindow("ybh_setup/general")
{
    m_touchtone_st = -1;
    m_answer_st = -1;

    m_bright_text.setParent(this);
    m_bright_text.load(m_style, "bright_text");

    m_bright.setParent(this);
    m_bright.load(m_style, "bright");
    m_bright.setRange(100, 255);
    m_bright.setValue(sys.lcd.bright());
    sys.lcd.bright(m_bright.value());
    fb.bright(sys.lcd.bright());
    sys.save();

    m_sound_text.setParent(this);
    m_sound_text.load(m_style, "sound_text");
    m_sound.setParent(this);
    m_sound.load(m_style, "sound");
    m_sound.setRange(0, 5);
    m_sound.setValue(5-sys.volume.music());

    m_talk_text.setParent(this);
    m_talk_text.load(m_style, "talk_text");
    m_talk.setParent(this);
    m_talk.load(m_style, "talk");
    m_talk.setRange(0, 5);
    m_talk.setValue(5-sys.volume.talk());

    m_ringtong_text.setParent(this);
    m_ringtong_text.load(m_style, "ringtone_text");
    m_ringtong.setParent(this);
    m_ringtong.load(m_style, "ringtone");
    m_ringtong.insert(m_style.getText("style/text/ring0"));
    m_ringtong.insert(m_style.getText("style/text/ring1"));
    m_ringtong.insert(m_style.getText("style/text/ring2"));
    m_ringtong.insert(m_style.getText("style/text/ring3"));

    m_answer_text.setParent(this);
    m_answer_text.load(m_style, "answer_text");
    m_answer.setParent(this);
    //m_answer.load(m_style, "answer");
    //sys.talk

    m_touchtone_text.setParent(this);
    m_touchtone_text.load(m_style, "touchtone_text");
    m_touchtone.setParent(this);
    // m_touchtone.load(m_style, "touchtone1");
    //m_touchtone.load(m_style, "touchtone");

    m_time_text.setParent(this);
    m_time_text.load(m_style, "time_text");
    m_time.setParent(this);
    m_time.load(m_style, "time");
    //m_time.setMode(mEdit::TZ);
    //m_time.setText(sys.user.tz());

    m_date.setParent(this);
    m_date.load(m_style, "date");


    m_reset.setParent(this);
    m_reset.load(m_style, "reset");

    this->loadToolbal();
    //this->save();
}

ybh_wSetupGeneralLabel::~ybh_wSetupGeneralLabel()
{
}

int ybh_wSetupGeneralLabel::m_press=0;

void ybh_wSetupGeneralLabel::doEvent(mEvent *e)
{
    mWindow::doEvent(e);
    mEvent *event;
#if 1
    if (e->type == mEvent::TouchEnd) {
        if (e->wParam == m_answer.id()) {
            if (sys.talk.answer()) {
                sys.talk.m_answer = 0.;
                //m_answer.load(m_style, "answer1");
            } else {
                sys.talk.m_answer = 1;
                //m_answer.load(m_style, "answer");
            }
        } else if (e->wParam == m_touchtone.id()) {
            press_ring = !press_ring;
            if (press_ring) {
                event->y_m_press = !event->y_m_press;
                event->doTouch();
            } else {
                event->y_m_press = !event->y_m_press;
                event->doTouch();
            }
        }
    }
#endif

#if 0
    if (e->type == mEvent::TouchEnd) {
        if (e->wParam == m_answer.id()) {
            m_answer_st = sys.talk.answer();
            m_answer_st = m_answer_st ? 0 : 1;
            if (m_answer_st)
                m_answer.load(m_style, "answer1");
            else 
                m_answer.load(m_style, "answer");
        }
    }
#endif

}

#if 1
void ybh_wSetupGeneralLabel::loadToolbal(void)
{
    mEvent *event;
    if (m_answer_st != sys.talk.answer()) {
        m_answer_st = sys.talk.answer();
        if(m_answer_st) {
            m_answer.load(m_style, "answer1");
        } else {
            m_answer.load(m_style, "answer");
        }
    }
    if(event->y_m_press) {
        m_touchtone.load(m_style, "touchtone1");
    } else {
        m_touchtone.load(m_style, "touchtone");
    }
}
#endif

void ybh_wSetupGeneralLabel::doTimer(void)
{
    mWindow::doTimer();
    this->loadToolbal();
}

 





 