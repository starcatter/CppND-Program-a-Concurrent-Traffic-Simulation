cmake_minimum_required(VERSION 3.10.0)

# set(CMAKE_CXX_STANDARD 17)
project(Traffic_sim)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

# Add project executable
add_executable(traffic_simulation ${project_SRCS})
target_link_libraries(traffic_simulation PUBLIC ${wxWidgets_LIBRARIES})
target_include_directories(traffic_simulation PUBLIC ${wxWidgets_INCLUDE_DIRS})
