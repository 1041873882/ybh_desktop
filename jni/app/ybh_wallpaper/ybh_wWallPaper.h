#ifndef __YBH_W_WALLPAPER_H__
#define __YBH_W_WALLPAPER_H__

#include "mWindow.h"
#include "mEdit.h"
#include "mButton.h"
#include "mCombox.h"
#include <string.h>

#define MAX_WALLPAPER_LINE 4

class ybh_wWallPaper : public mWindow {
public:
    ybh_wWallPaper();
    virtual ~ybh_wWallPaper();

    void showData();
    void save();

    void doEvent(mEvent *e);
    static int y_b;


private:
    mPixmap m_bkg;
    mPixmap m_pre_bkg[MAX_WALLPAPER_LINE];  //预览壁纸

    mText m_wall_text;
    mText m_data[MAX_WALLPAPER_LINE];

    mButton m_wall[MAX_WALLPAPER_LINE];
    mButton m_exit;

    int m_offset;
    int m_max;

};


#endif