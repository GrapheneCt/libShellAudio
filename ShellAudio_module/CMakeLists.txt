cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{DOLCESDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{DOLCESDK}/share/dolce.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define DOLCESDK to point to your SDK path!")
  endif()
endif()

include("$ENV{DOLCESDK}/share/dolce.cmake" REQUIRED)

project(ShellAudio)
dolce_gen_libs(SceShellSvc_stubs
  SceShellSvc.yml
  LIB SceShellSvc_stub_weak)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
link_directories(${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -nostdlib")
set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS
  -nostartfiles
)
target_link_libraries(${PROJECT_NAME}
  SceSysmem_stub_weak
  SceDisplay_stub_weak
  SceSysmodule_stub_weak
  SceIofilemgr_stub_weak
  SceLibKernel_stub_weak
  SceKernelThreadMgr_stub_weak
  SceKernelModulemgr_stub_weak
  SceAppMgr_stub_weak
  SceShellSvc_stub_weak
)

dolce_create_self(${PROJECT_NAME}.suprx ${PROJECT_NAME}
  CONFIG ${CMAKE_SOURCE_DIR}/exports.yml
  UNSAFE
)

dolce_create_stubs(${PROJECT_NAME}_stubs 
  ${PROJECT_NAME} 
  ${CMAKE_SOURCE_DIR}/exports.yml
  LIB ShellAudio_stub
)

