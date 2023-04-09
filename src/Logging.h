#pragma once

#if not defined (LOG_LEVEL) || LOG_LEVEL == 0
    #define ERROR(...)
    #define INFO(...)
    #define DEBUG(...)
#elif LOG_LEVEL == 1
    #define ERROR(...)    Serial.printf(__VA_ARGS__)
    #define INFO(...)
    #define DEBUG(...)
#elif LOG_LEVEL == 2
    #define ERROR(...)    Serial.printf(__VA_ARGS__)
    #define INFO(...)     Serial.printf(__VA_ARGS__)
    #define DEBUG(...)
#elif LOG_LEVEL == 3
    #define ERROR(...)    Serial.printf(__VA_ARGS__)
    #define INFO(...)     Serial.printf(__VA_ARGS__)
    #define DEBUG(...)    Serial.printf(__VA_ARGS__)
#endif
