#include "mprpccontroller.h"

/*
MprpcController 模块在 RPC（Remote Procedure Call，远程过程调用）框架中通常用于控制和管理一次 RPC 调用的行为，包括错误处理、取消操作等功能。它是客户端发起 RPC 请求时的一个辅助类，允许用户设置调用参数、获取调用状态、处理异常或中断请求。

MprpcController 是一个轻量级的控制器对象，主要职责包括：

控制 RPC 调用的状态（成功/失败）
提供错误信息反馈机制
支持调用取消与回调通知（虽未完整实现）
它使得客户端能够更灵活地控制 RPC 调用行为，适用于需要精细控制调用生命周期的场景，例如超时、取消、日志记录等。
*/
MprpcController::MprpcController()
{
    m_failed = false;
    m_errText = "";
}

void MprpcController::Reset()
{
    m_failed = false;
    m_errText = "";
}

bool MprpcController::Failed() const
{
    return m_failed;
}

std::string MprpcController::ErrorText() const
{
    return m_errText;
}

void MprpcController::SetFailed(const std::string& reason)
{
    m_failed = true;
    m_errText = reason;
}

// 目前未实现具体的功能
void MprpcController::StartCancel(){}
bool MprpcController::IsCanceled() const {return false;}
void MprpcController::NotifyOnCancel(google::protobuf::Closure* callback) {}


