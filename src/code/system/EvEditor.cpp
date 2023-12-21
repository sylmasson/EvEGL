
#include    <EvGUI.h>
#include    <include/system/EvEditor.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const uint8_t  sShapeIcons_data[243] =
{
  0x78, 0x01, 0xED, 0x96, 0x5B, 0x11, 0xC2, 0x30, 0x10, 0x45, 0xB7, 0xCD, 0xF6, 0x11, 0xFA, 0x92,
  0x80, 0x04, 0x24, 0x20, 0x01, 0x09, 0x48, 0x40, 0x02, 0x12, 0x90, 0x80, 0x04, 0x24, 0x20, 0x01,
  0x09, 0x48, 0x40, 0xC2, 0x05, 0x06, 0xDA, 0x26, 0xE9, 0x4E, 0xE9, 0x47, 0x87, 0x30, 0x43, 0xCE,
  0xEF, 0x49, 0x73, 0xDB, 0x64, 0xBB, 0x09, 0x1A, 0x6C, 0x71, 0xC2, 0x0D, 0xC0, 0x15, 0x47, 0xAC,
  0x71, 0xC6, 0x05, 0x7B, 0x1C, 0xB0, 0xC4, 0x0A, 0x1B, 0xEC, 0x28, 0x10, 0x08, 0xFC, 0x04, 0x51,
  0xAC, 0x98, 0x93, 0x34, 0x23, 0xC9, 0xE5, 0x0F, 0xD7, 0xA2, 0x17, 0x64, 0x53, 0x18, 0xF2, 0x4D,
  0x5C, 0x76, 0x36, 0x67, 0x91, 0x82, 0x2C, 0xB2, 0xA4, 0x15, 0x79, 0x49, 0x0E, 0x95, 0x66, 0x1B,
  0x55, 0x9B, 0x5A, 0x0C, 0x50, 0xDD, 0x2C, 0x9A, 0x45, 0xEC, 0xFC, 0xC8, 0x30, 0xAA, 0xB2, 0x14,
  0x55, 0xC3, 0xCF, 0x63, 0xE3, 0x05, 0x6A, 0x16, 0xE9, 0xE6, 0x9F, 0x92, 0x9F, 0x59, 0x2A, 0xB1,
  0x57, 0x40, 0x9C, 0xA0, 0x1F, 0xB2, 0x60, 0x91, 0x7C, 0xF4, 0x71, 0x27, 0x3F, 0xB1, 0x5D, 0x6A,
  0xBA, 0x88, 0x25, 0xD2, 0x4F, 0xF9, 0x9A, 0xA6, 0xE7, 0x37, 0xEC, 0x60, 0xEE, 0x80, 0x5C, 0x3F,
  0x8A, 0x66, 0xCC, 0x1F, 0x0C, 0x31, 0x2B, 0x5C, 0xD8, 0xFD, 0x27, 0xD5, 0x8C, 0xF9, 0xEC, 0x12,
  0x8F, 0xC9, 0x17, 0xD9, 0x7C, 0xF9, 0x25, 0xBB, 0xE8, 0xBF, 0xCA, 0xF7, 0xBD, 0xFE, 0xDE, 0xEB,
  0xCF, 0xF7, 0xFF, 0xE7, 0xBB, 0xFF, 0x38, 0x3D, 0x7C, 0x4A, 0xFF, 0xED, 0x57, 0x68, 0x8E, 0xFE,
  0xEB, 0xFB, 0xFC, 0xF1, 0x7E, 0xFE, 0x92, 0xEF, 0xFB, 0x47, 0x20, 0x10, 0xF8, 0x36, 0x77, 0x31,
  0x73, 0x33, 0x1F
};

static const uint8_t  sAlignIconsX_data[130] =
{
  0x78, 0x01, 0xED, 0xD4, 0xBD, 0x0D, 0x85, 0x30, 0x0C, 0x04, 0x60, 0xE7, 0x07, 0x1E, 0x8F, 0x86,
  0x11, 0x18, 0x81, 0x11, 0x18, 0x81, 0x11, 0x18, 0x81, 0x11, 0x18, 0x81, 0xD1, 0x18, 0xE9, 0xA8,
  0x12, 0x29, 0xE8, 0x24, 0xE8, 0x52, 0x70, 0x5F, 0x19, 0x9F, 0x23, 0x17, 0x96, 0x31, 0x60, 0x07,
  0x70, 0x62, 0xC6, 0x81, 0x15, 0x23, 0x16, 0x4C, 0xD8, 0x4C, 0x44, 0xAA, 0x72, 0x9E, 0x0A, 0xA9,
  0xEE, 0xB9, 0xF8, 0xB6, 0x3F, 0x78, 0xAA, 0x49, 0xF5, 0xE8, 0xA8, 0x36, 0xFF, 0xCF, 0xFD, 0xDE,
  0xF6, 0xB7, 0x8E, 0xCA, 0xF3, 0x4B, 0x5D, 0x5D, 0x53, 0x30, 0xE6, 0x5F, 0x44, 0x7A, 0xA3, 0x7A,
  0x1E, 0x2A, 0x9F, 0xB3, 0x2E, 0xD5, 0x6F, 0xFB, 0x6B, 0x4C, 0x2C, 0x22, 0xC1, 0xA8, 0xC0, 0x43,
  0x4F, 0xFB, 0x2F, 0xDF, 0xA6, 0xFB, 0x2B, 0x35, 0xE9, 0xFE, 0x8A, 0x48, 0x0D, 0x17, 0xDC, 0x21,
  0x16, 0x25
};

static const uint8_t  sAlignIconsY_data[303] =
{
  0x78, 0x01, 0xED, 0xD6, 0x4B, 0x55, 0xC5, 0x30, 0x10, 0x06, 0xE0, 0xC9, 0xEB, 0x4F, 0xDB, 0xB4,
  0x89, 0x04, 0x24, 0x20, 0x01, 0x09, 0x48, 0x40, 0x02, 0x12, 0x90, 0x80, 0x04, 0x24, 0x20, 0x01,
  0x09, 0x48, 0x40, 0x02, 0x12, 0x86, 0xE4, 0xDC, 0xD2, 0xB2, 0xE0, 0xE6, 0x71, 0x4E, 0xBA, 0xBA,
  0xFD, 0x76, 0x59, 0xB4, 0x7F, 0x9A, 0x33, 0x9D, 0x09, 0x07, 0xBE, 0xE3, 0x7B, 0xFE, 0xE6, 0x67,
  0x7E, 0x63, 0xE6, 0x17, 0x7E, 0xE4, 0x0F, 0x7E, 0xE7, 0x2F, 0x7E, 0xE2, 0x57, 0xFE, 0xE4, 0x07,
  0x3A, 0x9D, 0x4E, 0x24, 0xAE, 0x90, 0x82, 0x72, 0x4F, 0x29, 0x4D, 0x7D, 0x18, 0x5C, 0x61, 0x28,
  0x43, 0xC2, 0x52, 0x1F, 0x83, 0xFD, 0xA5, 0x2D, 0x30, 0xEA, 0x6D, 0x39, 0x50, 0x46, 0xBF, 0xFC,
  0x3F, 0xA6, 0xF8, 0xD5, 0xD9, 0xD4, 0xDD, 0x11, 0xF9, 0x23, 0x94, 0x72, 0x35, 0x1B, 0x98, 0x87,
  0x4B, 0xBE, 0x95, 0xD4, 0xD1, 0x88, 0x79, 0x51, 0xC2, 0x63, 0xA6, 0x12, 0x87, 0x20, 0xA0, 0x63,
  0xE5, 0xF4, 0xAA, 0xC1, 0x44, 0xA7, 0x37, 0x2A, 0x12, 0xCE, 0x51, 0x89, 0x54, 0x6A, 0x86, 0x5D,
  0xB0, 0x50, 0x4F, 0x81, 0x52, 0x3E, 0x89, 0x8A, 0x43, 0x95, 0x0E, 0x50, 0x07, 0x54, 0x40, 0xCA,
  0xAF, 0x92, 0x36, 0x50, 0x1B, 0x1F, 0x7C, 0x75, 0x99, 0x14, 0xF2, 0x8D, 0x72, 0x2B, 0xAF, 0x00,
  0xB7, 0xF1, 0xD9, 0x56, 0x35, 0x23, 0x50, 0x25, 0x03, 0xCA, 0xB1, 0x7E, 0x5C, 0x4D, 0x1E, 0x98,
  0xC6, 0x4D, 0xAF, 0x7C, 0x3D, 0x51, 0x1D, 0xD1, 0x70, 0xFE, 0x0D, 0xF9, 0xB5, 0x2E, 0xF5, 0x57,
  0x53, 0xFE, 0x61, 0xCD, 0xEF, 0xBA, 0x85, 0xFA, 0xFF, 0x4F, 0xC6, 0x29, 0x92, 0xF2, 0x0D, 0x7A,
  0xB6, 0xAA, 0x86, 0xFE, 0x13, 0x37, 0x19, 0xF3, 0x4D, 0xDF, 0x56, 0xD1, 0xD2, 0x7F, 0x17, 0x38,
  0xF8, 0xCE, 0x9D, 0x8A, 0x5A, 0xE6, 0x4F, 0x1C, 0xAA, 0x07, 0x74, 0xAA, 0x52, 0xFE, 0x3E, 0xC6,
  0xB5, 0x83, 0xDB, 0x57, 0xE5, 0xE1, 0x52, 0xA7, 0x94, 0xAF, 0xF0, 0x3F, 0x4F, 0x7D, 0x94, 0xEE,
  0x5F, 0xF1, 0xFA, 0xD6, 0x7E, 0xA9, 0x3B, 0x9D, 0x6E, 0xC6, 0x0F, 0x2A, 0x7C, 0x22, 0x27
};

static const EvBmp    sShapeIcons  = {ZIP_DATA, PALETTED4444, 128, 32, 32, 4096, 243, sShapeIcons_data, "ShapeIcons"};
static const EvBmp    sAlignIconsX = {ZIP_DATA, PALETTED4444, 128, 32, 22, 4096, 130, sAlignIconsX_data, "AlignIconsX"};
static const EvBmp    sAlignIconsY = {ZIP_DATA, PALETTED4444, 128, 32, 32, 4096, 303, sAlignIconsY_data, "AlignIconsY"};

const EvBmp       *ShapeIcons = &sShapeIcons;
const EvBmp       *AlignIconsX = &sAlignIconsX;
const EvBmp       *AlignIconsY = &sAlignIconsY;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static EvEditor   *editor = nullptr;
static EvObj      *editObj = nullptr;
static EvProperty *property = nullptr;

static int16_t    propLeft = 0;
static int16_t    propTop = 0;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const char  *OpenEditor(EvDisplay *Disp)
{
  if (editor != nullptr)
    return "Editor already open";

  if ((editor = (EvEditor *)EvObj::TryCreate(new EvEditor(Disp, "Editor"), Disp)) == nullptr ||
      (property = (EvProperty *)EvObj::TryCreate(new EvProperty(Disp, "Property"), Disp)) == nullptr)
    {
      if (editor != nullptr)
        editor->Delete();

      return "Cannot open editor";
    }

  SetEditObj(editObj);
  return "Editor is open";
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const char  *CloseEditor(void)
{
  if (editor == nullptr)
    return "Editor already closed";

  editor->Delete();
  property->Delete();
  return "Editor is closed";
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EditorToFront(void)
{
  if (editor != nullptr)
  {
    editor->ToFront();
    property->ToFront();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        SetEditObj(EvObj *Obj)
{
  editObj = Obj;

  if (editor == nullptr)
    return;

  if (Obj == nullptr)
  {
    property->NumLeft->Disable();
    property->NumTop->Disable();
    property->NumWidth->Disable();
    property->NumHeight->Disable();
    property->NumFont->Disable();
    property->NumPadX->Disable();
    property->NumPadY->Disable();
    property->SelShape->Disable();
    property->SelAlignX->Disable();
    property->SelAlignY->Disable();
    property->NumLeft->SetValue(0);
    property->NumTop->SetValue(0);
    property->NumWidth->SetValue(0);
    property->NumHeight->SetValue(0);
    property->NumFont->SetValue(0);
    property->NumFont->TextColor(TEXT_COLOR);
    property->NumPadX->SetValue(0);
    property->NumPadY->SetValue(0);
    property->SelShape->SetValue(-1);
    property->SelAlignX->SetValue(-1);
    property->SelAlignY->SetValue(-1);
    property->LabTag->TextLabel("Select Object");
    property->LabTag->TextColor(TEXT_ERROR);
    editor->Hide();
  }
  else
  {
    editor->MoveTo(Obj->Disp);
    editor->ReSize(Obj->Disp->Width(), Obj->Disp->Height());

    if (property->TglEdit->Value())
    {
      property->NumLeft->Enable();
      property->NumTop->Enable();
      property->NumWidth->Enable();
      property->NumHeight->Enable();
      property->NumFont->Enable();
      property->NumPadX->Enable();
      property->NumPadY->Enable();
      property->SelShape->Enable();

      if (Obj->Style.align & ALIGNMENT_LOCK)
      {
        property->SelAlignX->Disable();
        property->SelAlignY->Disable();
      }
      else
      {
        property->SelAlignX->Enable();
        property->SelAlignY->Enable();
      }

      editor->Show();
    }
    else
    {
      property->NumLeft->Disable();
      property->NumTop->Disable();
      property->NumWidth->Disable();
      property->NumHeight->Disable();
      property->NumFont->Disable();
      property->NumPadX->Disable();
      property->NumPadY->Disable();
      property->SelShape->Disable();
      property->SelAlignX->Disable();
      property->SelAlignY->Disable();
      editor->Hide();
    }

    property->LabTag->TextLabel(Obj->Tag);
    property->LabTag->TextColor(TEXT_COLOR);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        SetEditObjDestroyed(EvObj *Obj)
{
  if (editObj == Obj)
  {
    editObj = nullptr;

    if (editor != nullptr)
      SetEditObj(editObj);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeMinimize(EvButton *Sender, int32_t Value)
{
  if (Value == true)
    return;

  if ((property->Minimized = !property->Minimized) == true)
  {
    property->ReSize(150, 36);
    property->BtnMinimize->TextLabel("+");
    property->BtnMinimize->TextPadding(4, -6);
  }
  else
  {
    property->ReSize(150, 310);
    property->BtnMinimize->TextLabel("_");
    property->BtnMinimize->TextPadding(5, -17);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeEdit(EvToggle *Sender, int32_t Value)
{
  SetEditObj(editObj);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeTab(EvTab *Sender, int32_t Value)
{
  switch (Value)
  {
    case 0:
      property->PanObj->Show();
      property->PanText->Hide();
      break;

    case 1:
      property->PanObj->Hide();
      property->PanText->Show();
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeLeft(EvNumInt *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->MoveTo(Value, editObj->Top());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeTop(EvNumInt *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->MoveTo(editObj->Left(), Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeWidth(EvNumInt *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->ReSize(Value, editObj->Height());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeHeight(EvNumInt *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->ReSize(editObj->Width(), Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeShape(EvSelector *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->BdShape((editObj->Shape() & ~7) | Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeFont(EvNumInt *Sender, int32_t Value)
{
  if (editObj != nullptr)
  {
    Sender->TextColor(editObj->Disp->SystemFont[Value] == 0 ? TEXT_ERROR : TEXT_COLOR);
    editObj->TextFont(Value);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangePadX(EvNumInt *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->TextPadding(Value, editObj->Style.padY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangePadY(EvNumInt *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->TextPadding(editObj->Style.padX, Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeAlignX(EvSelector *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->TextAlign((editObj->Style.align & ~3) | Value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeAlignY(EvSelector *Sender, int32_t Value)
{
  if (editObj != nullptr)
    editObj->TextAlign((editObj->Style.align & ~0x0C) | (Value << 2));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnTouchDotMove(EvDot *Sender, EvTouchEvent *Touch)
{
  int16_t   x, y;

  switch (Touch->event)
  {
    case TOUCH_START:
      Sender->X = editObj->Left();
      Sender->Y = editObj->Top();
      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      Sender->X += Touch->move.x;
      Sender->Y += Touch->move.y;
      x = (((Sender->X << 1) + 5) / 10) * 5;
      y = (((Sender->Y << 1) + 5) / 10) * 5;
      editObj->MoveTo(x, y);
      Touch->event = 0;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnTouchDotReSize(EvDot *Sender, EvTouchEvent *Touch)
{
  int16_t   w, h;

  switch (Touch->event)
  {
    case TOUCH_START:
      Sender->X = editObj->Left() + editObj->Width();
      Sender->Y = editObj->Top() + editObj->Height();
      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      Sender->X += Touch->move.x;
      Sender->Y += Touch->move.y;
      w = ((((Sender->X - editObj->Left()) << 1) + 5) / 10) * 5;
      h = ((((Sender->Y - editObj->Top()) << 1) + 5) / 10) * 5;
      editObj->ReSize(w < 0 ? 0 : w, h < 0 ? 0 : h);
      Touch->event = 0;
      break;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDot::EvDot(uint16_t Size, void (*OnTouch)(EvDot *Sender, EvTouchEvent *Touch), EvDisplay *Disp, const char *Tag) : EvObj(0, 0, Size, Size, Disp, Tag, VISIBLE_OBJ | SYSTEM_OBJ)
{
  mOnTouch = OnTouch;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDot::drawEvent(void)
{
  int16_t   radius = mWidth << 3;

  FillCircle2f(radius, radius, DOT_COLOR, radius >> 1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvDot::touchEvent(EvTouchEvent *Touch)
{
  if (mOnTouch)
    (*mOnTouch)(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvEditor::EvEditor(EvDisplay *Disp, const char *Tag) : EvPanel(0, 0, Disp->Width(), Disp->Height(), Disp, Tag, VISIBLE_DIS_OBJ | SYSTEM_OBJ)
{
  if (!(DotMove = (EvDot *)TryCreate(new EvDot(50, OnTouchDotMove, Disp, "DotMove"), this)) ||
      !(DotReSize = (EvDot *)TryCreate(new EvDot(50, OnTouchDotReSize, Disp, "DotReSize"), this)))
    Abort();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvEditor::~EvEditor(void)
{
  editor = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditor::drawEvent(void)
{
  if (editObj != nullptr)
  {
    int16_t x1, x2, y1, y2;

    x1 = editObj->View.ox;
    y1 = editObj->View.oy;
    x2 = x1 + editObj->Width() - 1;
    y2 = y1 + editObj->Height() - 1;

    Disp->LineWidth(8);
    Disp->Begin(LINES);
    Disp->ColorA(255);
    Disp->ColorRGB(DOT_COLOR);
    Disp->Vertex2i(x1, 0);
    Disp->Vertex2i(x1, mHeight);
    Disp->Vertex2i(x2, 0);
    Disp->Vertex2i(x2, mHeight);
    Disp->Vertex2i(0, y1);
    Disp->Vertex2i(mWidth, y1);
    Disp->Vertex2i(0, y2);
    Disp->Vertex2i(mWidth, y2);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditor::refreshEvent(void)
{
  if (editObj != nullptr)
  {
    int16_t x = editObj->View.ox - (DotMove->Width() / 2);
    int16_t y = editObj->View.oy - (DotMove->Height() / 2);

    if (x != DotMove->Left() || y != DotMove->Top())
    {
      DotMove->MoveTo(x, y);
      DotMove->Modified();
      Modified();
    }

    x += editObj->Width();
    y += editObj->Height();

    if (x != DotReSize->Left() || y != DotReSize->Top())
    {
      DotReSize->MoveTo(x, y);
      DotReSize->Modified();
      Modified();
    }
  }
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvProperty::EvProperty(EvDisplay *Disp, const char *Tag) : EvPanel(0, 0, 150, 310, Disp, Tag, VISIBLE_OBJ | SYSTEM_OBJ)
{
  if (property || !(LabTitle = EvLabel::Create(40, 0, 65, 36, this, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(BtnMinimize = EvButton::Create(6, 6, 25, 25, this, nullptr, VISIBLE_OBJ | SYSTEM_OBJ)) ||
      !(TglEdit = EvToggle::Create(105, 6, 40, 25, this, nullptr, VISIBLE_OBJ | SYSTEM_OBJ)) ||
      !(TabPanel = EvTab::Create(1, 35, mWidth - 2, 31, this, nullptr, VISIBLE_OBJ | SYSTEM_OBJ)) ||
      !(PanObj = EvPanel::Create(1, 65, mWidth - 2, 213, this, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(PanText = EvPanel::Create(1, 65, mWidth - 2, 213, this, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabTag = EvLabel::Create(7, 283, mWidth - 11, 20, this, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabLeft = EvLabel::Create(5, 10, 63, 32, PanObj, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabTop = EvLabel::Create(5, 50, 63, 32, PanObj, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabWidth = EvLabel::Create(5, 90, 63, 32, PanObj, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabHeight = EvLabel::Create(5, 130, 63, 32, PanObj, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(NumLeft = EvNumInt::Create(73, 10, 65, 32, PanObj, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumTop = EvNumInt::Create(73, 50, 65, 32, PanObj, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumWidth = EvNumInt::Create(73, 90, 65, 32, PanObj, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumHeight = EvNumInt::Create(73, 130, 65, 32, PanObj, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(SelShape = EvSelector::Create(10, 172, 128, 32, PanObj, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(LabFont = EvLabel::Create(5, 10, 63, 32, PanText, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabPadX = EvLabel::Create(5, 50, 63, 32, PanText, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabPadY = EvLabel::Create(5, 90, 63, 32, PanText, nullptr, VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(NumFont = EvNumInt::Create(73, 10, 65, 32, PanText, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumPadX = EvNumInt::Create(73, 50, 65, 32, PanText, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumPadY = EvNumInt::Create(73, 90, 65, 32, PanText, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(SelAlignX = EvSelector::Create(10, 132, 128, 32, PanText, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(SelAlignY = EvSelector::Create(10, 172, 128, 32, PanText, nullptr, VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !SelShape->SetBmp(ShapeIcons, 4) || !SelAlignX->SetBmp(AlignIconsX, 4) || !SelAlignY->SetBmp(AlignIconsY, 3))
    {
      Abort();
      return;
    }

  property = this;
  Minimized = false;
  BgColor(BG_COLOR);
  BdShape(FIXED_CORNERS);
  MoveTo(propLeft, propTop);

  LabTitle->TextLabel("Editor");
  LabTitle->TextColor(TEXT_COLOR);
  LabTitle->TextAlign(LEFT_CENTER);

  BtnMinimize->TextLabel("_");
  BtnMinimize->BdShape(ROUND_CORNERS);
  BtnMinimize->TextFont(27);
  BtnMinimize->TextAlign(LEFT_TOP);
  BtnMinimize->TextPadding(5, -17);
  BtnMinimize->SetOnChange(OnChangeMinimize);

  TabPanel->TextFont(25);
  TabPanel->TextAlign(CENTER);
  TabPanel->TextColor(TEXT_COLOR, TEXT_COLOR);
  TabPanel->Items.Add("Obj|Text", '|');
  TabPanel->HideDelimiter();
  TabPanel->SetColor(0, PANEL_COLOR, COLOR_SELNEW);
  TabPanel->SetOnChange(OnChangeTab);

  PanObj->BdRadius(1 << 4);
  PanObj->BgColor(PANEL_COLOR);
  PanObj->Show();

  PanText->BdRadius(1 << 4);
  PanText->BgColor(PANEL_COLOR);
  PanText->Hide();

  TglEdit->SetValue(true);
  TglEdit->SetOnChange(OnChangeEdit);

  LabLeft->TextLabel("Left");
  LabLeft->TextColor(TEXT_COLOR);

  LabTop->TextLabel("Top");
  LabTop->TextColor(TEXT_COLOR);

  LabWidth->TextLabel("Width");
  LabWidth->TextColor(TEXT_COLOR);

  LabHeight->TextLabel("Height");
  LabHeight->TextColor(TEXT_COLOR);

  NumLeft->BgColor(NUM_COLOR);
  NumLeft->TextFont(26);
  NumLeft->TextColor(TEXT_COLOR);
  NumLeft->SetRange(-32768, 32767);
  NumLeft->SetOnChange(OnChangeLeft);

  NumTop->BgColor(NUM_COLOR);
  NumTop->TextFont(26);
  NumTop->TextColor(TEXT_COLOR);
  NumTop->SetRange(-32768, 32767);
  NumTop->SetOnChange(OnChangeTop);

  NumWidth->BgColor(NUM_COLOR);
  NumWidth->TextFont(26);
  NumWidth->TextColor(TEXT_COLOR);
  NumWidth->SetRange(0, 2048);
  NumWidth->SetOnChange(OnChangeWidth);

  NumHeight->BgColor(NUM_COLOR);
  NumHeight->TextFont(26);
  NumHeight->TextColor(TEXT_COLOR);
  NumHeight->SetRange(0, 2048);
  NumHeight->SetOnChange(OnChangeHeight);

  SelShape->BdRadius(3 << 4);
  SelShape->TextColor(COLOR_ICONS, TEXT_SELECT);
  SelShape->SetColor(COLOR_SELNONE, COLOR_SELECT, COLOR_SELNEW);
  SelShape->SetOnChange(OnChangeShape);

  LabFont->TextLabel("Font");
  LabFont->TextColor(TEXT_COLOR);

  LabPadX->TextLabel("PadX");
  LabPadX->TextColor(TEXT_COLOR);

  LabPadY->TextLabel("PadY");
  LabPadY->TextColor(TEXT_COLOR);

  NumFont->BgColor(NUM_COLOR);
  NumFont->TextFont(26);
  NumFont->TextColor(TEXT_COLOR);
  NumFont->SetRange(1, 31);
  NumFont->SetOnChange(OnChangeFont);

  NumPadX->BgColor(NUM_COLOR);
  NumPadX->TextFont(26);
  NumPadX->TextColor(TEXT_COLOR);
  NumPadX->SetRange(-128, 127);
  NumPadX->SetOnChange(OnChangePadX);

  NumPadY->BgColor(NUM_COLOR);
  NumPadY->TextFont(26);
  NumPadY->TextColor(TEXT_COLOR);
  NumPadY->SetRange(-128, 127);
  NumPadY->SetOnChange(OnChangePadY);

  SelAlignX->BdRadius(3 << 4);
  SelAlignX->TextColor(COLOR_ICONS, TEXT_SELECT);
  SelAlignX->SetColor(COLOR_SELNONE, COLOR_SELECT, COLOR_SELNEW);
  SelAlignX->SetOnChange(OnChangeAlignX);

  SelAlignY->BdRadius(3 << 4);
  SelAlignY->TextColor(COLOR_ICONS, TEXT_SELECT);
  SelAlignY->SetColor(COLOR_SELNONE, COLOR_SELECT, COLOR_SELNEW);
  SelAlignY->SetOnChange(OnChangeAlignY);

  LabTag->TextFont(24);
  LabTag->TextAlign(LEFT_TOP);
  LabTag->TextColor(TEXT_COLOR);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvProperty::~EvProperty(void)
{
  property = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvProperty::refreshEvent(void)
{
  if (editObj != nullptr)
  {
    int16_t shape = editObj->Shape() & 7;

    NumLeft->SetValue(editObj->Left());
    NumTop->SetValue(editObj->Top());
    NumWidth->SetValue(editObj->Width());
    NumHeight->SetValue(editObj->Height());
    SelShape->SetValue((shape == USER_CORNERS ? -1 : shape));
    NumFont->SetValue(editObj->Style.font);
    NumFont->TextColor(editObj->Disp->SystemFont[editObj->Style.font] == nullptr ? TEXT_ERROR : TEXT_COLOR);
    NumPadX->SetValue(editObj->Style.padX);
    NumPadY->SetValue(editObj->Style.padY);
    SelAlignX->SetValue(editObj->Style.align & 3);
    SelAlignY->SetValue((editObj->Style.align >> 2) & 3);

    if (TglEdit->Value() == 0 || editObj->Style.align & ALIGNMENT_LOCK)
    {
      SelAlignX->Disable();
      SelAlignY->Disable();
    }
    else
    {
      SelAlignX->Enable();
      SelAlignY->Enable();
    }
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvProperty::touchEvent(EvTouchEvent *Touch)
{
  switch (Touch->event)
  {
    case TOUCH_START:
      SetOpacity(170);
      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      MoveRel(Touch->move.x, Touch->move.y);
      propLeft = Left();
      propTop = Top();
      Touch->event = 0;
      break;

    case TOUCH_END:
      SetOpacity(OPACITY_MAX);
      Touch->event = 0;
      break;
  }
}
