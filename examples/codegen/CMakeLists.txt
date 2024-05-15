set(CMAKE_CXX_STANDARD 17)

# going to need to clone a few repos to get this to work. First up is Mustache, which is the template engine we are using. Then we'll 
#	need the LUA, python, and Json libraries

# Mustache
include(FetchContent)
FetchContent_Declare(
  Mustache
  GIT_REPOSITORY https://github.com/kainjow/Mustache.git
  GIT_TAG        "origin/master"
)
FetchContent_MakeAvailable(Mustache)

set(SOURCES 
	codegen.cpp
    point.cpp
)

# SimpleRTTRCodeGen example
add_executable(SimpleRTTRCodeGen ${SOURCES})

target_compile_definitions(SimpleRTTRCodeGen PUBLIC TEMPLATE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/templates/")
target_compile_definitions(SimpleRTTRCodeGen PUBLIC PROJECT_DIR="${PROJECT_SOURCE_DIR}")
target_compile_definitions(SimpleRTTRCodeGen PUBLIC BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}")

# Add SimpleRTTR
target_include_directories(SimpleRTTRCodeGen PUBLIC ${SIMPLERTTR_INCLUDE_PATH})

# Add the Mustache library
target_include_directories(SimpleRTTRCodeGen PUBLIC ${mustache_SOURCE_DIR})

set(GENERATED_CMAKELISTS ${CMAKE_CURRENT_BINARY_DIR}/generated/CMakeLists.txt)
set(GENERATED_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(CONFIGURED_CACHE ${GENERATED_BUILD_DIR}/CMakeCache.txt)

# the CodeGen project generates a separate CMakeLists.txt file that is used to build the generated code, so we need to set up a target and all the dependencies
add_custom_target(CodeGen
	COMMAND SimpleRTTRCodeGen
	BYPRODUCTS ${GENERATED_CMAKELISTS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS SimpleRTTRCodeGen
	COMMENT "Generating Code"
	USES_TERMINAL
)

# Configure the new cmake file to build the generated code
add_custom_command(
    OUTPUT ${CONFIGURED_CACHE}
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_CURRENT_BINARY_DIR}/generated -B ${CMAKE_CURRENT_BINARY_DIR}/build
    DEPENDS ${GENERATED_CMAKELISTS}
    COMMENT "Configuring generated code"
    USES_TERMINAL
)

add_custom_target(ConfigureGenerated
    DEPENDS ${CONFIGURED_CACHE}
)

# Custom target to build the configured generated code
add_custom_target(BuildGenerated
    COMMAND ${CMAKE_COMMAND} --build ${GENERATED_BUILD_DIR}
    DEPENDS ConfigureGenerated
    COMMENT "Building generated code"
    USES_TERMINAL
)

# annoyingly, the library is located in a build directory, so we need to extract the path.
set(GENERATED_LIBRARY ${GENERATED_BUILD_DIR})

add_executable(SimpleRTTRCodeGenRun run_codegen.cpp)
add_dependencies(SimpleRTTRCodeGenRun BuildGenerated)
target_link_libraries(SimpleRTTRCodeGenRun ${GENERATED_LIBRARY}/CodeGen.dir/Debug/CodeGen.lib)
