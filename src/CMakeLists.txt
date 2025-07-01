#添加源文件
aux_source_directory(. SRC_LIST)

#有了上边的，就不用再添加了下边的了
# set(SRC_LIST 
#     mprpcapplication.cc 
#     mprpcconfig.cc 
#     rpcheader.pb.cc 
#     rpcprovider.cc 
#     mprpcchannel.cc
#     mprpccontroller.cc
#     logger.cc
#     zookeeperutil.cc)

# 生成库文件
add_library(mprpc ${SRC_LIST})
# 链接第三方动态库
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)


