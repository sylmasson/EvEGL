
#ifndef     _EV_EDITOR_H_
#define     _EV_EDITOR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class       EvEditor;
class       EvEditDot;
class       EvEditProp;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvEditor : public EvPanel
{
  protected:
    EvEditor(EvDisplay *Disp, const char *Tag);

  public:
    virtual       ~EvEditor(void);

    EvEditDot     *DotMove;
    EvEditDot     *DotReSize;

  protected:
    virtual void  drawEvent(void);
    virtual void  refreshEvent(void);

  public:
    static bool         Close(void);
    static bool         Open(EvDisplay *Disp);
    static bool         IsAlreadyOpen(void);
    static void         SelectObj(EvObj *Obj);
    static void         DestroyedObj(EvObj *Obj);
    static void         AlwaysToFront(void);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvEditDot : public EvObj
{
  friend class EvEditor;

  protected:
    EvEditDot(uint16_t Size, void (*OnTouch)(EvEditDot *Sender, EvTouchEvent *Touch), EvDisplay *Disp, const char *Tag);

  public:
    int16_t       X, Y;

  protected:
    virtual void  drawEvent(void);
    virtual void  touchEvent(EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvEditDot *Sender, EvTouchEvent *Touch);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvEditProp : public EvPanel
{
  friend class EvEditor;

  protected:
    EvEditProp(EvDisplay *Disp, const char *Tag);

  public:
    virtual       ~EvEditProp(void);

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

  private:
    static int16_t    sLeft;
    static int16_t    sTop;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_EDITOR_H_ */
