#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>


/**
 * 显示参数帮助信息的函数
 * 当命令行参数不正确时调用此函数，向用户展示正确的命令行参数格式
 */
void ShowArgsHelp()
{
    std::cout<<"format: command -i <configfile>" << std::endl;
}

/**
 * 初始化应用程序的函数
 * @param argc 命令行参数的个数
 * @param argv 命令行参数的数组
 * 该函数通过解析命令行参数来获取配置文件的路径，并加载配置文件
 */
void MprpcApplication::Init(int argc, char **argv)
{
    //判断参数个数，是三个参数
    if (argc < 2)
    {
        ShowArgsHelp();//输出提示信息
        exit(EXIT_FAILURE);
    }

    int c = 0; // 用于存储 getopt 返回的命令行参数标识符
    std::string config_file; // 存储解析得到的配置文件路径

    // 使用 getopt 解析命令行参数，"i:" 表示需要一个参数值的选项 -i
    while ((c = getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg; // 获取 -i 参数后的配置文件路径
            break;
        case '?':
            ShowArgsHelp(); // 用户输入了无效参数，显示帮助信息
            exit(EXIT_FAILURE);
        case ':':
            ShowArgsHelp(); // 用户未给参数指定值，显示帮助信息
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    // 开始加载配置文件了 rpcserver_ip=  rpcserver_port   zookeeper_ip=  zookepper_port=
    m_config.LoadConfigFile(config_file.c_str());

    std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
    std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;
}

/**
 * 获取MprpcApplication类的单例实例
 * @return MprpcApplication类的引用
 * 该函数实现了单例模式，确保在整个应用程序中只有一个MprpcApplication实例，懒汉模式
 */
MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

// MprpcApplication类的静态成员变量m_config，用于存储配置信息
// 类内定义，类外初始化，因为静态成员变量必须在类外初始化
MprpcConfig MprpcApplication::m_config;
MprpcConfig& MprpcApplication::GetConfig()
{

    return m_config;
}