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

#ifndef     _EV_TOUCH_H_
#define     _EV_TOUCH_H_

// Touch Event constant

#define     TOUCH_DONE      0
#define     TOUCH_START     1
#define     TOUCH_END       2
#define     TOUCH_MOVE      3
#define     TOUCH_HOLD      4
#define     TOUCH_REPEAT    5
#define     TOUCH_DOUBLE    6
#define     TOUCH_CANCEL    7

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * @brief     EvTouchEvent is the struct that contains the information of the Touch Event.
 * 
 * @subsection  TOUCH_EVENT List of Touch Event
 *
 * @tableofcontents
 *    Event     |                    Description
 * ------------ | ----------------------------------------------------
 * TOUCH_DONE   | No event occurs
 * TOUCH_START  | Triggered immediately upon touching the object
 * TOUCH_END    | Triggered after releasing the active object
 * TOUCH_MOVE   | Triggered when moving the touch position
 * TOUCH_HOLD   | Triggered when the hold touch time has expired
 * TOUCH_REPEAT | Triggered when the repeat touch time has expired
 * TOUCH_DOUBLE | Triggered when the double touch event is encountered
 * TOUCH_CANCEL | Triggered to force the release of the object
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

union EvTouchPos
{
  uint32_t    xy;
  struct
  {
    int16_t   y;
    int16_t   x;
  };
};

struct EvTouchEvent
{
  uint8_t             status;       ///< Status flags for the Touch Event process.
  mutable uint8_t     event;        ///< Current Touch Event. See @ref TOUCH_EVENT.
  uint8_t             tag;          ///< Tag value provided by the display.
  uint8_t             id;
  int16_t             x;            ///< Horizontal relative touch position to the active object.
  int16_t             y;            ///< Vertical relative touch position to the active object.
  EvTouchPos          abs;          ///< Horizontal and vertical absolute touch position.
  EvTouchPos          prev;         ///< Horizontal and vertical previous absolute touch position.
  mutable EvTouchPos  move;         ///< Horizontal and vertical moving touch position.
  EvObj               *obj;
  EvObj               *owner;
  uint32_t            timer;        ///< Timer value in msec since the object was initially touched.
  uint32_t            endTimer;     ///< Timestamp in msec when the finger is release.
  uint32_t            startTimer;   ///< Timestamp in msec when the object was initially touched.
  mutable uint32_t    repeatTimer;  ///< Timestamp in msec used for the next TOUCH_HOLD or TOUCH_REPEAT events.
  mutable uint16_t    repeatDelay;  ///< Time in msec defined between TOUCH_REPEAT events.
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTouch
{
  friend class EvDisplay;

  public:
    void          WaitForRelease(EvObj *Obj = nullptr);

  protected:
    EvTouch();

    void          update(EvDisplay *Disp);
    void          update(EvDisplay *Disp, uint8_t Id, EvTouchPos TouchPos, uint32_t msec);

  protected:
    EvTouchEvent  mTouchEvent[5];
    void          (*mOnTouch)(EvObj *Obj, const EvTouchEvent *Touch);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_TOUCH_H_ */
