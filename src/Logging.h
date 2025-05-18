#pragma once

#if defined (LOG_LEVEL) && LOG_LEVEL > 0

// define a LOG_CALLBACK function which directs logging to Serial, or to MQTT, or whatever. For example:
// void LOG_CALLBACK(const char* txt) {  Serial.println(txt); }
extern void LOG_CALLBACK(const char *);

static void __FORMAT_LOG(const char* m, ...) { 
    char *logmsg;
    va_list args;
    va_start(args, m);
    vasprintf(&logmsg, m, args);
    if (logmsg[strlen(logmsg)-1] == '\n')   // remove trailing newlines
        logmsg[strlen(logmsg)-1] = 0;
    LOG_CALLBACK(logmsg);
    free(logmsg);
    va_end(args);
};
#endif

#if not defined (LOG_LEVEL) || LOG_LEVEL == 0
    #define ERROR(...)
    #define INFO(...)
    #define DEBUG(...)
#elif LOG_LEVEL == 1
    #define ERROR(...)    __FORMAT_LOG(__VA_ARGS__)
    #define INFO(...)
    #define DEBUG(...)
#elif LOG_LEVEL == 2
    #define ERROR(...)    __FORMAT_LOG(__VA_ARGS__)
    #define INFO(...)     __FORMAT_LOG(__VA_ARGS__)
    #define DEBUG(...)
#elif LOG_LEVEL == 3
    #define ERROR(...)    __FORMAT_LOG(__VA_ARGS__)
    #define INFO(...)     __FORMAT_LOG(__VA_ARGS__)
    #define DEBUG(...)    __FORMAT_LOG(__VA_ARGS__)
#endif


