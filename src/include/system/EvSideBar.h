
#ifndef     _EV_SIDEBAR_H_
#define     _EV_SIDEBAR_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvSideBar : public EvPanel
{
  public:
    void          Open(void);
    void          Close(void);
    void          Setup(uint8_t Type, uint16_t Period = 220);
    void          Setup(int16_t Left, int16_t Top, uint8_t Type, uint16_t Period = 220);

  protected:
    EvSideBar(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvDisplay *Disp, const char *Tag = nullptr, uint16_t State = VISIBLE_DIS_OBJ);

    uint8_t       mType;
    int16_t       mOpenTop;
    int16_t       mOpenLeft;
    EvKinMove     mKinMotion;

    virtual void  refreshEvent(void);

  public:
    static EvSideBar  *Create(int16_t Left, int16_t Top, uint16_t Width, uint16_t Height, EvPanel *Dest, const char *Tag = nullptr, uint16_t State = VISIBLE_OBJ);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_SIDEBAR_H_ */
