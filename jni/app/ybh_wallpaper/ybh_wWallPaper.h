#ifndef __YBH_W_WALLPAPER_H__
#define __YBH_W_WALLPAPER_H__

#include "mWindow.h"
#include "mEdit.h"
#include "mButton.h"
#include "mCombox.h"
#include <string.h>

#define MAX_WALLPAPER_LINE 4
static const char *y_main_wall[] = {"bkg0", "bkg1", "bkg2", "bkg3"};

class ybh_wWallPaper : public mWindow {
public:
    ybh_wWallPaper();
    virtual ~ybh_wWallPaper();

    void showData(int n);

    void doEvent(mEvent *e);
    static int y_b;


private:
    mPixmap m_bkg;
    // mPixmap m_wall_bkg;
    mPixmap m_pre_bkg[MAX_WALLPAPER_LINE];  //预览壁纸

    mText m_wall_text;
    mText m_data[MAX_WALLPAPER_LINE];
    //mText m_screen_text;

    mButton m_wall[MAX_WALLPAPER_LINE];
    mButton m_exit;
    //mEdit m_screen;

    int m_offset;
    int m_first;
    int m_max;
    int m_select;
    int m_start;

};


#endif