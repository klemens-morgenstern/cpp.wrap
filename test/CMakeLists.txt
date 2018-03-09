project(cpp.wrap.test)

if(CMAKE_COMPILER_IS_GNUCC)
    option(ENABLE_COVERAGE "Enable Coverage Reporting in GCC" FALSE)

    if(ENABLE_COVERAGE)
        add_definitions(--coverage -O0)
        set(LINKER_FLAGS "${LINKER_FLAGS} --coverage")
    endif()
endif()

add_library(test_stub STATIC test_stub.cpp)
add_library(test_impl STATIC test_impl.cpp)

add_library(test_    STATIC     test.cpp)
add_library(test_dyn STATIC test_dyn.cpp)

generate_wrapped_cpp(test_gen.cpp test_impl test_stub TEMPLATE test_tpl.cpp WRAPPER_OPT wrapper.opt INDIRECT)

add_executable(no-stub dummy.cpp)
target_link_libraries(no-stub test_impl test)
target_compile_definitions(no-stub PRIVATE -DCPP_NO_DYNAMIC_TEST)

add_executable(stub test_gen.cpp)
target_link_libraries(stub test_stub test_impl test_gen test_)
set_target_properties(stub PROPERTIES LINK_FLAGS @wrapper.opt)

add_executable(dyn-test test_gen.cpp)
target_link_libraries(dyn-test test_stub test_impl test_gen test_dyn)
set_target_properties(dyn-test PROPERTIES LINK_FLAGS @wrapper.opt)