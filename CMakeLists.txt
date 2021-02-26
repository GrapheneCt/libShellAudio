cmake_minimum_required(VERSION 3.19)

set(CMAKE_TOOLCHAIN_FILE "$ENV{SCE_PSP2_SDK_DIR}/host_tools/build/cmake/psp2-snc-toolchain.cmake")

project(ShellAudio LANGUAGES C)

include(VitaDevelopmentSuite)
set(CMAKE_C_STANDARD 99)

add_compile_options(
  -Xdiag=0 -Xquit=2 -O3
)

add_link_options(
  -s
)

include_directories(
  ${VDSUITE_USER_INCLUDE_DIRECTORIES}
  libShellAudio/include
)

link_directories(
  ${VDSUITE_LIBRARY_DIRECTORIES}
)

add_library(${PROJECT_NAME} STATIC
  libShellAudio/source/ShellAudio.c
)

add_definitions(-DSHELLAUDIO_PRX)

add_library("${PROJECT_NAME}.suprx" SHARED
  libShellAudio/source/ShellAudio.c
)

target_link_libraries("${PROJECT_NAME}.suprx"
  SceAppMgrUser_stub_weak
  SceShellSvc_stub_weak
)
