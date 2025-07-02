# set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callfriendservice_test.cc ../user.pb.cc)
add_executable(consumer ${SRC_LIST})

target_link_libraries(consumer mprpc protobuf)
# target_link_libraries(consumer mprpc ${PROTOBUF_LIBRARIES})


