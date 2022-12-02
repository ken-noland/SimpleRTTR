cmake_minimum_required(VERSION 3.14)
project(SimpleRTTR)

set(CMAKE_CXX_STANDARD 17)

option(SIMPLERTTR_BUILD_TESTS "Build the tests" true)

set(SIMPLERTTR_SRC		src/types.cpp)

set(SIMPLERTTR_HEADERS	include/assert.h
						include/method.h
						include/method.inl
						include/property.h
						include/property.inl
						include/registration.h
						include/registration.inl
						include/simplerttr.h
						include/simplerttr.inl
						include/typebinding.h
						include/typebinding.inl
						include/typedata.h
						include/types.h
						include/types.inl
						include/variant.h
						include/variant.inl
)

add_library(SimpleRTTR STATIC ${SIMPLERTTR_SRC} ${SIMPLERTTR_HEADERS})
target_include_directories(SimpleRTTR PUBLIC include)

if(${SIMPLERTTR_BUILD_TESTS})

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)

enable_testing()

add_subdirectory(test)

endif()
