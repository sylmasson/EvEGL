
#ifndef     _EV_EDITOR_H_
#define     _EV_EDITOR_H_

#define     BG_COLOR        RGB555(245, 245, 245)
#define     PANEL_COLOR     RGB555(210, 210, 210)
#define     NUM_COLOR       RGB555(255, 255, 255)
#define     DOT_COLOR       RGB555(255,   0,   0)
#define     TEXT_COLOR      RGB555(  0,   0,   0)
#define     TEXT_ERROR      RGB555(255,   0,   0)
#define     TEXT_SELECT     RGB555(255, 255, 255)

#define     COLOR_SELNONE   RGB555(255, 255, 255)
#define     COLOR_SELECT    RGB555(  0,   0, 160)
#define     COLOR_SELNEW    RGB555(160, 160, 210)
#define     COLOR_ICONS     RGB555(140, 140, 140)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDot : public EvObj
{
  public:
    EvDot(uint16_t Size, void (*OnTouch)(EvDot *Sender, EvTouchEvent *Touch), EvDisplay *Disp, const char *Tag);

    int16_t       X, Y;

  protected:
    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvDot *Sender, EvTouchEvent *Touch);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvEditor : public EvPanel
{
  public:
    EvEditor(EvDisplay *Disp, const char *Tag);
    ~EvEditor(void);

    EvDot         *DotMove;
    EvDot         *DotReSize;

  protected:
    virtual void  drawEvent(void);
    virtual void  refreshEvent(void);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvProperty : public EvPanel
{
  public:
    EvProperty(EvDisplay *Disp, const char *Tag);
    ~EvProperty(void);

    bool          Minimized;

    EvLabel       *LabTitle;
    EvButton      *BtnMinimize;
    EvToggle      *TglEdit;
    EvTab         *TabPanel;
    EvPanel       *PanObj, *PanText;
    EvLabel       *LabLeft, *LabTop;
    EvLabel       *LabWidth, *LabHeight;
    EvLabel       *LabPadX, *LabPadY;
    EvLabel       *LabFont, *LabTag;
    EvNumInt      *NumLeft, *NumTop;
    EvNumInt      *NumWidth, *NumHeight;
    EvNumInt      *NumFont, *NumPadX, *NumPadY;
    EvSelector    *SelShape, *SelAlignX, *SelAlignY;

  protected:
    virtual void  refreshEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_EDITOR_H_ */
