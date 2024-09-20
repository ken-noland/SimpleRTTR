cmake_minimum_required(VERSION 3.14)
project(SimpleRTTR)

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0135 NEW) 

option(SIMPLERTTR_BUILD_TESTS "Build the tests" true)
option(SIMPLERTTR_BUILD_EXAMPLES "Build the examples" true)

set(SIMPLERTTR_SRC		src/types.cpp)

set(SIMPLERTTR_HEADERS	include/helpers.h
						include/iterator.h
						include/meta.h
						include/meta.inl
						include/method.h
						include/method.inl
						include/property.h
						include/property.inl
						include/registration.h
						include/registration.inl
						include/simplerttr.h
						include/simplerttr.inl
						include/simplerttrassert.h
						include/typebinding.h
						include/typebinding.inl
						include/typedata.h
						include/typedata.inl
						include/typehelpers.h
						include/typereference.h
						include/typereference.inl
						include/types.h
						include/types.inl
						include/value.h
						include/value.inl
						include/variant.h
						include/variant.inl
						include/variant_tostring.inl
)

add_library(SimpleRTTR STATIC ${SIMPLERTTR_SRC} ${SIMPLERTTR_HEADERS})
target_include_directories(SimpleRTTR PUBLIC include)

# Tests
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

	set(SIMPLERTTR_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)

endif()

# Examples
if(${SIMPLERTTR_BUILD_EXAMPLES})

	add_subdirectory(examples/codegen)

endif()
