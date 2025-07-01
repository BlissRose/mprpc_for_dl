#include "logger.h"      // 引入日志系统头文件，包含 Logger 类定义和日志级别枚举
#include <time.h>        // 提供时间相关函数，用于生成日志时间戳
#include <iostream>      // 标准输入输出流，用于错误信息输出

// 获取日志系统的单例实例
// 使用静态局部变量实现线程安全的懒汉式单例模式
Logger& Logger::GetInstance()
{
    static Logger logger; // 静态局部变量，只会被初始化一次
    return logger;        // 返回唯一的 Logger 实例
}

// 构造函数：初始化日志系统并启动写日志线程
Logger::Logger()
{
    // 启动一个子线程专门用于写日志到文件，lamba 函数表达式
    std::thread writeLogTask([&](){
        for (;;)
        {
            // 获取当前时间
            time_t now = time(nullptr);           // 获取当前时间戳（秒）
            tm *nowtm = localtime(&now);          // 转换为本地时间结构体

            // 生成日志文件名，格式为年-月-日-log.txt
            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);

            // 以追加方式打开日志文件（a+ 表示读写并追加）
            FILE *pf = fopen(file_name, "a+");
            if (pf == nullptr) // 如果文件打开失败
            {
                std::cout << "logger file : " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE); // 输出错误信息并退出程序
            }

            // 从线程安全队列中取出一条日志消息
            std::string msg = m_lckQue.Pop();

            // 构建时间戳前缀字符串
            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d =>[%s] ", 
                    nowtm->tm_hour, 
                    nowtm->tm_min, 
                    nowtm->tm_sec,
                    (m_loglevel == INFO ? "info" : "error")); // 添加日志级别标识

            // 将时间戳插入到日志消息开头，并添加换行符
            msg.insert(0, time_buf);
            msg.append("\n");

            // 写入日志文件
            fputs(msg.c_str(), pf); // 将日志内容写入文件
            fclose(pf);               // 关闭文件指针
        }
    });

    // 设置线程为分离状态，使其在后台独立运行，守护线程
    writeLogTask.detach();
}

// 设置日志输出级别
// level: INFO 或 ERROR，控制是否输出某些级别的日志
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level; // 更新当前日志级别
}

// 将日志消息推入线程安全队列，供后台线程异步写入文件
// msg: 要记录的日志内容
void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg); // 将日志消息加入线程安全队列
}