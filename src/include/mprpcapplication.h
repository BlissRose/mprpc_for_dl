#pragma once  // 防止头文件重复包含

#include "mprpcconfig.h"        // 引入配置类头文件
#include "mprpcchannel.h"       // 引入 RPC 通信通道类头文件
#include "mprpccontroller.h"    // 引入控制器类头文件

// mprpc框架的基础类，负责框架的一些初始化操作
class MprpcApplication
{
public:
    // char **argv：是一个指向字符指针数组的指针，用于存储每个参数的具体值。
    static void Init(int argc, char **argv);  // 初始化应用程序，加载配置文件并解析命令行参数
    
    static MprpcApplication &GetInstance();  // 获取单例实例

    static MprpcConfig &GetConfig();         // 获取配置对象引用

private:
    static MprpcConfig m_config;  // 静态配置对象，用于存储全局配置信息

    MprpcApplication() {}                        // 私有构造函数，防止外部创建实例
    MprpcApplication(const MprpcApplication &) = delete;  // 禁用拷贝构造函数
    MprpcApplication(MprpcApplication &&) = delete;        // 禁用移动构造函数
};


