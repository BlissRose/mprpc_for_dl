#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

// 全局的watcher观察器   zkserver给zkclient的通知
/**
 * global_watcher是一个ZooKeeper的全局观察者回调函数。
 * 它的作用是监听ZooKeeper会话事件，并在连接成功时通知等待的线程。
 *
 * @param zh ZooKeeper句柄，代表和ZooKeeper服务器的连接。
 * @param type 事件类型，表示发生了什么类型的事件。
 * @param state 会话状态，表示当前会话的状态。
 * @param path 触发事件的节点路径，对于会话事件来说这个参数没有意义。
 * @param watcherCtx 观察者上下文，可以是任何用户定义的数据。
 */
void global_watcher(zhandle_t *zh, int type,
					int state, const char *path, void *watcherCtx)
{
	// 检查事件类型是否为会话事件
	if (type == ZOO_SESSION_EVENT) // 回调的消息类型是和会话相关的消息类型
	{
		// 检查会话状态是否为已连接
		if (state == ZOO_CONNECTED_STATE) // zkclient和zkserver连接成功
		{
			// 获取信号量，这是在ZooKeeper连接初始化时设置的
			sem_t *sem = (sem_t *)zoo_get_context(zh);
			// 释放信号量，通知等待的线程ZooKeeper连接已经成功
			sem_post(sem);
		}
	}
}

ZkClient::ZkClient() : m_zhandle(nullptr)
{
}

ZkClient::~ZkClient()
{
	if (m_zhandle != nullptr)
	{
		zookeeper_close(m_zhandle); // 关闭句柄，释放资源  MySQL_Conn
	}
}

// 启动Zookeeper客户端连接
void ZkClient::Start()
{
	// 从配置文件中加载Zookeeper服务器的IP地址和端口号
	std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
	std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
	// 拼接IP地址和端口号，形成连接字符串
	std::string connstr = host + ":" + port;

	/*
	zookeeper_mt：多线程版本
	zookeeper的API客户端程序提供了三个线程
	API调用线程
	网络I/O线程  pthread_create  poll
	watcher回调线程 pthread_create
	*/
	// 初始化Zookeeper句柄并建立连接
	m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
	if (nullptr == m_zhandle)
	{
		// 如果句柄为空，说明连接失败，输出错误信息并退出程序
		std::cout << "zookeeper_init error!" << std::endl;
		exit(EXIT_FAILURE);
	}

	// 创建信号量用于同步操作
	sem_t sem;
	sem_init(&sem, 0, 0);
	// 设置Zookeeper句柄的上下文为信号量地址
	zoo_set_context(m_zhandle, &sem);

	// 等待信号量，用于阻塞线程直到Zookeeper连接成功
	sem_wait(&sem);
	// 连接成功后输出成功信息
	std::cout << "zookeeper_init success!" << std::endl;
}

void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
	char path_buffer[128];
	int bufferlen = sizeof(path_buffer);
	int flag;
	// 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了
	flag = zoo_exists(m_zhandle, path, 0, nullptr);
	if (ZNONODE == flag) // 表示path的znode节点不存在
	{
		// 创建指定path的znode节点了
		flag = zoo_create(m_zhandle, path, data, datalen,
						  &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
		if (flag == ZOK)
		{
			std::cout << "znode create success... path:" << path << std::endl;
		}
		else
		{
			std::cout << "flag:" << flag << std::endl;
			std::cout << "znode create error... path:" << path << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

// 根据指定的path，获取znode节点的值
std::string ZkClient::GetData(const char *path)
{
	char buffer[64];
	int bufferlen = sizeof(buffer);
	int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
	if (flag != ZOK)
	{
		std::cout << "get znode error... path:" << path << std::endl;
		return "";
	}
	else
	{
		return buffer;
	}
}