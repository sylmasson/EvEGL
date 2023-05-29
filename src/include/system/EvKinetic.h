
#ifndef     _EV_KINETIC_H_
#define     _EV_KINETIC_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvKinMove
{
  public:
    EvKinMove(uint16_t Period = 320);

    void          Setup(uint16_t Period = 320);
    void          Start(int16_t Pixels);
    bool          IsEnabled(void);
    void          Stop(void);
    int16_t       Value(void);

  protected:
    uint32_t      mScale;
    uint16_t      mTotal;
    uint16_t      mPixels;
    uint16_t      mValue;
    uint8_t       mPeriod;
    uint8_t       mIndex;
    bool          mNegative;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvKinScroll
{
  public:
    EvKinScroll(uint8_t Speed = 16, uint16_t Period = 640);

    void          Setup(uint8_t Speed, uint16_t Period = 640);
    void          Start(int16_t Velocity);
    bool          IsEnabled(void);
    void          Stop(void);
    int16_t       Value(void);

  protected:
    int16_t       mValue;
    uint16_t      mSpeed;
    int16_t       mVelocity;
    uint8_t       mPeriod;
    uint8_t       mIndex;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_KINETIC_H_ */
