project(cpp.wrap)

cmake_minimum_required(VERSION 3.9)
find_package(boost COMPONENTS program_options REQUIRED)
include_directories(./include ${Boost_INCLUDE_DIRS} libs/fmt/include)
link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(example)
add_subdirectory(libs/fmt)
add_subdirectory(test)

add_executable(cppwrap src/wrapper_main.cpp src/cpp/wrap/outline/gcc.cpp src/cpp/wrap/generator.cpp
               include/cpp/wrap.hpp include/cpp/wrap/outline/gcc.hpp include/cpp/wrap/generator.hpp
               include/cpp/wrap/outline.hpp)
target_link_libraries(cppwrap fmt-header-only )

function(generate_wrapped_cpp name)
    set(CPPWRAP_CMD $<TARGET_FILE:cppwrap>)
    list(FIND ARGV INDIRECT is_indirect_loc)

    if (${is_indirect_loc} EQUAL -1)
        unset(is_indirect_loc)
    else()
        set(is_indirect --indirect)
        list(REMOVE_AT ARGV ${is_indirect_loc})
    endif()

    list(FIND ARGV TEMPLATE tpl_loc)
    if (${tpl_loc} EQUAL -1)
        unset(tpl_loc)
    else()
        math(EXPR tpl_loc2 ${tpl_loc}+1 )
        list(GET ARGV ${tpl_loc2} tpl)
        set(tpl --template=${tpl})
        list(REMOVE_AT ARGV ${tpl_loc} ${tpl_loc2})
    endif()

    list(FIND ARGV WRAPPER_OPT wrapper_opt_loc)
    if (${wrapper_opt_loc} EQUAL -1)
        unset(wrapper_opt_loc)
    else()
        math(EXPR wrapper_opt_loc2 ${wrapper_opt_loc}+1 )
        list(GET ARGV ${wrapper_opt_loc2} wrapper_opt)
        set(wrapper_opt --wrapper-out=${wrapper_opt})
        list(REMOVE_AT ARGV ${wrapper_opt_loc} ${wrapper_opt_loc2})
    endif()

    add_custom_target(wrap_${name}
            COMMAND $<TARGET_FILE:cppwrap> --output=${name} ${ARGV} ${tpl} ${indirect} ${wrapper_opt}
            BYPRODUCTS ${name} ${wrapper_opt}
            DEPENDENCY cpprwap ${ARGV})

endfunction(generate_wrapped_cpp)