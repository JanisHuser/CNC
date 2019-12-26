#ifndef __CONFIG_H__
#define __CONFIG_H__



#include "Helpers/Logger.h"

#define LOG_WARNING(msg) Logger::get_logger().log(Logger::WARNING_LOG_LEVEL) << "WARNING: " << msg << endlog;
#define LOG_ERROR(msg) Logger::get_logger().log(Logger::ERROR_LOG_LEVEL) << "ERROR: " << msg << endlog;
#define LOG(msg)  Logger::get_logger().log(Logger::APPLICATION_LOG_LEVEL) <<"APP: " << msg << endlog;
#define LOG_VERBOSE(msg)  Logger::get_logger().log(Logger::VERBOSE_LOG_LEVEL) << "VERBOSE: " << msg << endlog;





namespace Config
{
  const unsigned char SD_CARD_CHIP_SELECT = 53;
  const unsigned char DRIVERS_STEP_PIN = 13;
  const unsigned char SD_CARD_ADDRESS = 0x20;

  const unsigned char SD_CARD_LEN_REG = 0xFE;
};



namespace STATUS
{
  enum class SD_CARD
  {
    INIT_SUCCESS = 0x01,
    INIT_FAILED = 0x02
  };
};
#endif // !__CONFIG_H__