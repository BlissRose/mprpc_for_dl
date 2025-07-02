#pragma once // 防止头文件重复包含

#include <queue>         // 使用标准库队列容器
#include <thread>        // 支持多线程操作
#include <mutex>         // 提供互斥锁 std::mutex 和自动加锁工具
#include <condition_variable> // 提供条件变量，用于线程间同步

// 异步写日志的日志队列模板类

// 适用于生产者-消费者模型：多个线程写入（Push），一个线程读取（Pop）
template<typename T>

class LockQueue
{
public:
    /**
     * 向队列中插入一个元素
     * @param data 要插入的数据（const引用避免拷贝）
     * 多个线程可能同时调用此方法，因此需要加锁保护
     */
    void Push(const T &data)
    {
        std::lock_guard<std::mutex> lock(m_mutex); // 自动加锁，离开作用域自动解锁
        m_queue.push(data); // 将数据压入队列
        m_condvariable.notify_one(); // 唤醒一个等待的 Pop 线程，表示有新数据到来
    }

    /**
     * 从队列中取出一个元素
     * @return 队列前端的元素
     * 只有一个线程会调用此方法（如日志写入线程）
     */
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 加锁，支持手动控制生命周期

        // 如果队列为空，进入等待状态，直到被 notify_one 唤醒
        while (m_queue.empty())
        {
            m_condvariable.wait(lock); // 条件变量等待通知，自动释放锁并挂起线程
        }

        T data = m_queue.front(); // 获取队列头部元素
        m_queue.pop(); // 移除该元素
        return data; // 返回获取的数据
    }

private:
    std::queue<T> m_queue;              // 底层使用的标准库队列容器
    std::mutex m_mutex;                 // 互斥锁，保证线程安全访问队列
    std::condition_variable m_condvariable; // 条件变量，用于线程同步
};