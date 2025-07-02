# 设置源文件列表，用于指定将要编译的源代码文件
# 在这个例子中，我们只编译friendservice.cc和../friend.pb.cc这两个源文件
# set(SRC_LIST friendservice.cc ../friend.pb.cc)
set(SRC_LIST userservice.cc ../user.pb.cc)

# 添加一个可执行目标，名称为provider，源文件来自之前设置的SRC_LIST变量
# 这个命令告诉CMake将指定的源文件编译并链接成一个名为provider的可执行文件
add_executable(provider ${SRC_LIST})
# 为provider目标链接所需的库
# 这里链接了mprpc和protobuf库，因为编译过程中可能需要这些库的功能
target_link_libraries(provider mprpc protobuf)


