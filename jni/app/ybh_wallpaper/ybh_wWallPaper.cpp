#include "sys.h"
#include "SysSound.h"
#include "mFB.h"
#include "wMain.h"
#include "ybh_wWallPaper.h"

ybh_wWallPaper::ybh_wWallPaper() : mWindow("ybh_wallpaper")
{
    //char s2[12];
    //strcpy(s2, "bkg0");
    //bkg_s = s2;
    //this->bkg_s;
    char s1[128];
    m_bkg.setParent(this);
    m_bkg.load(m_style, "bkg");

    // m_wall_bkg.setParent(this);
    // m_wall_bkg.load(m_style, "wall_bkg");

    for (int i = 0; i < MAX_WALLPAPER_LINE; i++) {
        sprintf(s1, "pre_bkg%d", i);
        m_pre_bkg[i].setParent(this);
        m_pre_bkg[i].load(m_style, s1);
        m_pre_bkg[i].hide();
    }

    m_wall_text.setParent(this);
    m_wall_text.load(m_style, "wall_text");

    m_exit.setParent(this);
    m_exit.load(m_style, "exit");


    m_offset = m_style.getInt("/style/offset", 112); //每个按钮控件上下间隔112像素
    fprintf(stderr, "m_offset = %d", m_offset);
    for (int i = 0; i < MAX_WALLPAPER_LINE; i++) {
        m_wall[i].setParent(this);
        m_wall[i].load(m_style, "wall");
        m_wall[i].move(m_wall[0].x(), m_wall[0].y()+i*m_offset);

        m_data[i].setParent(this);
        m_data[i].load(m_style, "data");
        m_data[i].move(m_data[0].x(), m_data[0].y()+i*m_offset);
    }

    m_first = 0;
    m_start = 0;
    this->showData(m_first);
    
}

ybh_wWallPaper::~ybh_wWallPaper()
{
}

void ybh_wWallPaper::showData(int n)
{
    m_max = MAX_WALLPAPER_LINE;

    for(int i = 0; i < m_max; i++) {
        m_data[i].setColor(0xFFFFFF);
        char s[128];
        
        sprintf(s, "bkg %02d", i+1);
        m_data[i].setText(s);
        m_data[i].show();
    }
}

int ybh_wWallPaper::y_b=0;  //初始化静态变量

void ybh_wWallPaper::doEvent(mEvent *e)
{
    char s2[128];
    mWindow::doEvent(e); 
    if(e->type == mEvent::TouchEnd) {
        if (!m_start) {
            int n = -1;
#if 1
            for (int i = 0; i < MAX_WALLPAPER_LINE; i++) {
                if (e->wParam == m_wall[i].id() && i < m_max) {                    
                    n = i;
                    y_b = i;
                    break;
                }
            }
#endif
            if (n != -1) {
                for(int i = 0; i < m_max; i++) {
                    m_data[i].setColor(0xFFFFFF);
                    m_pre_bkg[i].hide();
                }
                m_select = n;
                m_data[n].setColor(0x0000FF);
                m_pre_bkg[n].show();
            }
        }    

        if (e->wParam == m_exit.id()) {
            wMain *w = new wMain();
            w->show();
        } 
    }
}


