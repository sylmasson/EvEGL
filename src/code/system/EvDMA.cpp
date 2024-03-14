
#include    <EvGUI.h>

EvTaskDMA   TaskDMA;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvDMA::EvDMA(EvDisplay *Disp, uint8_t *Src, uint32_t Cnt, void *Tag, void *Sender, void (*OnEvent)(EvDMA *Data), EvMem *Dst)
{
  if (Dst == nullptr)
    Cnt = (Cnt + 3) & ~3;   // Padding 32bit alignment for MEDIAFIFO

  EvDMA::Status = EvDMA::PENDING;
  EvDMA::Disp = Disp;
  EvDMA::Dst = Dst;
  EvDMA::Src = Src;
  EvDMA::Cnt = Cnt;
  EvDMA::Tag = Tag;
  EvDMA::Sender = Sender;
  EvDMA::OnEvent = OnEvent;
  mNext = nullptr;
  mPrev = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvTaskDMA::EvTaskDMA(void)
{
  mBusy = false;
  mFirst = mLast = nullptr;
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
        EvOut->println("EvDMA ABORT");
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
      EvOut->println("Mediafifo FULL");
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
