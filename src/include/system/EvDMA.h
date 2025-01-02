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

#ifndef     _EV_DMA_H_
#define     _EV_DMA_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvDMA
{
  friend class    EvTaskDMA;

  public:
    enum {PENDING, BEGIN, LOADING, COMPLETED, ABORT};

    uint16_t        Status;
    EvDisplay       *Disp;
    const EvMem     *Dst;     // Is nullptr the destination is MEDIAFIFO
    uint8_t         *Src;
    uint32_t        Cnt;
    void            *Tag;
    void            *Sender;
    void            (*OnEvent)(EvDMA *Data);

  protected:
    EvDMA           *mNext;
    EvDMA           *mPrev;

    EvDMA(EvDisplay *Disp, uint8_t *Src, uint32_t Cnt, void *Tag, void *Sender, void (*OnEvent)(EvDMA *Data), EvMem *Dst);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvTaskDMA
{
  public:
    EvTaskDMA(void);

    void            Update(void);
    bool            Busy(void) { return mBusy; };
    void            Start(void) { mBusy = true; };
    bool            InQueues(void) { return (mInProgress != nullptr || mFirst != nullptr); };
    EvDMA           *Add(EvDisplay *Disp, uint8_t *Src, uint32_t Cnt, void *Tag = nullptr, void *Sender = nullptr, void (*OnEvent)(EvDMA *Data) = nullptr, EvMem *Dst = nullptr);

  protected:
    EvDMA           *mInProgress;
    EvDMA           *mFirst;
    EvDMA           *mLast;
    uint32_t        mInd;
    uint32_t        mCnt;
    volatile bool   mBusy;
    EventResponder  mEvent;

    static void     mOnEventDMA(EventResponderRef Event);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern EvTaskDMA  TaskDMA;

#endif  /* _EV_DMA_H_ */
