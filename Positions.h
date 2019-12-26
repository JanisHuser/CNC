#ifndef __POSITION_H__
#define __POSITION_H__

#define FLT_MAX1 3.402823466e+38F /* max value */
#define FLT_MAX 450.0F

namespace Positioning
{
  #define CONV_INCH2MM(IMPERIAL) (IMPERIAL) * 25.4
  #define FR_RAPID_MOVEMENT FLT_MAX

  typedef float Point;
  typedef float Feedrate;

  typedef struct Point2D
  {
    Point x = FLT_MAX;
    Point y = FLT_MAX;
  } Point2D;
  typedef struct Point3D
  {
    Point x = FLT_MAX;
    Point y = FLT_MAX;
    Point z = FLT_MAX;
  } Point3D;

  typedef struct Arc
  {
    Point2D endPoint;
    Point2D centerPoint;
  } Arc;

  enum class E_UNIT
  {
    MM,
    INCH
  };

  enum class E_MOVEMENT
  {
    UNKNOWN,
    ARC,
    LINEAR_INTERPOLATION,
    RAPID_MOVEMENT,
  };
  enum class E_ARC_ROT
  {
    COUNTERCLOCKWISE,
    CLOCKWISE
  };
}


#endif // !__POSITION_H__