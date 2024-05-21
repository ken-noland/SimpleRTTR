set(CMAKE_CXX_STANDARD 17)


#---------------------------------------------------------------------------------------------------------
# Get remote content

# Fetch Mustache
#  Mustache is used as our template engine for generating code
include(FetchContent)
FetchContent_Declare(
  Mustache
  GIT_REPOSITORY https://github.com/kainjow/Mustache.git
  GIT_TAG        "origin/master"
)
FetchContent_MakeAvailable(Mustache)

# Fetch Lua
#  Lua is used as a scripting language to showcase the script binding capabilities of SimpleRTTR
FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/lua/lua.git
  GIT_TAG "origin/v5.4"
)
FetchContent_MakeAvailable(lua)

# Lua does not provide a CMake build system, so we need to manually set up the target
add_library(lua STATIC  ${lua_SOURCE_DIR}/lapi.c
                        ${lua_SOURCE_DIR}/lauxlib.c
                        ${lua_SOURCE_DIR}/lbaselib.c
                        ${lua_SOURCE_DIR}/lcode.c
                        ${lua_SOURCE_DIR}/lcorolib.c
                        ${lua_SOURCE_DIR}/lctype.c
                        ${lua_SOURCE_DIR}/ldblib.c
                        ${lua_SOURCE_DIR}/ldebug.c
                        ${lua_SOURCE_DIR}/ldo.c
                        ${lua_SOURCE_DIR}/ldump.c
                        ${lua_SOURCE_DIR}/lfunc.c
                        ${lua_SOURCE_DIR}/lgc.c
                        ${lua_SOURCE_DIR}/linit.c
                        ${lua_SOURCE_DIR}/liolib.c
                        ${lua_SOURCE_DIR}/llex.c
                        ${lua_SOURCE_DIR}/lmathlib.c
                        ${lua_SOURCE_DIR}/lmem.c
                        ${lua_SOURCE_DIR}/loadlib.c
                        ${lua_SOURCE_DIR}/lobject.c
                        ${lua_SOURCE_DIR}/lopcodes.c
                        ${lua_SOURCE_DIR}/loslib.c
                        ${lua_SOURCE_DIR}/lparser.c
                        ${lua_SOURCE_DIR}/lstate.c
                        ${lua_SOURCE_DIR}/lstring.c
                        ${lua_SOURCE_DIR}/lstrlib.c
                        ${lua_SOURCE_DIR}/ltable.c
                        ${lua_SOURCE_DIR}/ltablib.c
                        ${lua_SOURCE_DIR}/ltests.c
                        ${lua_SOURCE_DIR}/ltm.c
                        ${lua_SOURCE_DIR}/lua.c
                        ${lua_SOURCE_DIR}/lundump.c
                        ${lua_SOURCE_DIR}/lutf8lib.c
                        ${lua_SOURCE_DIR}/lvm.c
                        ${lua_SOURCE_DIR}/lzio.c)

target_include_directories(lua PUBLIC ${lua_SOURCE_DIR}/src)

# Fetch nlohmann/json
#  nlohmann/json is used to parse and render json data to demonstrate the serialization capabilities of SimpleRTTR
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG "origin/develop"
)
FetchContent_MakeAvailable(json)

#---------------------------------------------------------------------------------------------------------
# Compile the example objects

set(EXAMPLE_OBJECTS_SOURCES 
	"objects/customer.cpp"
	"objects/customer.h"
	"objects/order.cpp"
	"objects/order.h"
	"objects/point.cpp"
	"objects/point.h"
)

# the example objects are compiled into a library that will be used by the code generation project
add_library(ExampleObjects OBJECT ${EXAMPLE_OBJECTS_SOURCES})
target_include_directories(ExampleObjects PUBLIC ${SIMPLERTTR_INCLUDE_PATH})

#---------------------------------------------------------------------------------------------------------
# Compile and link the CodeGen project with the example objects
add_executable(ExampleCodeGen codegen.cpp)

# Note: the CodeGen doesn't care about the json or lua libraries, so we only need the ExampleObjects library
target_link_libraries(ExampleCodeGen ExampleObjects)

target_compile_definitions(ExampleCodeGen PUBLIC SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(ExampleCodeGen PUBLIC PROJECT_DIR="${PROJECT_SOURCE_DIR}")
target_compile_definitions(ExampleCodeGen PUBLIC BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}")

# Add SimpleRTTR
target_include_directories(ExampleCodeGen PUBLIC ${SIMPLERTTR_INCLUDE_PATH})

# Add the Mustache library
target_include_directories(ExampleCodeGen PUBLIC ${mustache_SOURCE_DIR})

set(GENERATED_CMAKELISTS ${CMAKE_CURRENT_BINARY_DIR}/generated/CMakeLists.txt)
set(GENERATED_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(CONFIGURED_CACHE ${GENERATED_BUILD_DIR}/CMakeCache.txt)

#---------------------------------------------------------------------------------------------------------
# With the CodeGen project compiled, we can now generate and build the generated code

# the templates that are used to generate the code
set(TEMPLATE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/templates/CMakeLists.txt.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/templates/JsonTemplate.cpp.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/templates/LuaTemplate.cpp.in"
)

# The CodeGen project generates a separate CMakeLists.txt file in the output folder that is used to build the generated code
add_custom_command(
    OUTPUT ${GENERATED_CMAKELISTS}
    COMMAND ExampleCodeGen
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ExampleCodeGen ${TEMPLATE_FILES}
    COMMENT "Generating Code"
)

set(CMAKE_INCLUDE_DIRECTIVES
	-DSIMPLERTTR_INCLUDE_PATH="${SIMPLERTTR_INCLUDE_PATH}"
	-DLUA_INCLUDE_PATH="${lua_SOURCE_DIR}"
)

# Configure the new cmake file to build the generated code
add_custom_command(
    OUTPUT ${CONFIGURED_CACHE}
    COMMAND ${CMAKE_COMMAND} ${CMAKE_INCLUDE_DIRECTIVES} -S ${CMAKE_CURRENT_BINARY_DIR}/generated -B ${CMAKE_CURRENT_BINARY_DIR}/build
    DEPENDS ${GENERATED_CMAKELISTS}
    COMMENT "Configuring generated code"
    USES_TERMINAL
)

# annoyingly, the library is located in a build directory, so we need to extract the path. I'm not too sure if this is portable
set(EXAMPLE_GENERATED_LIBRARY ${GENERATED_BUILD_DIR}/CodeGen.dir/Debug/CodeGen.lib)
message("Generated library : ${GENERATED_LIBRARY}")

# Custom command to build the configured generated code
add_custom_command(
    OUTPUT ${EXAMPLE_GENERATED_LIBRARY}
    COMMAND ${CMAKE_COMMAND} --build ${GENERATED_BUILD_DIR}
    DEPENDS ${CONFIGURED_CACHE}
    COMMENT "Building generated code"
    USES_TERMINAL
)

add_custom_target(ExampleCodeGenBuild ALL DEPENDS ${EXAMPLE_GENERATED_LIBRARY})

add_executable(ExampleCodeGenRun run_codegen.cpp)
add_dependencies(ExampleCodeGenRun ExampleCodeGenBuild)

target_link_libraries(ExampleCodeGenRun ${ExampleObjects} ${EXAMPLE_GENERATED_LIBRARY} lua nlohmann_json::nlohmann_json)
