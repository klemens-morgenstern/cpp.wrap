project(cpp.wrap.test)

if(CMAKE_COMPILER_IS_GNUCC)
    option(ENABLE_COVERAGE "Enable Coverage Reporting in GCC" FALSE)

    if(ENABLE_COVERAGE)
        add_definitions(--coverage -O0)
        set(LINKER_FLAGS "${LINKER_FLAGS} --coverage")
    endif()
endif()


add_library(test_static OBJECT test_static.cpp)
add_library(test_dyn    OBJECT test_dyn.cpp)
add_library(test_stub   OBJECT test_stub.cpp)
add_library(test_impl   OBJECT test_impl.cpp)

generate_wrapped_cpp(test_gen.cpp test_impl test_stub test_static TEMPLATE test_tpl.cpp WRAPPER_OPT wrapper.opt INDIRECT)

add_executable(no-stub dummy.cpp test_impl test_static)
target_compile_definitions(no-stub PRIVATE -DCPP_NO_DYNAMIC_TEST)
add_test(no-stub no-stub)

add_executable(stub test_gen.cpp test_stub test_impl test_static test_gen.cpp)
set_target_properties(stub PROPERTIES LINK_FLAGS @wrapper.opt)
add_test(stub stub)

add_executable(dyn-test test_gen.cpp test_stub test_impl test_gen.cpp test_dyn)
set_target_properties(dyn-test PROPERTIES LINK_FLAGS @wrapper.opt)
add_test(dyn-test dyn-test)