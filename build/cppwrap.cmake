
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
        get_filename_component(tpl_full ${tpl} ABSOLUTE)
        set(tpl_arg --template=${tpl_full})
        list(REMOVE_AT ARGV ${tpl_loc} ${tpl_loc2})
    endif()

    list(FIND ARGV WRAPPER_OPT wrapper_opt_loc)
    if (${wrapper_opt_loc} EQUAL -1)
        unset(wrapper_opt_loc)
    else()
        math(EXPR wrapper_opt_loc2 ${wrapper_opt_loc}+1 )
        list(GET ARGV ${wrapper_opt_loc2} wrapper_opt)
        set(wrapper_opt_arg --wrapper-out=${wrapper_opt})
        list(REMOVE_AT ARGV ${wrapper_opt_loc} ${wrapper_opt_loc2})
    endif()
    list(REMOVE_AT ARGV 0)

    foreach(list_it IN LISTS ARGV)
        set(in_dep ${in_dep} ${list_it})
        set(target_objects $<TARGET_OBJECTS:${list_it}> ${target_objects})
    endforeach(list_it)

    add_custom_command(
            COMMAND $<TARGET_FILE:cppwrap> --output=${name} ${target_objects} ${tpl_arg} ${is_indirect} ${wrapper_opt_arg}
            OUTPUT ${name}
            BYPRODUCTS ${wrapper_opt}
            COMMENT "Running wrapper on ${ARGV}"
            DEPENDS cppwrap ${in_dep} ${tpl})

endfunction(generate_wrapped_cpp)