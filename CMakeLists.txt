cmake_minimum_required(VERSION 3.24)



project(
    Linmeng3D 
    VERSION 0.1.0 
    LANGUAGES C CXX
)

#encode set
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")


set(CMAKE_CXX_STANDARD 17)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
set(SYS_DLL_DIR "F:/C++Program/bin" CACHE PATH "你的一个全局动态链接库搜索路径" )
set(CMAKE_PREFIX_PATH "F:/C++Program/C++3rd")
include(3rd/include_3rds.cmake)#第三方

add_subdirectory(Linmeng)




include(CTest)
option(ENGINE_BUILD_TEST "should build unittest" OFF)
# 如果为顶层cmake(这个库为工程本体-> 库开发) 必会开启测试
if (PROJECT_IS_TOP_LEVEL OR ENGINE_BUILD_TEST) 

    include(CTest) 
    enable_testing()
    add_subdirectory(test)

endif()
