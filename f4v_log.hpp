//
// Created by chnvideo on 2016/2/2.
//

#ifndef F4V_PARSEER_F4V_LOG_HPP
#define F4V_PARSEER_F4V_LOG_HPP

#include <stdio.h>

class F4vLog;

extern F4vLog* _f4v_log;

#define f4v_info(msg, ...)    _f4v_log->info( __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define f4v_trace(msg, ...)    _f4v_log->trace( __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define f4v_warn(msg, ...)    _f4v_log->warn( __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define f4v_error(msg, ...)    _f4v_log->error( __FILE__, __LINE__, msg, ##__VA_ARGS__)

class LEVEL{
public:
    static const int info = 0x01;
    static const int trace = 0x02;
    static const int warn = 0x03;
    static const int error = 0x04;
};

class F4vLog{
private:
    int pid;
    int level;
public:
    F4vLog();
    virtual ~F4vLog();

public:
    bool generate_header(char** head, const char* file, int line, const char* level, int& size); 
public:
    void info(const char *file, int line, const char* msg, ...);
    void trace(const char *file, int line, const char* msg, ...);
    void warn(const char *file, int line, const char* msg, ...);
    void error(const char *file, int line, const char* msg, ...);
};

#endif //F4V_PARSEER_F4V_LOG_HPP
