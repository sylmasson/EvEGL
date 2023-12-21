
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
 * EvLabel *myLabel = EvLabel::Create(10, 10, 150, 50, myPanel, "MyLabel");
 * myLabel->TextLabel("Hello World!");
 * myPanel->TextAlign(CENTER);
 * myPanel->TextFont(29);
 * ~~~
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvPanel : public EvObj
{
  struct Node
  {
    EvObj         *obj;
    Node          *next;
  };

  public:
    ~EvPanel(void);

    EvObj         *AddObj(EvObj *Obj, const char *Tag = nullptr);
    EvObj         *RemoveObj(EvObj *Obj);
    EvObj         *FirstObj(void);
    virtual void  ClearCache(void);
    virtual void  SetOpacity(uint16_t Opacity);
    virtual void  ModifiedAll(void);

  public:         // Public functions reserved for the system
    void          SetToFront(EvObj *Obj, bool AllOwner = true);
    virtual EvObj *Touching(EvTouchEvent *Touch);
//    virtual bool  MustBeDraw(void);
    virtual void  SetDisplay(EvDisplay *Disp);
    virtual void  SetView(void);
    virtual void  Refresh(void);
    virtual void  Preload(void);
    virtual void  Draw(void);
    int16_t       OffsetX();
    int16_t       OffsetY();

  protected:
    EvPanel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);

    int16_t       mOffsetX;
    int16_t       mOffsetY;
    Node          *mFirst;
    Node          *mLast;

  public:
    static EvPanel  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_PANEL_H_ */
