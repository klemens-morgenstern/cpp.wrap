project(cpp.wrap)

cmake_minimum_required(VERSION 3.9)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/state/lib)
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
include_directories(./include ${Boost_INCLUDE_DIRS} libs/fmt/include)
link_directories(${BOOST_LIBRARYDIR})


add_executable(cppwrap src/wrapper_main.cpp src/cpp/wrap/outline/gcc.cpp src/cpp/wrap/generator.cpp
        include/cpp/wrap.hpp include/cpp/wrap/outline/gcc.hpp include/cpp/wrap/generator.hpp
        include/cpp/wrap/outline.hpp)
target_link_libraries(cppwrap fmt-header-only Boost::program_options Boost::system Boost::filesystem)

include(build/cppwrap.cmake)

add_subdirectory(example)
add_subdirectory(libs/fmt)
add_subdirectory(test)