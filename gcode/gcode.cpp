#include "gcode.h"

using namespace gcode;

uint8_t Handler::DecodeCommand(String cmdLine)
{
  String lCpy = removeComments(cmdLine);
  int pos = lCpy.indexOf(" ");
  if (pos == -1 || lCpy.length() == 0)
  {
    return 1;
  }
  int cmd = getGCode(lCpy.substring(0, pos));
  lCpy = lCpy.substring(pos+1, lCpy.length());

  Positioning::Point2D tempPoint;
  Positioning::Point tempPointZ;
  Positioning::Point2D tempArcCenter;
  Positioning::Feedrate tempFeedrate;
  //parse parameters
  while ((pos = lCpy.indexOf(' ')) != -1)
  {
    String p = lCpy.substring(0, pos+1);
    S_PARAM param = parseParam(p);
    if (param.p == 'x') { tempPoint.x = param.val; }
    if (param.p == 'y') { tempPoint.y = param.val; }
    if (param.p == 'z') { tempPointZ = param.val; }
    if (param.p == 'f') { tempFeedrate = param.val; }
    if (param.p == 'i') { tempArcCenter.x = param.val; }
    if (param.p == 'j') { tempArcCenter.y = param.val; }
    lCpy = lCpy.substring(pos+1);
  }

  mGType = Positioning::E_MOVEMENT::UNKNOWN;
  switch(cmd)
  {
    case 0:
    {
      mGType = Positioning::E_MOVEMENT::RAPID_MOVEMENT;
      mLinear.x = tempPoint.x;
      mLinear.y = tempPoint.y;
      mLinear.z = (tempPointZ != FLT_MAX ? tempPointZ : mLastPosition.z);
      mFeedrate = FR_RAPID_MOVEMENT;
    }
    break;
    case 1:
    {
      mGType = Positioning::E_MOVEMENT::LINEAR_INTERPOLATION;
      mLinear.x = tempPoint.x;
      mLinear.y = tempPoint.y;
      mLinear.z = (tempPointZ != FLT_MAX ? tempPointZ : mLastPosition.z);
      mFeedrate = tempFeedrate;
    }
    case 2:
    {
      mGType = Positioning::E_MOVEMENT::ARC;
      mArcDirection = Positioning::E_ARC_ROT::COUNTERCLOCKWISE;
      mArc.endPoint.x = tempPoint.x;
      mArc.endPoint.y = tempPoint.y;
      if (tempArcCenter.x != FLT_MAX) { mArc.centerPoint.x = tempArcCenter.x; }
      if (tempArcCenter.x != FLT_MAX) { mArc.centerPoint.x = tempArcCenter.x; }
    }
    break;
    case 3:
    {
      mGType = Positioning::E_MOVEMENT::ARC;
      mArcDirection = Positioning::E_ARC_ROT::CLOCKWISE;
      mArc.endPoint.x = tempPoint.x;
      mArc.endPoint.y = tempPoint.y;
      if (tempArcCenter.x != FLT_MAX) { mArc.centerPoint.x = tempArcCenter.x; }
      if (tempArcCenter.x != FLT_MAX) { mArc.centerPoint.x = tempArcCenter.x; }
    }
    break;
  }

  if (mGType != Positioning::E_MOVEMENT::UNKNOWN)
  {
    return 1;
  }
  return 0;
}

S_PARAM Handler::parseParam(String p)
{
  S_PARAM param;
  char * buff;
  p.toLowerCase();
  p.toCharArray(buff, p.length());

  sprintf(buff, "%c%f",param.p, param.val);

  return param;
}

int Handler::getGCode(String line)
{
  int cmd;
  char * buff;
  line.toCharArray(buff,line.length());
  sprintf(buff, "G%i", cmd);
  return cmd;
}


Positioning::E_MOVEMENT Handler::GetType()
{
  return mGType;
}

Positioning::Arc Handler::GetArc()
{
  return mArc;
}

Positioning::Point3D Handler::GetLinear()
{
  return mLinear;
}


Positioning::Feedrate Handler::GetFeedrate()
{
  return mFeedrate;
}

Positioning::E_ARC_ROT Handler::GetArcDirection()
{
  return mArcDirection;
}

Positioning::E_UNIT Handler::GetUnit()
{
  return mUnit;
}

String Handler::removeComments(String line)
{
  int lp = 0;
  if ((lp = line.indexOf('(')) == -1)
  {
    return line;
  }
  return line.substring(0, lp);
}
