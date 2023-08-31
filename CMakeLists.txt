cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(flow_model)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(flow_model flow_model.cpp)
target_link_libraries(flow_model "${TORCH_LIBRARIES}")
set_property(TARGET flow_model PROPERTY CXX_STANDARD 17)
