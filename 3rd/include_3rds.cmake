
include(${CMAKE_CURRENT_LIST_DIR}/sdl/find_sdl2.cmake)


# get_target_property(SDL2main_DLL    SDL2::SDL2main IMPORTED_LOCATION) sdl2 main 没有dll
get_target_property(SDL2_DLL        SDL2::SDL2 IMPORTED_LOCATION)


# 对于子层cmake变量 设置cache使之上层可见
# 为了使目标在上层可见. -> 改为.cmake在top引用. cwd 成为顶层目录
set(3rd_LIBS        ${SDL2_LIBRARIES}  CACHE INTERNAL "3rd libs target names" )
set(3rd_LIBS_DLLS   ${SDL2_DLL}  CACHE INTERNAL "3rd libs' dll path lists" )
