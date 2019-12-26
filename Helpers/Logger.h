#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <Arduino.h>

 
/**
 * marker to start log message
 */
struct loglevel {
    uint8_t level;
    loglevel(uint8_t l): level(l) {}
};
 
/**
 * marker to end a log message
 * (prints newline and resets log level)
 */
struct EndLog {};
const EndLog endlog = EndLog();
 
/**
 * marker to end a log message
 * (flushes output stream and resets log level)
 */
struct FlushLog {};
const FlushLog flushlog = FlushLog();
 
/**
 * marker to modify output stream
 */
struct format {
    int8_t type;
    format(int8_t fmt = -1): type(fmt) {}
};
 
class Logger {
public:
    static const uint8_t DEFAULT_LOG_LEVEL =        0; /* nothing printed */
    static const uint8_t APPLICATION_LOG_LEVEL =    1; /* std::cout */
    static const uint8_t ERROR_LOG_LEVEL =          2; /* + error messages */
    static const uint8_t WARNING_LOG_LEVEL =        3; /* + warning messages */
    static const uint8_t VERBOSE_LOG_LEVEL =        4; /* + verbose messages */
 
    static Logger& get_logger()
    {
        static Logger logger;
        if (!logger.__initialized) {
            Serial.begin(115200);
            logger.__initialized = true;
        }
        return logger;
    };
    ///< returns singleton instance of Logger
    ///< NOTE: pin 1 reserved
    ///< NOTE: if Logger is used as GLOBAL variable, the initialization
    ///<     code will fail to properly set Serial object, thus
    ///<  you will have to add proper code in your Setup()
    ///<  method to make it work (e.g. Serial.begin(4800);)
 
    uint8_t get_log_level() const { return __log_level; };
    void set_log_level(uint8_t ll) { __log_level = ll; };
 
    Logger& operator<<(const String&);
    Logger& operator<<(const char[]);
    Logger& operator<<(char);
    Logger& operator<<(unsigned char);
    Logger& operator<<(int);
    Logger& operator<<(unsigned int);
    Logger& operator<<(long);
    Logger& operator<<(unsigned long);
    Logger& operator<<(double);
 
    Logger& operator<<(const loglevel &lvl);
    Logger& operator<<(const EndLog&);
    Logger& operator<<(const FlushLog&);
    Logger& operator<<(const format& fmt);
 
    inline Logger& log(uint8_t log_level = 4) {
        *this << loglevel(log_level);
        return *this;
    }
    ///< alternative to << loglevel(value)
 
private:
    uint8_t __log_level;
    ///< the log level
 
    uint8_t __cur_log_level;
    ///< level of current message
 
    int8_t __format;
    ///< format with which print data
 
    bool __initialized = false;
    ///< resources have been loaded
 
    /* disabled: singleton class */
    Logger(): __log_level(DEFAULT_LOG_LEVEL),
            __cur_log_level(DEFAULT_LOG_LEVEL),
            __format(-1),
            __initialized(false) {};
    Logger(const Logger& other);
    Logger& operator =(const Logger& other);
};

 
#endif // !__LOGGER_H__