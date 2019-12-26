#include "Logger.h"

 
Logger& Logger::operator<<(const String& msg)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        Serial.print(msg);
    }
    return *this;
}
 
Logger& Logger::operator<<(const char msg[])
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        Serial.print(msg);
    }
    return *this;
}
 
Logger& Logger::operator<<(char c)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        Serial.print(c);
    }
    return *this;
}
 
Logger& Logger::operator<<(unsigned char c)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        if (__format > 0) {
            Serial.print(c, __format);
        } else {
            Serial.print(c); /* DEC */
        }
    }
    return *this;
}
 
Logger& Logger::operator<<(int n)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        if (__format > 0) {
            Serial.print(n, __format);
        } else {
            Serial.print(n); /* DEC */
        }
    }
    return *this;
}
 
Logger& Logger::operator<<(unsigned int n)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        if (__format > 0) {
            Serial.print(n, __format);
        } else {
            Serial.print(n); /* DEC */
        }
    }
    return *this;
}
 
Logger& Logger::operator<<(long l)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        if (__format > 0) {
            Serial.print(l, __format);
        } else {
            Serial.print(l); /* DEC */
        }
    }
    return *this;
}
 
Logger& Logger::operator<<(unsigned long l)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        if (__format > 0) {
            Serial.print(l, __format);
        } else {
            Serial.print(l); /* DEC */
        }
    }
    return *this;
}
 
Logger& Logger::operator<<(double d)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        if (__format > 0) {
            Serial.print(d, __format);
        } else {
            Serial.print(d); /* BIN */
        }
    }
    return *this;
}
 
Logger& Logger::operator<<(const loglevel &lvl)
{
    __cur_log_level = lvl.level;
    return *this;
}
 
Logger& Logger::operator<<(const EndLog& el)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        Serial.println("");
    }
    __cur_log_level = DEFAULT_LOG_LEVEL;
    __format = -1;
    return *this;
}
 
Logger& Logger::operator<<(const FlushLog& fl)
{
    if (__cur_log_level > DEFAULT_LOG_LEVEL &&
        __log_level >= __cur_log_level)
    {
        Serial.flush();
    }
    __cur_log_level = DEFAULT_LOG_LEVEL;
    __format = -1;
    return *this;
}
 
Logger& Logger::operator<<(const format& fmt)
{
    switch (fmt.type)
    {
        case DEC:
        case HEX:
        case OCT:
        case BIN:
            __format = fmt.type;
            break;
        default:
            __format = -1;
            break;
    }
    return *this;
}
