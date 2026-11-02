# 不引入测试框架，每个测试是一个独立程序：返回 0 表示通过，非 0 表示失败。
# 运行：cmake --build build && ctest --test-dir build --output-on-failure

set(tests test_bytes test_messages test_thread_queue)
# test_framing 用 pipe() 造管道，只在 POSIX 上编译（Windows 版要改用 CreatePipe）
if(NOT WIN32)
  list(APPEND tests test_framing)
endif()

foreach(name ${tests})
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} PRIVATE tether_common)
  add_test(NAME ${name} COMMAND ${name})
  # 多线程测试写错时容易卡死，10 秒没结束就算失败
  set_tests_properties(${name} PROPERTIES TIMEOUT 10)
endforeach()

# 流控逻辑不依赖 X11，直接把源文件编进测试
add_executable(test_flow_control
  test_flow_control.cpp
  ${PROJECT_SOURCE_DIR}/agent/flow/flow_control.cpp)
target_include_directories(test_flow_control PRIVATE ${PROJECT_SOURCE_DIR}/agent)
target_link_libraries(test_flow_control PRIVATE tether_common)
add_test(NAME test_flow_control COMMAND test_flow_control)
set_tests_properties(test_flow_control PROPERTIES TIMEOUT 10)
