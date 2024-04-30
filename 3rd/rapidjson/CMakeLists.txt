add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE ./)
target_compile_features(rapidjson INTERFACE cxx_std_17)