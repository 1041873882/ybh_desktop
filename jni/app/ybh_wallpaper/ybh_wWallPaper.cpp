#include "sys.h"
#include "SysSound.h"
#include "mFB.h"
#include "wMain.h"
#include "ybh_wWallPaper.h"

int ybh_wWallPaper::y_b=0;  //初始化静态变量

ybh_wWallPaper::ybh_wWallPaper() : mWindow("ybh_wallpaper")
{
    m_max = MAX_WALLPAPER_LINE;
    char s[128];

    y_b = sys.user.bkg();
    m_bkg.setParent(this);
    m_bkg.load(m_style, "bkg");

    for (int i = 0; i < m_max; i++) {
        sprintf(s, "pre_bkg%d", i);
        m_pre_bkg[i].setParent(this);
        m_pre_bkg[i].load(m_style, s);
        m_pre_bkg[i].hide();
    }
    m_pre_bkg[y_b].show();

    m_wall_text.setParent(this);
    m_wall_text.load(m_style, "wall_text");

    m_exit.setParent(this);
    m_exit.load(m_style, "exit");


    m_offset = m_style.getInt("/style/offset", 112); //每个按钮控件上下间隔112像素
    for (int i = 0; i < m_max; i++) {
        m_wall[i].setParent(this);
        m_wall[i].load(m_style, "wall");
        m_wall[i].move(m_wall[0].x(), m_wall[0].y()+i*m_offset);

        m_data[i].setParent(this);
        m_data[i].load(m_style, "data");
        m_data[i].move(m_data[0].x(), m_data[0].y()+i*m_offset);
        
        
        sprintf(s, "bkg %02d", i+1);
        m_data[i].setText(s);
        if (y_b == i)
            m_data[i].setColor(0x0000FF);
       else 
            m_data[i].setColor(0xFFFFFF); 
       m_data[i].show();
    }    
}

ybh_wWallPaper::~ybh_wWallPaper()
{
}




void ybh_wWallPaper::doEvent(mEvent *e)
{
    char s2[128];
    int n = -1;
    mWindow::doEvent(e); 
    if(e->type == mEvent::TouchEnd) {
        for (int i = 0; i < m_max; i++) {
                if (e->wParam == m_wall[i].id()) {  
                    m_pre_bkg[i].show();    
                    n = i;             
                    y_b = i;
                    break;
                }
            }
        if (e->wParam == m_exit.id()) {
            this->save();
            wMain *w = new wMain();
            w->show();
        } 
    }

    if (n != -1) {      //防止重复选中
        for(int i = 0; i < m_max; i++) {
            m_data[i].setColor(0xFFFFFF);
            m_pre_bkg[i].hide();
        }
        m_data[n].setColor(0x0000FF);
        m_pre_bkg[n].show();
    }
}

void ybh_wWallPaper::save(void)
{
    sys.user.bkg(y_b);
    sys.save();
}


