/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  The MIT License (MIT)

  Copyright (c) 2024 Sylvain Masson

  Permission is hereby granted, free of charge, to any person obtaining a copy of 
  this software and associated documentation files (the “Software”), to deal in 
  the Software without restriction, including without limitation the rights to 
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
  of the Software, and to permit persons to whom the Software is furnished to 
  do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
  OTHER DEALINGS IN THE SOFTWARE.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

  protected:
    virtual void  drawEvent(void);
    virtual void  refreshEvent(void);

  public:
    EvEditDot     *DotMove;
    EvEditDot     *DotReSize;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static bool   Close(void);
    static bool   Open(EvDisplay *Disp);
    static bool   IsAlreadyOpen(void);
    static void   SelectObj(EvObj *Obj);
    static void   DestroyedObj(EvObj *Obj);
    static void   AlwaysToFront(void);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvEditDot : public EvObj
{
  friend class EvEditor;

  protected:
    EvEditDot(uint16_t Size, void (*OnTouch)(EvEditDot *Sender, const EvTouchEvent *Touch), EvDisplay *Disp, const char *Tag);

  protected:
    virtual void  drawEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    void          (*mOnTouch)(EvEditDot *Sender, const EvTouchEvent *Touch);

  public:
    int16_t       X, Y;

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvEditProp : public EvPanel
{
  friend class EvEditor;

  protected:
    EvEditProp(EvDisplay *Disp, const char *Tag);

  public:
    virtual       ~EvEditProp(void);

  protected:
    virtual void  refreshEvent(void);
    virtual void  touchEvent(const EvTouchEvent *Touch);

  private:
    static int16_t    sLeft;
    static int16_t    sTop;

  public:
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

    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_EDITOR_H_ */
