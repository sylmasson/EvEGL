
#ifndef     _EV_KEYBOARD_H_
#define     _EV_KEYBOARD_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define     BG_COLOR              RGB555(213, 215, 220)
#define     OVER_KEY_BD_COLOR     RGB555(179, 183, 192)
#define     STD_KEY_COLOR_UP      RGB555(255, 255, 255)
#define     STD_KEY_COLOR_DOWN    RGB555(255, 255, 255)
#define     CTRL_KEY_COLOR_UP     RGB555(179, 183, 192)
#define     CTRL_KEY_COLOR_DOWN   RGB555(255, 255, 255)
#define     SPACE_KEY_COLOR_UP    RGB555(255, 255, 255)
#define     SPACE_KEY_COLOR_DOWN  RGB555(179, 183, 192)
#define     TEXT_COLOR_UP         RGB555(  0,   0,   0)
#define     TEXT_COLOR_DOWN       RGB555(  0,   0,   0)

#define     SHIFT_KEY             1
#define     ALPHA_KEY             2
#define     NUM_KEY               3
#define     CLOSE_KEY             4
#define     SYM1_KEY              5
#define     SYM2_KEY              6
#define     BELL_KEY              7
#define     DEL_KEY               8
#define     TAB_KEY               9
#define     RETURN_KEY            10

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern EvKeyboard           			Keyboard480x230;
extern EvKeyboard           			Keyboard600x252;
extern EvKeyboard           			Keyboard800x220;
extern EvKeyboard           			Keyboard1024x276;
extern EvKeyboard           			Keyboard1077x276;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_KEYBOARD_H_ */
