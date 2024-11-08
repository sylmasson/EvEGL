
#include    <EvGUI.h>

//#define     VERBOSE

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Create a new instance of a standard Panel.
 * 
 * A new Panel is created at the specified size and relative position
 * of its owner Dest.
 * 
 * @param[in]  Left    The left position of the Panel.
 * @param[in]  Top     The top position of the Panel.
 * @param[in]  Width   The width of the Panel.
 * @param[in]  Height  The height of the Panel.
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be nullptr.
 * @param[in]  Tag     The tag name of the Panel. If nullptr, the default tag name is "EvPanel".
 * @param[in]  State   The initial state of the Panel. Default is set to VISIBLE_OBJ.
 *
 * @return     EvPanel address pointer on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPanel     *EvPanel::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? nullptr : (EvPanel *)EvObj::TryCreate(new EvPanel(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvPanel" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPanel::EvPanel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) :
  EvObj(Left, Top, Width, Height, Disp, Tag, State),
  mPanelOffsetX(0),
  mPanelOffsetY(0),
  mFirst(nullptr),
  mLast(nullptr)
{
  BgColor(CL_PANEL_BG);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPanel::~EvPanel(void)
{
  EvObj     *obj;
  Node      *node, *next;

  #ifdef VERBOSE
    char    str[32];

    snprintf(str, sizeof(str) - 1, "~EvPanel: %p ", this);
    EvOut->print(str);
    DisplayTagList(EvOut);
    EvOut->println();
  #endif

  for (node = mFirst; node != nullptr; node = next)
  {
    obj = node->obj;
    next = node->next;

    if (obj != nullptr)
    {
      obj->mOwner = nullptr;
      delete obj;
    }

    delete node;
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Add a visual object to the panel.
 * 
 * The user can add himself objects in a Panel with this function. However, it is
 * recommended to use the Create() function specific to all visual objects. The Create()
 * function will use this AddObj() function to add objects to a destination Panel.
 *
 * @param[in]  *Obj     The address pointer of the object to add.
 * @param[in]  *Tag     The tag name of the object to add. Default is nullptr.
 * 
 * @return     The address pointer of the object on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::AddObj(EvObj *Obj, const char *Tag)
{
  Node      *node;

  for (node = mFirst; node != nullptr; node = node->next)
    if (Obj == node->obj)
      return nullptr;

  if (Obj == nullptr || (node = new Node) == nullptr)
    return nullptr;

  node->obj = Obj;
  node->next = nullptr;
  mLast = (mFirst == nullptr) ? mFirst = node : mLast->next = node;
  Obj->SetOwner(this);

  if (Tag != nullptr)
    Obj->Tag = Tag;

  Modified();
  return Obj;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Remove a visual object from the panel.
 * 
 * The user can remove a visual objects in a Panel at any time. This means that the object
 * is removed from the Panel object list. The object itself is not deleted by this function.
 * It is up to the user to take care of the deletion of it.
 *
 * @param[in]  *Obj     The address pointer of the object to be removed.
 * 
 * @return     The address pointer of the object on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::RemoveObj(EvObj *Obj)
{
  Node      *node, *prev = nullptr;

  for (node = mFirst; node != nullptr; prev = node, node = node->next)
    if (Obj == node->obj)
    {
      if (node == mFirst)
        mFirst = (node == mLast) ? mLast = nullptr : node->next; 
      else if (node != mLast)
        prev->next = node->next;
      else
      {
        prev->next = nullptr;
        mLast = prev;
      }

      delete node;
      Modified();
      return Obj;
    }

  return nullptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the first visual object from the panel.
 *
 * @return     The address pointer of the first object on success, otherwise returns nullptr.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::FirstObj(void)
{
  return (mFirst != nullptr) ? mFirst->obj : nullptr;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the Object as not visible.
 * 
 * If the Object is visible, change it to not visible Object.
 * 
 * By default, a new Object is set as visible.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::Hide(void)
{
  if (mStatus & VISIBLE_OBJ)
  {
    EvObj::Hide();
    ClearCache();
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Clear the display list cache of the Panel and all of its objects.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::ClearCache(void)
{
  EvObj::ClearCache();

  for (Node *node = mFirst; node != nullptr; node = node->next)
    node->obj->ClearCache();
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets the opacity of the Panel and all of its objects.
 * 
 * @param[in]  Opacity   The opacity of the all object. Can be from 0 to 256.
 * 
 * @li  0 for full transparency.
 * @li  256 for a full opacity.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::SetOpacity(uint16_t Opacity)
{
  EvObj::SetOpacity(Opacity);

  for (Node *node = mFirst; node != nullptr; node = node->next)
    node->obj->SetOpacity(Opacity);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Sets all Objects of the Panel as modified.
 * 
 * Notify that all Objects of the Panel are modified for the next drawEvent.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::ModifiedAll(void)
{
  EvObj::ModifiedAll();

  for (Node *node = mFirst; node != nullptr; node = node->next)
    node->obj->ModifiedAll();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::SetToFront(EvObj *Obj, bool AllOwner)
{
  Node      *node, *prev = nullptr;

  if (mLast != nullptr && mLast->obj == Obj)
  {
    if (AllOwner)
      ToFront();
  }
  else
  {
    for (node = mFirst; node != nullptr; prev = node, node = node->next)
      if (Obj == node->obj)
      {
        if (node->next != nullptr)
        {
          if (prev == nullptr)
            mFirst = node->next;
          else
            prev->next = node->next;

          node->next = nullptr;
          mLast->next = node;
          mLast = node;
        }

        if (AllOwner)
          ToFront();
      }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::Touching(const EvTouchEvent *Touch)
{
  EvObj     *obj, *touchObj;

  if (!IsVisible())
    return nullptr;

  touchObj = EvObj::Touching(Touch);

  for (Node *node = mFirst; node != nullptr; node = node->next)
    if ((obj = node->obj->Touching(Touch)) != nullptr)
      touchObj = obj;

  return touchObj;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::SetDisplay(EvDisplay *Disp)
{
  EvObj::SetDisplay(Disp);

  for (Node *node = mFirst; node != nullptr; node = node->next)
    node->obj->SetDisplay(Disp);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::SetView(void)
{
  EvObj::SetView();

  for (Node *node = mFirst; node != nullptr; node = node->next)
    node->obj->SetView();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::Refresh(void)
{
  EvObj::Refresh();

  for (Node *node = mFirst; node != nullptr; node = node->next)
    node->obj->Refresh();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::Draw(void)
{
  if (mStatus & VISIBLE_OBJ)
  {
    EvObj::Draw();

    for (Node *node = mFirst; node != nullptr; node = node->next)
      node->obj->Draw();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvPanel::OffsetX(void)
{
  return mPanelOffsetX;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvPanel::OffsetY(void)
{
  return mPanelOffsetY;
}
