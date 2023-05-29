
#ifndef     _EV_BITMAP_H_
#define     _EV_BITMAP_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvBitmap
{
  struct XY
  {
    int16_t     x;
    int16_t     y;
  };

  public:
    EvBitmap(const EvBmp *mBmp);

    void          Unload(void);
    bool          Load(EvDisplay *Disp);
    bool          Setup(int16_t &X, int16_t &Y, int16_t Angle = 0, int32_t ScaleX = 65536, int32_t ScaleY = 0);
    bool          Draw(int16_t X, int16_t Y, int16_t Angle = 0, int32_t Scale = 65536);
    void          Vertex2f(int16_t X, int16_t Y);
    void          Center(int16_t X, int16_t Y);
    void          Align(uint8_t Align);

/*    bool          LoadIdentity(void);
    bool          Resize(uint16_t Width, uint16_t Height);
    bool          Rotate2f(int16_t X, int16_t Y, int16_t Angle);
    bool          Draw2f(int16_t X, int16_t Y);
    bool          Draw(int16_t X, int16_t Y); */

  protected:
    XY            mOffset;
    XY            mCenter;
    const EvBmp   *mBmp;

  public:
    const EvMem   *mDest;
    EvDisplay     *Disp;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_BITMAP_H_ */
