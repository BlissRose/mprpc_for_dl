#pragma once

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

/*
 * 框架提供的rpc数据通信发送接口
1、rpc请求数据的组装，数据的序列化
2、发送rpc请求数据，网络发送
3、等待rpc请求的响应数据，网络接收
4、rpc响应数据的反序列化
 */
class MprpcChannel : public google::protobuf::RpcChannel
{
public:
    // 所有通过stub代理对象调用的rpc方法，都走到这里了，统一做rpc方法调用的数据数据序列化和网络发送 
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response,
                          google::protobuf:: Closure* done);
};

