
#include    <EvEGL.h>

#define     BG_COLOR        RGB555(240, 240, 240)
#define     PANEL_COLOR     CL_LIGHT_GRAY
#define     NUM_COLOR       CL_WHITE
#define     DOT_COLOR       CL_RED
#define     TEXT_COLOR      CL_BLACK
#define     TEXT_ERROR      CL_RED
#define     TEXT_SELECT     CL_WHITE

#define     COLOR_SELNONE   CL_WHITE
#define     COLOR_SELECT    RGB555(  0,   0, 160)
#define     COLOR_SELNEW    RGB555(160, 160, 210)
#define     COLOR_ICONS     RGB555(140, 140, 140)

#define     TGL_EDIT_ON     RGB555( 50, 200, 100)
#define     TGL_EDIT_OFF    CL_LIGHT_GRAY
#define     TGL_EDIT_KNOB   CL_WHITE

#define     BTN_UP          CL_LIGHT_GRAY
#define     BTN_DOWN        CL_GRAY

#define     NUM_INT_HOLD    RGB555(  0,   0, 160)
#define     NUM_INT_INC     RGB555( 50, 200, 100)
#define     NUM_INT_DEC     RGB555(230,   0,   0)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const char * const EvEditor::TypeName = "EvEditor";
const char * const EvEditDot::TypeName = "EvEditDot";
const char * const EvEditProp::TypeName = "EvEditProp";

int16_t     EvEditProp::sLeft = 0;
int16_t     EvEditProp::sTop = 0;

static EvEditor   *editor = nullptr;
static EvObj      *editObj = nullptr;
static EvEditProp *property = nullptr;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static const uint8_t  sShapeIcons_data[238] =
{
  0x78, 0x01, 0xED, 0x96, 0x5B, 0x11, 0xC2, 0x30, 0x10, 0x45, 0xB7, 0xCD, 0xF6, 0x11, 0xFA, 0x92,
  0x80, 0x04, 0x24, 0x20, 0x01, 0x09, 0x48, 0x40, 0x02, 0x12, 0x90, 0x80, 0x04, 0x24, 0x20, 0x01,
  0x09, 0x48, 0x40, 0xC2, 0x05, 0x06, 0x5A, 0x92, 0x74, 0xA7, 0xF4, 0xA3, 0xD3, 0x30, 0x43, 0xCE,
  0xEF, 0x49, 0x73, 0xDB, 0x64, 0xBB, 0x09, 0x1A, 0x6C, 0x71, 0xC2, 0x0D, 0xC0, 0x15, 0x47, 0xAC,
  0x71, 0xC6, 0x05, 0x7B, 0x1C, 0xB0, 0xC4, 0x0A, 0x1B, 0xEC, 0x28, 0x10, 0x08, 0xFC, 0x04, 0x51,
  0xAC, 0x98, 0x93, 0x34, 0x23, 0xC9, 0xE5, 0x0F, 0xD7, 0xA2, 0x17, 0x64, 0x53, 0x18, 0xF2, 0x4D,
  0x5C, 0x76, 0x36, 0x67, 0x91, 0x82, 0x2C, 0xB2, 0xA4, 0x15, 0x79, 0x49, 0x0E, 0x95, 0x66, 0x1B,
  0x55, 0x9B, 0x5A, 0x0C, 0x50, 0xDD, 0x2C, 0x9A, 0x45, 0xEC, 0xFC, 0xC8, 0x30, 0xAA, 0xB2, 0x14,
  0x55, 0xFD, 0xCF, 0x63, 0xE3, 0x05, 0x6A, 0x16, 0xE9, 0xE6, 0x1F, 0x93, 0x9F, 0x59, 0x2A, 0xB1,
  0x57, 0x40, 0x9C, 0xE0, 0x33, 0x64, 0xC1, 0x22, 0xF9, 0xE0, 0xE3, 0x4E, 0x7E, 0x62, 0xBB, 0xD4,
  0x74, 0x11, 0x4B, 0xA4, 0xDF, 0xF2, 0x35, 0x8D, 0xCF, 0x6F, 0xD8, 0xC1, 0xDC, 0x01, 0xB9, 0x7E,
  0x14, 0x4D, 0x98, 0xDF, 0x1B, 0x62, 0x56, 0xB8, 0xB0, 0xFB, 0x4F, 0xAA, 0x09, 0xF3, 0xD9, 0x25,
  0x1E, 0x92, 0x2F, 0xB2, 0xE9, 0xF2, 0x4B, 0x76, 0xD1, 0x7F, 0x95, 0xEF, 0x7B, 0xFD, 0xBD, 0xD7,
  0x9F, 0xEF, 0xFF, 0xCF, 0x77, 0xFF, 0x71, 0x7A, 0xF8, 0xFC, 0xFD, 0xD7, 0xF7, 0xF9, 0xE3, 0xFD,
  0xFC, 0x25, 0xDF, 0xF7, 0x8F, 0x40, 0x20, 0x30, 0x37, 0x77, 0x2C, 0x2D, 0x33, 0x1E
};

static const uint8_t  sAlignIconsX_data[154] =
{
  0x78, 0x01, 0xED, 0xD4, 0xB1, 0x15, 0xC2, 0x30, 0x0C, 0x04, 0x50, 0xD9, 0x89, 0x15, 0x8B, 0x14,
  0x8C, 0xC0, 0x08, 0x8C, 0xC0, 0x28, 0x8C, 0xC0, 0x28, 0x8C, 0xC0, 0x08, 0x8C, 0xC2, 0x28, 0x8C,
  0x70, 0x54, 0xF6, 0x7B, 0xE2, 0x5D, 0xE1, 0x54, 0x49, 0xA1, 0x5F, 0x4A, 0xA7, 0x4E, 0xEF, 0x70,
  0xC6, 0x1D, 0x5F, 0x7C, 0xF0, 0xC2, 0x0D, 0x6F, 0x3C, 0xF1, 0xC0, 0x15, 0xC0, 0x45, 0x42, 0x08,
  0xBB, 0x4A, 0x99, 0x9A, 0xDA, 0x7E, 0xCA, 0x54, 0x1A, 0xBD, 0x9F, 0x33, 0x35, 0xB7, 0x7D, 0x51,
  0x6A, 0x69, 0xFB, 0x45, 0xA9, 0x32, 0x7A, 0x5F, 0x95, 0xAA, 0x12, 0x0E, 0xC1, 0xAA, 0x23, 0x94,
  0x8B, 0x24, 0xA1, 0x0A, 0x0F, 0xF9, 0x71, 0xD7, 0xFF, 0xA7, 0x9E, 0x1C, 0x13, 0xC2, 0x5C, 0x24,
  0x0B, 0xA5, 0x3C, 0xE4, 0xC7, 0x9D, 0xF6, 0x3B, 0x73, 0x84, 0xB2, 0x81, 0xCC, 0x6A, 0x34, 0xF4,
  0x37, 0x6E, 0x56, 0x09, 0x87, 0x10, 0xFD, 0x1B, 0xF6, 0x14, 0xFD, 0xEB, 0x08, 0x65, 0x03, 0x99,
  0xE8, 0xDF, 0x10, 0x36, 0xF9, 0x01, 0x94, 0x62, 0x26, 0x17
};

static const uint8_t  sAlignIconsY_data[274] =
{
  0x78, 0x01, 0xED, 0x96, 0x41, 0x79, 0xC3, 0x30, 0x0C, 0x46, 0x15, 0xFB, 0x8F, 0x5D, 0x27, 0x4E,
  0x03, 0x61, 0x10, 0x06, 0x61, 0x10, 0x06, 0x61, 0x10, 0x06, 0x61, 0x10, 0x06, 0xA1, 0x10, 0x0A,
  0xA1, 0x10, 0x0A, 0xA1, 0x10, 0x0A, 0x41, 0x4D, 0xDA, 0x83, 0x73, 0x89, 0xAD, 0xF6, 0x53, 0x4E,
  0xF5, 0xBB, 0xE4, 0xD3, 0xE9, 0xC9, 0xB2, 0x2C, 0x85, 0x47, 0xFE, 0xE0, 0x0B, 0x9F, 0x98, 0xF9,
  0x8F, 0xCF, 0xFC, 0xCB, 0xFF, 0xFC, 0xC9, 0x3F, 0x7C, 0xE0, 0x6F, 0x3E, 0xF2, 0x17, 0x5F, 0xA9,
  0x52, 0xA9, 0x10, 0x35, 0x2B, 0x50, 0x16, 0x63, 0x49, 0x09, 0xAC, 0x60, 0x28, 0x87, 0x9E, 0xBF,
  0x4D, 0x38, 0xD8, 0x14, 0x78, 0xCA, 0xA1, 0xE7, 0x4F, 0xB4, 0xD3, 0xA9, 0x5B, 0x92, 0xB1, 0x81,
  0x7F, 0x07, 0x03, 0x27, 0x49, 0x20, 0x74, 0x0F, 0x7F, 0x17, 0x48, 0x91, 0x1D, 0xFA, 0x88, 0xD0,
  0x0B, 0x12, 0x18, 0xE0, 0x67, 0xBF, 0xC7, 0x40, 0x7A, 0x04, 0xF4, 0x34, 0xF9, 0xA9, 0x47, 0xF9,
  0x54, 0x16, 0xDE, 0x58, 0x0F, 0xDD, 0x2B, 0xD8, 0xD3, 0xDD, 0x3F, 0x7F, 0x8B, 0x58, 0x60, 0x84,
  0x7E, 0x07, 0x44, 0x48, 0x6F, 0x74, 0x4A, 0x40, 0xAC, 0xB7, 0x91, 0x84, 0x14, 0xFC, 0xDE, 0x25,
  0x80, 0x45, 0x90, 0x2F, 0x19, 0xF2, 0x2F, 0x79, 0x41, 0xC1, 0xDF, 0x0D, 0x09, 0x60, 0x11, 0x68,
  0xF9, 0xF7, 0x76, 0x8B, 0xFA, 0xCB, 0xFD, 0x62, 0xC4, 0xFD, 0x17, 0x9A, 0x87, 0x5F, 0xD2, 0xD4,
  0x72, 0xE4, 0xEF, 0x6F, 0x74, 0xCD, 0xEC, 0xF7, 0xAA, 0x35, 0x78, 0x62, 0xFE, 0x44, 0xB8, 0x00,
  0xAF, 0x3C, 0x2A, 0x9E, 0x99, 0xBF, 0xF1, 0x3E, 0xD4, 0xF5, 0x47, 0x85, 0x78, 0xFF, 0x44, 0x60,
  0x03, 0x7D, 0xC9, 0xDF, 0xA5, 0x3D, 0x3E, 0xC0, 0xA4, 0x40, 0xBA, 0x5E, 0x8B, 0x14, 0xFC, 0x16,
  0x2B, 0x90, 0x12, 0xA5, 0xFA, 0x37, 0x2F, 0xFD, 0xD5, 0x55, 0x2A, 0x6F, 0xC2, 0x0D, 0x8E, 0x73,
  0x20, 0x1A
};

static const EvBmp    sShapeIcons = {ZIP_DATA, PALETTED4444, 128, 32, 32, 4096, 238, sShapeIcons_data, "ShapeIcons"};
static const EvBmp    sAlignIconsX = {ZIP_DATA, PALETTED4444, 128, 32, 24, 4096, 154, sAlignIconsX_data, "AlignIconsX"};
static const EvBmp    sAlignIconsY = {ZIP_DATA, PALETTED4444, 128, 32, 32, 4096, 274, sAlignIconsY_data, "AlignIconsY"};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void OnChangeMinimize(EvButton *Sender, int32_t Value);
static void OnChangeEdit(EvToggle *Sender, int32_t Value);
static void OnChangeTab(EvTab *Sender, int32_t Value);
static void OnChangeLeft(EvNumInt *Sender, int32_t Value);
static void OnChangeTop(EvNumInt *Sender, int32_t Value);
static void OnChangeWidth(EvNumInt *Sender, int32_t Value);
static void OnChangeHeight(EvNumInt *Sender, int32_t Value);
static void OnChangeShape(EvSelector *Sender, int32_t Value);
static void OnChangeFont(EvNumInt *Sender, int32_t Value);
static void OnChangePadX(EvNumInt *Sender, int32_t Value);
static void OnChangePadY(EvNumInt *Sender, int32_t Value);
static void OnChangeAlignX(EvSelector *Sender, int32_t Value);
static void OnChangeAlignY(EvSelector *Sender, int32_t Value);
static void OnTouchDotMove(EvEditDot *Sender, const EvTouchEvent *Touch);
static void OnTouchDotReSize(EvEditDot *Sender, const EvTouchEvent *Touch);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvEditor::EvEditor(EvDisplay *Disp, const char *Tag) :
  EvPanel(0, 0, Disp->Width(), Disp->Height(), Disp, Tag, VISIBLE_DIS_OBJ | SYSTEM_OBJ)
{
  if (!(DotMove = (EvEditDot *)TryCreate(new EvEditDot(50, OnTouchDotMove, Disp, "DotMove"), this)) ||
      !(DotReSize = (EvEditDot *)TryCreate(new EvEditDot(50, OnTouchDotReSize, Disp, "DotReSize"), this)))
    abortCreate();

  BgColor(CL_NOCOLOR);
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

bool        EvEditor::Close(void)
{
  if (editor == nullptr)
    return false;

  editor->Delete();
  property->Delete();
  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvEditor::Open(EvDisplay *Disp)
{
  if (editor == nullptr)
  {
    if ((editor = (EvEditor *)EvObj::TryCreate(new EvEditor(Disp, "Editor"), Disp)) == nullptr ||
        (property = (EvEditProp *)EvObj::TryCreate(new EvEditProp(Disp, "EditProp"), Disp)) == nullptr)
      {
        if (editor != nullptr)
          editor->Delete();

        return false;
      }

    SelectObj(editObj);
  }

  return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool        EvEditor::IsAlreadyOpen(void)
{
  return editor;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditor::SelectObj(EvObj *Obj)
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
    property->NumFont->TextColor(CL_BLACK);
    property->NumPadX->SetValue(0);
    property->NumPadY->SetValue(0);
    property->SelShape->SetValue(-1);
    property->SelAlignX->SetValue(-1);
    property->SelAlignY->SetValue(-1);
    property->LabTag->TextLabel("Unselected object");
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
    property->LabTag->TextColor(CL_BLACK);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditor::DestroyedObj(EvObj *Obj)
{
  if (editObj == Obj)
  {
    editObj = nullptr;

    if (editor != nullptr)
      SelectObj(editObj);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditor::AlwaysToFront(void)
{
  if (editor != nullptr)
  {
    editor->ToFront();
    property->ToFront();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvEditDot::EvEditDot(uint16_t Size, void (*OnTouch)(EvEditDot *Sender, const EvTouchEvent *Touch), EvDisplay *Disp, const char *Tag) :
  EvObj(0, 0, Size, Size, Disp, Tag, VISIBLE_OBJ | SYSTEM_OBJ),
  mOnTouch(OnTouch)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditDot::drawEvent(void)
{
  int16_t   radius = mWidth << 3;

  FillCircle2f(radius, radius, DOT_COLOR, radius >> 1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditDot::touchEvent(const EvTouchEvent *Touch)
{
  if (mOnTouch)
    mOnTouch(this, Touch);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvEditProp::EvEditProp(EvDisplay *Disp, const char *Tag) :
  EvPanel(0, 0, 150, 310, Disp, Tag, VISIBLE_OBJ | SYSTEM_OBJ),
  Minimized(false)
{
  if (property || !(LabTitle = EvLabel::Create(35, 0, 63, 36, this, "Editor", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(BtnMinimize = EvButton::Create(6, 6, 25, 25, this, "Minimize", VISIBLE_OBJ | SYSTEM_OBJ)) ||
      !(TglEdit = EvToggle::Create(105, 6, 40, 25, this, "TglEdit", VISIBLE_OBJ | SYSTEM_OBJ)) ||
      !(TabPanel = EvTab::Create(1, 35, mWidth - 2, 31, this, "TabPanel", VISIBLE_OBJ | SYSTEM_OBJ)) ||
      !(PanObj = EvPanel::Create(1, 65, mWidth - 2, 213, this, "PanObj", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(PanText = EvPanel::Create(1, 65, mWidth - 2, 213, this, "PanText", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabTag = EvLabel::Create(7, 283, mWidth - 11, 20, this, "Tag", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabLeft = EvLabel::Create(5, 10, 63, 32, PanObj, "Left", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabTop = EvLabel::Create(5, 50, 63, 32, PanObj, "Top", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabWidth = EvLabel::Create(5, 90, 63, 32, PanObj, "Width", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabHeight = EvLabel::Create(5, 130, 63, 32, PanObj, "Height", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(NumLeft = EvNumInt::Create(73, 10, 65, 32, PanObj, "NumLeft", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumTop = EvNumInt::Create(73, 50, 65, 32, PanObj, "NumTop", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumWidth = EvNumInt::Create(73, 90, 65, 32, PanObj, "NumWidth", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumHeight = EvNumInt::Create(73, 130, 65, 32, PanObj, "NumHeight", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(SelShape = EvSelector::Create(10, 172, 128, 32, PanObj, "Shape", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(LabFont = EvLabel::Create(5, 10, 63, 32, PanText, "Font", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabPadX = EvLabel::Create(5, 50, 63, 32, PanText, "PadX", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(LabPadY = EvLabel::Create(5, 90, 63, 32, PanText, "PadY", VISIBLE_DIS_OBJ | SYSTEM_OBJ)) ||
      !(NumFont = EvNumInt::Create(73, 10, 65, 32, PanText, "NumFont", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumPadX = EvNumInt::Create(73, 50, 65, 32, PanText, "NumPadX", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(NumPadY = EvNumInt::Create(73, 90, 65, 32, PanText, "NumPadY", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(SelAlignX = EvSelector::Create(10, 132, 128, 32, PanText, "AlignX", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !(SelAlignY = EvSelector::Create(10, 172, 128, 32, PanText, "AlignY", VISIBLE_OBJ | SYSTEM_OBJ | FILTER_DIS_OBJ)) ||
      !SelShape->SetBmp(&sShapeIcons, 4) || !SelAlignX->SetBmp(&sAlignIconsX, 4) || !SelAlignY->SetBmp(&sAlignIconsY, 3))
    {
      abortCreate();
      return;
    }

  property = this;
  Minimized = false;
  BgColor(BG_COLOR);
  BdShape(FIXED_CORNERS);
  MoveTo(sLeft, sTop);

  BtnMinimize->TextLabel("_");
  BtnMinimize->BdShape(ROUND_CORNERS);
  BtnMinimize->TextFont(27);
  BtnMinimize->TextAlign(LEFT_TOP);
  BtnMinimize->TextPadding(5, -17);
  BtnMinimize->TextColor(TEXT_COLOR);
  BtnMinimize->SetColor(BTN_UP, BTN_DOWN);
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
  TglEdit->SetColor(TGL_EDIT_ON, TGL_EDIT_OFF, TGL_EDIT_KNOB);
  TglEdit->SetOnChange(OnChangeEdit);

  NumLeft->BgColor(NUM_COLOR);
  NumLeft->TextFont(26);
  NumLeft->TextColor(TEXT_COLOR);
  NumLeft->SetColor(NUM_INT_HOLD, NUM_INT_INC, NUM_INT_DEC);
  NumLeft->SetRange(-32768, 32767);
  NumLeft->SetOnChange(OnChangeLeft);

  NumTop->BgColor(NUM_COLOR);
  NumTop->TextFont(26);
  NumTop->TextColor(TEXT_COLOR);
  NumTop->SetColor(NUM_INT_HOLD, NUM_INT_INC, NUM_INT_DEC);
  NumTop->SetRange(-32768, 32767);
  NumTop->SetOnChange(OnChangeTop);

  NumWidth->BgColor(NUM_COLOR);
  NumWidth->TextFont(26);
  NumWidth->TextColor(TEXT_COLOR);
  NumWidth->SetColor(NUM_INT_HOLD, NUM_INT_INC, NUM_INT_DEC);
  NumWidth->SetRange(0, 2048);
  NumWidth->SetOnChange(OnChangeWidth);

  NumHeight->BgColor(NUM_COLOR);
  NumHeight->TextFont(26);
  NumHeight->TextColor(TEXT_COLOR);
  NumHeight->SetColor(NUM_INT_HOLD, NUM_INT_INC, NUM_INT_DEC);
  NumHeight->SetRange(0, 2048);
  NumHeight->SetOnChange(OnChangeHeight);

  SelShape->BdRadius(3 << 4);
  SelShape->TextColor(COLOR_ICONS, TEXT_SELECT);
  SelShape->SetColor(COLOR_SELNONE, COLOR_SELECT, COLOR_SELNEW);
  SelShape->SetOnChange(OnChangeShape);

  NumFont->BgColor(NUM_COLOR);
  NumFont->TextFont(26);
  NumFont->TextColor(TEXT_COLOR);
  NumFont->SetColor(NUM_INT_HOLD, NUM_INT_INC, NUM_INT_DEC);
  NumFont->SetRange(1, 31);
  NumFont->SetOnChange(OnChangeFont);

  NumPadX->BgColor(NUM_COLOR);
  NumPadX->TextFont(26);
  NumPadX->TextColor(TEXT_COLOR);
  NumPadX->SetColor(NUM_INT_HOLD, NUM_INT_INC, NUM_INT_DEC);
  NumPadX->SetRange(-128, 127);
  NumPadX->SetOnChange(OnChangePadX);

  NumPadY->BgColor(NUM_COLOR);
  NumPadY->TextFont(26);
  NumPadY->TextColor(TEXT_COLOR);
  NumPadY->SetColor(NUM_INT_HOLD, NUM_INT_INC, NUM_INT_DEC);
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

  LabTitle->TextColor(TEXT_COLOR);
  LabLeft->TextColor(TEXT_COLOR);
  LabTop->TextColor(TEXT_COLOR);
  LabWidth->TextColor(TEXT_COLOR);
  LabHeight->TextColor(TEXT_COLOR);
  LabFont->TextColor(TEXT_COLOR);
  LabPadX->TextColor(TEXT_COLOR);
  LabPadY->TextColor(TEXT_COLOR);

  LabTag->TextFont(24);
  LabTag->TextAlign(LEFT_CENTER);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvEditProp::~EvEditProp(void)
{
  property = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvEditProp::refreshEvent(void)
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
    NumFont->TextColor(editObj->Disp->SystemFont[editObj->Style.font] == nullptr ? TEXT_ERROR : CL_BLACK);
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

void        EvEditProp::touchEvent(const EvTouchEvent *Touch)
{
  switch (Touch->event)
  {
    case TOUCH_START:
      SetOpacity(170);
      Touch->event = 0;
      break;

    case TOUCH_MOVE:
      MoveRel(Touch->move.x, Touch->move.y);
      sLeft = Left();
      sTop = Top();
      Touch->event = 0;
      break;

    case TOUCH_END:
      SetOpacity(OPACITY_MAX);
      Touch->event = 0;
      break;
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
  EvEditor::SelectObj(editObj);
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
    Sender->TextColor(editObj->Disp->SystemFont[Value] == 0 ? TEXT_ERROR : CL_BLACK);
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

static void OnTouchDotMove(EvEditDot *Sender, const EvTouchEvent *Touch)
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

static void OnTouchDotReSize(EvEditDot *Sender, const EvTouchEvent *Touch)
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
