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

#ifndef     _EV_PANEL_H_
#define     _EV_PANEL_H_

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvPanel class allows you to group together several visual objects.
 * 
 * The EvPanel class is a child class of EvObj that allows you to group multiple
 * visual objects into a single Panel object.
 * 
 * The user can add the number of visual objects he wants in a Panel. The order of the 
 * displayed objects will be that of the added objects. You can change this order with
 * the ToFront() member function.
 * 
 * Since a Panel is a visual object itself, so you can add a Panel in another Panel.
 * 
 * The Left and the Top positions of objects added to a Panel become relative to
 * the upper left corner of the Panel.
 * 
 * @note      All objects added to a panel become the property of this panel.
 * This means that the owning Panel must take care of deleting all its own objects before
 * its own deletion. So, the user does not have to worry about deleting all the objects
 * he adds to a Panel. Removing a panel will remove all of these objects.
 * 
 * @warning   However, pointers that store the address of objects that have been
 * deleted by an owner Panel will not be set to nullptr. They should therefore never
 * be reused after deleting a Panel.
 * 
 * @par Example
 * ~~~
 * EvPanel *myPanel = EvPanel::Create(50, 50, 200, 200, myDisplay, "MyPanel");
 * EvLabel *myLabel = EvLabel::Create(10, 10, 150, 50, myPanel, "Hello World!");
 * myPanel->TextAlign(CENTER);
 * myPanel->TextFont(29);
 * ~~~
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvPanel : public EvObj
{
  friend class EvShell;

  struct Node
  {
    EvObj         *obj;
    Node          *next;
  };

  protected:
    EvPanel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

  public:
    virtual       ~EvPanel(void);

    EvObj         *AddObj(EvObj *Obj, const char *Tag = nullptr);
    EvObj         *RemoveObj(EvObj *Obj);
    virtual void  Hide(void);
    virtual void  ClearCache(void);
    virtual void  SetOpacity(uint16_t Opacity);
    virtual void  ModifiedAll(void);
    virtual bool  IsPanelObj(void) { return true; };

  public:         // Public functions reserved for the system
    void          SetToFront(EvObj *Obj, bool AllOwner = true);
    virtual EvObj *Touching(const EvTouchEvent *Touch);
    virtual void  SetDisplay(EvDisplay *Disp);
    virtual void  SetView(void);
    virtual void  Refresh(void);
    virtual void  Draw(void);
    int16_t       OffsetX();
    int16_t       OffsetY();

  protected:
    int16_t       mPanelOffsetX;
    int16_t       mPanelOffsetY;
    Node          *mFirst;
    Node          *mLast;

  public:
    static const  char *const TypeName;
    virtual const char *TypeObj(void) { return TypeName; };

    static EvPanel  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PANEL_H_ */
