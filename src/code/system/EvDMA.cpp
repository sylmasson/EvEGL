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

#include    <EvEGL.h>

EvTaskDMA   TaskDMA;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDMA::EvDMA(EvDisplay *Disp, uint8_t *Src, uint32_t Cnt, void *Tag, void *Sender, void (*OnEvent)(EvDMA *Data), EvMem *Dst) :
  Status(EvDMA::PENDING),
  Disp(Disp),
  Dst(Dst),
  Src(Src),
  Cnt(Cnt),
  Tag(Tag),
  Sender(Sender),
  OnEvent(OnEvent),
  mNext(nullptr),
  mPrev(nullptr)
{
  if (Dst == nullptr)
    EvDMA::Cnt = (Cnt + 3) & ~3;   // Padding 32bit alignment for MEDIAFIFO
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTaskDMA::EvTaskDMA(void) :
  mInProgress(nullptr),
  mFirst(nullptr),
  mLast(nullptr),
  mInd(0),
  mCnt(0),
  mBusy(false)
{
  mEvent.attach(mOnEventDMA);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTaskDMA::Update(void)
{
  uint8_t   *src;
  uint32_t  dst, cnt, free;

  if (Busy())
    return;

  while (mInProgress == nullptr)
  {
    if ((mInProgress = mFirst) == nullptr)
      return;

    if ((mFirst = mInProgress->mNext) != nullptr)
      mFirst->mPrev = nullptr;
    else
      mLast = nullptr;

    mInd = mCnt = 0;

    if (mInProgress->OnEvent != nullptr)
    {
      mInProgress->Status = EvDMA::BEGIN;
      mInProgress->OnEvent(mInProgress);

      if (mInProgress->Status == EvDMA::ABORT)
      {
        EvErr->println("EvDMA ABORT");
        delete mInProgress;
        mInProgress = nullptr;
      }
    }
  }

  src = mInProgress->Src + mInd;
  cnt = mInProgress->Cnt - mInd;

  if (mInProgress->Dst != nullptr)
    dst = mInProgress->Dst->addr + mInd;
  else
  { // transfer to MEDIAFIFO
    if ((free = mInProgress->Disp->MediaFifoFree(dst)) == 0)
    {
      EvErr->println("Mediafifo FULL");
      return;
    }

    if (cnt > free) 
      cnt = free;
  }

  mCnt = (cnt > 4096) ? 4096 : cnt;
  mInProgress->Status = EvDMA::LOADING;
//  EvOut->printf("dst=%08x src=%08x cnt=%u  %08x\n", dst, src, mCnt, mInProgress->Disp->rd32(REG_MEDIAFIFO_READ));
  mInProgress->Disp->wrDataDMA(dst, src, mCnt, mEvent);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDMA       *EvTaskDMA::Add(EvDisplay *Disp, uint8_t *Src, uint32_t Cnt, void *Tag, void *Sender, void (*OnEvent)(EvDMA *Data), EvMem *Dst)
{
  EvDMA     *node = nullptr;

  if (Disp != nullptr && Src != nullptr && Cnt > 0)
    node = new EvDMA(Disp, Src, Cnt, Tag, Sender, OnEvent, Dst);

  if (node != nullptr)
  {
    if (mFirst == nullptr)
      mFirst = mLast = node;
    else
    {
      node->mPrev = mLast;
      mLast->mNext = node;
      mLast = node;
    }
  }

  return node;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvTaskDMA::mOnEventDMA(EventResponderRef Event)
{
  EvDMA     *inProgress = TaskDMA.mInProgress;

  if (TaskDMA.Busy())
  {
    digitalWrite(5, LOW);
    TaskDMA.mBusy = false;
    TaskDMA.mInd += TaskDMA.mCnt;

    if (inProgress->Dst == nullptr)
      inProgress->Disp->MediaFifoUpdate(TaskDMA.mCnt);

    if (TaskDMA.mInd >= inProgress->Cnt)
    {
      if (inProgress->OnEvent != nullptr)
      {
        inProgress->Status = EvDMA::COMPLETED;
        (*inProgress->OnEvent)(inProgress);
      }

      delete inProgress;
      TaskDMA.mInProgress = nullptr;
    }

    TaskDMA.Update();
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
