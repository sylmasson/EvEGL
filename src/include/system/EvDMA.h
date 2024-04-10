
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
    bool            Empty(void) { return (mFirst == nullptr); };
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

#endif  /* _EV_DMA_H_ */
