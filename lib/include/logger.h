#pragma once
#include "lockqueue.h"
#include <string>
/*

Logger 模块是 Mprpc 框架中的日志系统核心组件，其主要作用是为整个 RPC 框架提供线程安全、可控制级别的日志记录功能。它帮助开发者在开发、调试和运维阶段获取程序运行状态、错误信息以及性能数据。

Logger 模块是一个轻量级但功能完整的日志系统，具备以下特点：

特性	描述
单例模式	全局唯一，方便统一管理
线程安全	多线程环境下无竞争风险
日志级别控制	支持 INFO 和 ERROR 级别过滤
宏接口友好	使用简单，支持格式化输出
异步写入能力	高效且不影响主业务逻辑
它是构建稳定、可维护的 RPC 框架的重要组成部分，尤其适用于服务端长时间运行、需要日志追踪与故障排查的场景。
*/

// 定义宏 LOG_INFO("xxx %d %s", 20, "xxxx")
#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    } while(0) \

#define LOG_ERR(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    } while(0) \

// 定义日志级别
enum LogLevel
{
    INFO,  // 普通信息
    ERROR, // 错误信息
};

// Mprpc框架提供的日志系统
class Logger
{
public:
    // 获取日志的单例
    static Logger& GetInstance();
    // 设置日志级别 
    void SetLogLevel(LogLevel level);
    // 写日志
    void Log(std::string msg);
private:
    int m_loglevel; // 记录日志级别
    LockQueue<std::string>  m_lckQue; // 日志缓冲队列
    // 设置为单例模式
    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};