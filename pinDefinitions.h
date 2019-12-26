#ifndef __PIN_DEFINITIONS_H__
#define __PIN_DEFINITIONS_H__

#include "driver.h"
using Drivers::PIN_FUNC;
using Drivers::C_Driver;

namespace PIN_DEFS
{

  
  #define DRIVER_X1_TABLE T_PIN(PIN_FUNC::ENABLE, 19) \
                          T_PIN(PIN_FUNC::DIRECTION, 11) \
                          T_PIN(PIN_FUNC::RESET, 15) \
                          T_PIN(PIN_FUNC::SLEEP, 14) \
                          T_PIN(PIN_FUNC::M0, 14) \
                          T_PIN(PIN_FUNC::M1, 18) \
                          T_PIN(PIN_FUNC::M2, 17) \
                          T_PIN(PIN_FUNC::STEP_CTRL, 10) \
                          T_PIN(PIN_FUNC::FAULT, 13)

  #define DRIVER_X2_TABLE T_PIN(PIN_FUNC::ENABLE, 30) \
                          T_PIN(PIN_FUNC::DIRECTION, 23) \
                          T_PIN(PIN_FUNC::RESET, 26) \
                          T_PIN(PIN_FUNC::SLEEP, 27) \
                          T_PIN(PIN_FUNC::M0, 31) \
                          T_PIN(PIN_FUNC::M1, 28) \
                          T_PIN(PIN_FUNC::M2, 29) \
                          T_PIN(PIN_FUNC::STEP_CTRL, 24) \
                          T_PIN(PIN_FUNC::FAULT, 25)



  void setDriverX1(C_Driver driver)
  {
    #define T_PIN(func, pin) \
      driver.setPin(func,pin);
     DRIVER_X1_TABLE
    #undef T_PIN
  }

  void setDriverX2(C_Driver driver)
  {
    #define T_PIN(func, pin) \
      driver.setPin(func,pin);
     DRIVER_X2_TABLE
    #undef T_PINs
  }

  void setDriverY1(C_Driver driver)
  {

  }

  void setDriverZ1(C_Driver driver)
  {
    
  }

  //
  #define DRIVERS_TABLE T_ENTRY("X1", 0, setDriverX1, 50, 250, 8, true, true, true) \
                        T_ENTRY("X2", 1, setDriverX2, 50, 250, 8, true, true, true) \
                        T_ENTRY("Y2", 2, setDriverY1, 50, 250, 8, true, true, true) \
                        T_ENTRY("Z1", 3, setDriverZ1, 50, 250, 8, true, true, true)
                      
/*
                      


  #define DRIVER_X1_EN        9
  #define DRIVER_X1_DIR       45
  #define DRIVER_X1_RST       4
  #define DRIVER_X1_SLP       3
  #define DRIVER_X1_M1        8
  #define DRIVER_X1_M2        7
  #define DRIVER_X1_M3        5
  #define DRIVER_X1_STP       45
  #define DRIVER_X1_FLT       2

  */
  
};

#endif //!__PIN_DEFINITIONS_H__
