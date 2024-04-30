
include(${CMAKE_CURRENT_LIST_DIR}/sdl/find_sdl2.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sdl2-image/find_sdl2_image.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sdl2_ttf/find_SDL_ttf.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/sdl2-image/find_sdl2_image.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glad)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/stb_image) 
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/rapidjson) 
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/fmod) 


get_target_property(SDL2_DLL        SDL2::SDL2 IMPORTED_LOCATION)
get_target_property(SDL2_ttf_DLL    SDL2_ttf::SDL2_ttf IMPORTED_LOCATION)
get_target_property(FMOD_CORE_DLL   FMOD::core IMPORTED_LOCATION)
get_target_property(FMOD_STUDIO_DLL   FMOD::studio IMPORTED_LOCATION)



# 对于子层cmake变量 设置cache使之上层可见
# 为了使目标在上层可见. -> 改为.cmake在top引用. cwd 成为顶层目录
set(3rd_LIBS        ${SDL2_LIBRARIES} rapidjson SDL2_ttf::SDL2_ttf glad stb_image FMOD::core FMOD::studio CACHE INTERNAL "3rd libs target names" )
set(3rd_LIBS_DLLS   ${SDL2_DLL} ${SDL2_ttf_DLL} ${SDL2_image_DLL} ${FMOD_CORE_DLL} ${FMOD_STUDIO_DLL} CACHE INTERNAL "3rd libs' dll path lists" )

