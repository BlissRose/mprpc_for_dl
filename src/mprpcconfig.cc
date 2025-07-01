#include "mprpcconfig.h" // 引入当前类的头文件，声明了 MprpcConfig 类的相关接口

#include <iostream>     // 标准输入输出流库，用于控制台输出
#include <string>       // C++标准库字符串类，用于处理字符串操作

// 负责解析加载配置文件
void MprpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file, "r"); // 以只读模式打开配置文件
    if (nullptr == pf) // 判断文件是否成功打开
    {
        std::cout << config_file << " is note exist!" << std::endl; // 输出错误信息，提示文件不存在
        exit(EXIT_FAILURE); // 文件未找到，程序异常退出
    }

    // 1.跳过注释行   2.提取合法配置项 key=value 形式    3.去掉前后多余的空格
    while (!feof(pf)) // 循环读取文件内容直到文件结束
    {
        char buf[512] = {0}; // 定义一个缓冲区，用于存储每次读取的一行数据
        fgets(buf, 512, pf); // 从文件中读取最多 511 字节的内容到 buf 中

        // 将原始字符串转换为 std::string 并去除前导和尾随空格
        std::string read_buf(buf);
        Trim(read_buf); // 调用 Trim 方法去除首尾空格

        // 判断是否是注释行或空行，如果是则跳过
        if (read_buf[0] == '#' || read_buf.empty())
        {
            continue; // 跳过当前循环，继续下一行
        }

        // 解析配置项 key=value 的格式
        int idx = read_buf.find('='); // 查找等号 '=' 的位置
        if (idx == -1) // 如果找不到等号，说明配置项不合法
        {
            // 配置项不合法，跳过
            continue;
        }

        std::string key; // 存储配置项的键
        std::string value; // 存储配置项的值
        key = read_buf.substr(0, idx); // 提取等号前面的部分作为 key
        Trim(key); // 去除 key 前后的空格

        // rpcserverip=127.0.0.1\n 示例：查找换行符的位置以截取 value
        int endidx = read_buf.find('\n', idx); // 从等号之后查找换行符
        value = read_buf.substr(idx + 1, endidx - idx - 1); // 截取等号后到换行前的内容作为 value
        Trim(value); // 去除 value 前后的空格
        m_configMap.insert({key, value}); // 将解析出的 key-value 插入配置映射表中
    }

    fclose(pf); // 关闭文件指针，释放资源
}

// 查询指定 key 对应的配置值
std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configMap.find(key); // 在配置 map 中查找指定的 key
    if (it == m_configMap.end()) // 如果没有找到对应的 key
    {
        return ""; // 返回空字符串表示未找到
    }
    return it->second; // 返回找到的配置值
}

// 去掉字符串前后的空格
void MprpcConfig::Trim(std::string &src_buf)
{
    int idx = src_buf.find_first_not_of(' '); // 查找第一个非空格字符的位置
    if (idx != -1) // 如果找到了非空格字符（即字符串不是全空格）
    {
        // 说明字符串前面有空格
        src_buf = src_buf.substr(idx, src_buf.size() - idx); // 从第一个非空格字符开始截取字符串
    }

    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' '); // 查找最后一个非空格字符的位置
    if (idx != -1) // 如果找到了非空格字符
    {
        // 说明字符串后面有空格
        src_buf = src_buf.substr(0, idx + 1); // 截取到最后一个非空格字符为止
    }
}