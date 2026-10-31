# 独立的小练习程序，每个都能单独跑。写完的代码会搬进 agent/ 或 client/。

# 练习：两个进程通过管道 / ssh 互发 ping-pong
add_executable(pipe_ping pipe_ping.cpp)
target_link_libraries(pipe_ping PRIVATE tether_common)

# 练习：X11 截屏存成 PPM 文件
find_package(X11 REQUIRED)
add_executable(grab_ppm grab_ppm.cpp)
target_link_libraries(grab_ppm PRIVATE tether_common X11::X11 X11::Xext)

# 练习：XTest 注入鼠标、键盘
add_executable(xtest_demo xtest_demo.cpp)
target_link_libraries(xtest_demo PRIVATE tether_common X11::X11 X11::Xtst)

# 练习：SDL 窗口显示图片 + 打印输入事件
find_package(SDL2 REQUIRED)
add_executable(sdl_view sdl_view.cpp)
target_link_libraries(sdl_view PRIVATE tether_common SDL2::SDL2)

# 练习：把连续截屏编码成 H.264 文件，用 ffplay 播放验证
if(TETHER_WITH_FFMPEG)
  add_executable(h264_file h264_file.cpp)
  target_link_libraries(h264_file PRIVATE tether_common X11::X11 X11::Xext PkgConfig::FFMPEG)
endif()
