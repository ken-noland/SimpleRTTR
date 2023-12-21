set(CMAKE_CXX_STANDARD 17)

add_executable(SimpleRTTRCodeGen main.cpp)
target_include_directories(SimpleRTTRCodeGen PUBLIC ${SIMPLERTTR_INCLUDE_PATH})
