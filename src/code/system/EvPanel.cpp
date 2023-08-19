
#include    <EvGUI.h>

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
 * @param[out] *Dest   The address pointer of the EvPanel destination. Cannot be NULL.
 * @param[in]  Tag     The tag name of the Panel. If NULL, the default tag name is "EvPanel".
 * @param[in]  State   The initial state of the Panel. Default is set to VISIBLE_OBJ.
 *
 * @return     EvPanel address pointer on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPanel     *EvPanel::Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag, uint16_t State)
{
  return !Dest ? NULL : (EvPanel *)EvObj::TryCreate(new EvPanel(Left, Top, Width, Height, Dest->Disp, !Tag ? "EvPanel" : Tag, State), Dest);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPanel::EvPanel(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag, uint16_t State) : EvObj(Left, Top, Width, Height, Disp, Tag, State)
{
  BgColor(TRANSPARENT);
  mFirst = NULL;
  mLast = NULL;
  mOffsetX = 0;
  mOffsetY = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvPanel::~EvPanel(void)
{
  EvObj     *obj;
  Node      *node, *next;

  for (node = mFirst; node != NULL; node = next)
  {
    obj = node->obj;
    next = node->next;

    if (obj != NULL)
      delete obj;

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
 * @param[in]  *Tag     The tag name of the object to add. Default is NULL.
 * 
 * @return     The address pointer of the object on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::AddObj(EvObj *Obj, const char *Tag)
{
  Node      *node;

  for (node = mFirst; node != NULL; node = node->next)
    if (Obj == node->obj)
      return NULL;

  if (Obj == NULL || (node = new Node) == NULL)
    return NULL;

  node->obj = Obj;
  node->next = NULL;
  mLast = (mFirst == NULL) ? mFirst = node : mLast->next = node;
  Obj->SetOwner(this);

  if (Tag != NULL)
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
 * @return     The address pointer of the object on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::RemoveObj(EvObj *Obj)
{
  Node      *node, *prev = NULL;

  for (node = mFirst; node != NULL; prev = node, node = node->next)
    if (Obj == node->obj)
    {
      if (node == mFirst)
        mFirst = (node == mLast) ? mLast = NULL : node->next; 
      else if (node != mLast)
        prev->next = node->next;
      else
      {
        prev->next = NULL;
        mLast = prev;
      }

      delete node;
      Modified();
      return Obj;
    }

  return NULL;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Get the first visual object from the panel.
 *
 * @return     The address pointer of the first object on success, otherwise returns NULL.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::FirstObj(void)
{
  return (mFirst != NULL) ? mFirst->obj : NULL;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief      Clear the display list cache of the Panel and all of its objects.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::ClearCache(void)
{
  EvObj::ClearCache();

  for (Node *node = mFirst; node != NULL; node = node->next)
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

  for (Node *node = mFirst; node != NULL; node = node->next)
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

  for (Node *node = mFirst; node != NULL; node = node->next)
    node->obj->ModifiedAll();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::SetToFront(EvObj *Obj, bool AllOwner)
{
  Node      *node, *prev = NULL;

  if (mLast != NULL && mLast->obj == Obj)
  {
    if (AllOwner)
      ToFront();
  }
  else
  {
    for (node = mFirst; node != NULL; prev = node, node = node->next)
      if (Obj == node->obj)
      {
        if (node->next != NULL)
        {
          if (prev == NULL)
            mFirst = node->next;
          else
            prev->next = node->next;

          node->next = NULL;
          mLast->next = node;
          mLast = node;
        }

        if (AllOwner)
          ToFront();
      }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvObj       *EvPanel::Touching(EvTouchEvent *Touch)
{
  EvObj     *obj, *touchObj;

  if (!IsVisible())
    return NULL;

  touchObj = EvObj::Touching(Touch);

  for (Node *node = mFirst; node != NULL; node = node->next)
    if ((obj = node->obj->Touching(Touch)) != NULL)
      touchObj = obj;

  return touchObj;
}

// EvObj       *EvPanel::Touching(int16_t X, int16_t Y)
// {
//   EvObj     *obj, *touchObj;

//   if (!IsVisible())
//     return NULL;

//   touchObj = EvObj::Touching(X, Y);

//   for (Node *node = mFirst; node != NULL; node = node->next)
//     if ((obj = node->obj->Touching(X, Y)) != NULL)
//       touchObj = obj;

//   return touchObj;
// }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvPanel::MustBeDraw(void)
{
  bool      retValue = ((mStatus & VISIBLE_OBJ) != 0 && mView.w > 0);

  for (Node *node = mFirst; node != NULL && retValue != true; node = node->next)
    if (node->obj->MustBeDraw())
      retValue = true;

  return retValue;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::SetDisp(EvDisplay *Disp)
{
  EvObj::SetDisp(Disp);

  for (Node *node = mFirst; node != NULL; node = node->next)
    node->obj->SetDisp(Disp);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::SetView(void)
{
  EvObj::SetView();

  for (Node *node = mFirst; node != NULL; node = node->next)
    node->obj->SetView();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::Refresh(void)
{
  EvObj::Refresh();

  for (Node *node = mFirst; node != NULL; node = node->next)
    node->obj->Refresh();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvPanel::Draw(void)
{
  if (mStatus & VISIBLE_OBJ)
  {
    EvObj::Draw();

    for (Node *node = mFirst; node != NULL; node = node->next)
      node->obj->Draw();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvPanel::OffsetX(void)
{
  return mOffsetX;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int16_t     EvPanel::OffsetY(void)
{
  return mOffsetY;
}
