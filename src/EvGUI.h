/// @file

#ifndef     _EV_GUI_H_
#define     _EV_GUI_H_

// Display constant

#define     DISP_MAX        4
#define     FREQ_REFRESH    60
#define     PERIOD_REFRESH  (1000000L / FREQ_REFRESH)   // usec

// Touch Event constant

#define     TOUCH_DONE      0
#define     TOUCH_START     1
#define     TOUCH_END       2
#define     TOUCH_MOVE      3
#define     TOUCH_HOLD      4
#define     TOUCH_REPEAT    5
#define     TOUCH_DOUBLE    6
#define     TOUCH_CANCEL    7

#define     HOLD_DELAY      800
#define     REPEAT_DELAY    150
#define     DOUBLE_DELAY    300

// Touch Status constant

#define     TOUCHING_FLAG   (1 << 0)
#define     REPEAT_FLAG     (1 << 1)
#define     DOUBLE_COUNT    (1 << 2)
#define     DOUBLE_FLAG     (1 << 3)
#define     MOVE_FLAG       (1 << 4)

// Object Status Flags constant

#define     VISIBLE_OBJ     (1 << 0)
#define     MODIFIED_OBJ    (1 << 1)
#define     DISABLED_OBJ    (1 << 2)
#define     MOVED_OBJ       (1 << 3)
#define     FIXED_OBJ       (1 << 4)
#define     ABS_OBJ         (1 << 5)
#define     FLOAT_OBJ       (1 << 6)
#define     CONTROL_OBJ     (1 << 7)
#define     MODIF_TEXT_OBJ  (1 << 8)
#define     FUNCT_USED_OBJ  (1 << 9)
#define  		FILTER_DIS_OBJ	(1 << 13)
#define     ABORT_OBJ       (1 << 14)
#define     SYSTEM_OBJ      (1 << 15) // reserved for GUI System

#define  		VISIBLE_DIS_OBJ	(VISIBLE_OBJ | DISABLED_OBJ)

// Shape constant

#define     ROUND_CORNERS   0
#define     RATIO_CORNERS   1
#define     FIXED_CORNERS   2
#define     SQUARE_CORNERS  3
#define     USER_CORNERS    4
#define     SHADOW          (1 << 3)  // Additionnal shadow if supported

// Alignment constant

#define     LEFT_TOP        0
#define     CENTER_TOP      1
#define     RIGHT_TOP       2
#define     JUSTIFY_TOP     3
#define     LEFT_CENTER     4
#define     CENTER_CENTER   5
#define     RIGHT_CENTER    6
#define     JUSTIFY_CENTER  7
#define     LEFT_BOTTOM     8
#define     CENTER_BOTTOM   9
#define     RIGHT_BOTTOM    10
#define     JUSTIFY_BOTTOM  11
#define     CENTER          5

#define  		ALIGNMENT_LOCK 	(1 << 7)

// EvSysFont constant

#define     FIRST_ROM_FONT  16
#define     LAST_ROM_FONT   34
#define     LAST_FONT       31
#define     FMB_SIZE        148

// EvScrollBar constant

#define     SCROLL_BAR_OFF  0
#define     SCROLL_BAR_ON   1
#define     SCROLL_BAR_AUTO 2

#define 		SCROLL_MOVING   (1 << 0)
#define  		SCROLL_TIMER    (1 << 1)

// EvTextCursor constant

#define     CURSOR_NONE     0
#define     CURSOR_SOLID    1
#define     CURSOR_BLINK    2
#define     CURSOR_SMOOTH   3

// EvKbd constant

#define     LAYOUT_ALPHA    0
#define     LAYOUT_NUM1     1
#define     LAYOUT_NUM2     2
#define     LAYOUT_KEYPAD   3
#define     LAYOUT_SHIFT    (1 << 6)
#define     LAYOUT_CAPS     (1 << 7)

// Trace flags constant

#define     TRACE_MODIFIED  (1 << 0)
#define     TRACE_TOUCH     (1 << 1)
#define     TRACE_FPS       (1 << 2)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct      EvView;
struct      EvTouchEvent;
struct      EvTextStyle;
struct      EvFont;

class       EvSysFont;
class       EvBitmap;
class       EvKinMove;
class       EvKinScroll;
class       EvStringList;

class       EvObj;
class       EvPanel;
class       EvDisplay;
class       EvScrollBar;
class       EvScrollBox;
class       EvTouchCal;
class       EvKbd;

class       EvLabel;
class       EvNumLabel;
class       EvNumInt;
class       EvButton;
class       EvSelector;
class       EvTab;
class       EvToggle;
class       EvProgress;
class       EvSlider;
class       EvCheckBox;
class       EvTextBlock;
class       EvTextBox;
class       EvTextCursor;
class       EvTerminal;
class       EvSpectrum;
class       EvPixelBox;
class       EvSmeter;
class       EvVuBar;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include    <SD.h>
#include    <Arduino.h>
#include    <include/system/EvEVE.h>

#include    <include/system/EvSysFont.h>
#include    <include/system/EvKinetic.h>
#include    <include/system/EvStringList.h>
#include    <include/system/EvObj.h>
#include    <include/system/EvPanel.h>
#include    <include/system/EvDisplay.h>
#include    <include/system/EvScrollBar.h>
#include    <include/system/EvScrollBox.h>
#include    <include/system/EvTouchCal.h>
#include    <include/system/EvKbd.h>

#include    <include/EvLabel.h>
#include    <include/EvNumLabel.h>
#include    <include/EvNumInt.h>
#include    <include/EvButton.h>
#include    <include/EvSelector.h>
#include    <include/EvToggle.h>
#include    <include/EvProgress.h>
#include    <include/EvSlider.h>
#include    <include/EvCheckBox.h>
#include  	<include/EvImage.h>
#include    <include/EvTextBlock.h>
#include    <include/EvTextBox.h>
#include    <include/EvTextCursor.h>
#include    <include/EvTerminal.h>
#include    <include/EvSmeter.h>
#include    <include/EvSpectrum.h>
#include    <include/EvPixelBox.h>
#include    <include/EvVideo.h>
#include    <include/EvVuBar.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern const char *OpenEditor(EvDisplay *Disp);
extern const char *CloseEditor(void);
extern void       EditorToFront(EvDisplay *Disp);
extern void       SetEditObj(EvObj *Obj);
extern void       SetEditObjDestroyed(EvObj *Obj);
extern void       ShellInput(const char byte);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_GUI_H_ */

