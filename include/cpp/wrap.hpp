/**
 * @file   cpp/wrap.hpp
 * @date   20.03.2017
 * @author Klemens D. Morgenstern
 * @brief Header for the declaration of wrapping functions.
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef CPP_WRAP_HPP_
#define CPP_WRAP_HPP_

#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <boost/preprocessor/facilities/overload.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/list/fold_left.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/tuple/size.hpp>

#include <boost/config.hpp>
#include <type_traits>

#include <stdexcept>

namespace cpp
{

namespace detail
{
    template<typename T>
    struct tag {};

    struct dummy;

    template<typename T, T func>
    struct connector_impl
    {
        static void    connect(void * p) {   connect_impl(p, func);}
        static void disconnect(void * p) {disconnect_impl(p, func);}

        static void    connect_impl(void*, T);
        static void disconnect_impl(void*, T);
    };

    struct connector
    {
        void * ptr;
        void (*impl_ptr)(void*);

        template<typename Class, typename ConnImpl>
        connector(Class * ptr, const ConnImpl & cn_impl) : ptr(static_cast<void*>(ptr)), impl_ptr(&ConnImpl::disconnect)
        {
            ConnImpl::connect(ptr);
        }
        ~connector()
        {
            impl_ptr(ptr);
        }
    };

}
}

#define CPP_VA_ARGS(...) ,##__VA_ARGS__

#define CPP_WRAP_MAKE_SCOPE_OP(d, state, x) BOOST_PP_CAT(BOOST_PP_CAT(state, __CPP__), x)
#define CPP_WRAP_MAKE_SCOPE(...) BOOST_PP_CAT(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), BOOST_PP_LIST_FOLD_LEFT(CPP_WRAP_MAKE_SCOPE_OP, , BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)))

#define CPP_WRAP_FN_FIX_4(Scope, Name, Return, Signature)  \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) BOOST_PP_CAT(BOOST_PP_CAT(_CPP_wrap_fn_fix_, CPP_WRAP_MAKE_SCOPE(BOOST_PP_REMOVE_PARENS(Scope))), __CPP__##Name)  \
    ( ::cpp::detail::tag<BOOST_PP_REMOVE_PARENS(Return)> * CPP_VA_ARGS Signature )


#define CPP_WRAP_FN_FIX_3(Name, Return, Signature)  CPP_WRAP_FN_FIX_4( (), Name, Return, Signature)

#define CPP_WRAP_FN_FIX(...) BOOST_PP_OVERLOAD(CPP_WRAP_FN_FIX_, __VA_ARGS__)(__VA_ARGS__)


#define CPP_WRAP_FN_FIX_NO_ARGS_3(Scope, Name, Return)  \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) BOOST_PP_CAT(BOOST_PP_CAT(_CPP_wrap_fn_fix_, CPP_WRAP_MAKE_SCOPE(BOOST_PP_REMOVE_PARENS(Scope))), __CPP__##Name)  \
    ( ::cpp::detail::tag<BOOST_PP_REMOVE_PARENS(Return)> * )

#define CPP_WRAP_FN_FIX_NO_ARGS_2(Name, Return)  CPP_WRAP_FN_FIX_NO_ARGS_3( (), Name, Return)

#define CPP_WRAP_FN_FIX_NO_ARGS(...) BOOST_PP_OVERLOAD(CPP_WRAP_FN_FIX_NO_ARGS_, __VA_ARGS__)(__VA_ARGS__)


#define CPP_WRAP_MEM_FIX(Type, Name, Return, Signature) \
BOOST_SYMBOL_EXPORT Return BOOST_PP_CAT(_CPP_wrap_mem_fix_, Name) (BOOST_PP_REMOVE_PARENS(Type) * const this_ CPP_VA_ARGS Signature, ::cpp::detail::tag<Return> * = nullptr)

#define CPP_WRAP_MEM_FIX_NO_ARGS(Type, Name, Return) \
BOOST_SYMBOL_EXPORT Return BOOST_PP_CAT(_CPP_wrap_mem_fix_, Name) (BOOST_PP_REMOVE_PARENS(Type) * const this_, ::cpp::detail::tag<Return> * = nullptr)


#define CPP_WRAP_STATIC_MEM_FIX(Type, Name, Return, Signature) \
BOOST_SYMBOL_EXPORT Return BOOST_PP_CAT(_CPP_wrap_static_mem_fix_, Name) (BOOST_PP_REMOVE_PARENS(Type) * const CPP_VA_ARGS Signature, ::cpp::detail::tag<Return> * = nullptr)

#define CPP_WRAP_STATIC_MEM_FIX_NO_ARGS(Type, Name, Return) \
BOOST_SYMBOL_EXPORT Return BOOST_PP_CAT(_CPP_wrap_static_mem_fix_, Name) (BOOST_PP_REMOVE_PARENS(Type) * const, ::cpp::detail::tag<Return> * = nullptr)


#define CPP_WRAP_MAKE_CONNECTOR_IMPL(Value) ::cpp::detail::connector_impl<decltype(Value), Value>()

#define CPP_WRAP_FN_IMPL(Scope, Name, Return, Signature, FuncName) \
::cpp::detail::connector BOOST_PP_CAT(_CPP_wrap_connector, __COUNTER__) { this, CPP_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName  \
    ( BOOST_PP_REMOVE_PARENS(Signature) )

#define CPP_WRAP_FN_4(Scope, Name, Return, Signature)  CPP_WRAP_FN_IMPL(Scope, Name, Return, Signature, BOOST_PP_CAT(_CPP_wrap_fn_ , BOOST_PP_CAT( CPP_WRAP_MAKE_SCOPE(BOOST_PP_REMOVE_PARENS(Scope)), BOOST_PP_CAT(__CPP__##Name##__CPP__, __LINE__))))
#define CPP_WRAP_FN_3(Name, Return, Signature)         CPP_WRAP_FN_4( (), Name, Return, Signature)

#define CPP_WRAP_FN(...) BOOST_PP_OVERLOAD(CPP_WRAP_FN_, __VA_ARGS__)(__VA_ARGS__)


#define CPP_WRAP_FN_NO_ARGS_IMPL(Scope, Name, Return, FuncName) \
::cpp::detail::connector BOOST_PP_CAT(_CPP_wrap_connector, __COUNTER__) { this, CPP_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName ( )

#define CPP_WRAP_FN_NO_ARGS_3(Scope, Name, Return)  CPP_WRAP_FN_NO_ARGS_IMPL(Scope, Name, Return, BOOST_PP_CAT(_CPP_wrap_fn_ , BOOST_PP_CAT( CPP_WRAP_MAKE_SCOPE(BOOST_PP_REMOVE_PARENS(Scope)), BOOST_PP_CAT(__CPP__##Name##__CPP__, __LINE__))))
#define CPP_WRAP_FN_NO_ARGS_2(Name, Return)         CPP_WRAP_FN_NO_ARGS_3( (), Name, Return)

#define CPP_WRAP_FN_NO_ARGS(...) BOOST_PP_OVERLOAD(CPP_WRAP_FN_NO_ARGS_, __VA_ARGS__)(__VA_ARGS__)


#define CPP_WRAP_MEM_IMPL(Type, Name, Return, Signature, FuncName) \
::cpp::detail::connector BOOST_PP_CAT(_CPP_wrap_connector, __COUNTER__) { this, CPP_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName (BOOST_PP_REMOVE_PARENS(Type) * const this_  CPP_VA_ARGS Signature )

#define CPP_WRAP_MEM(Type, Name, Return, Signature)  CPP_WRAP_MEM_IMPL(Type, Name, Return, Signature, BOOST_PP_CAT(_CPP_wrap_mem_##Name##__CPP__, __LINE__))


#define CPP_WRAP_MEM_NO_ARGS_IMPL(Type, Name, Return, FuncName) \
::cpp::detail::connector BOOST_PP_CAT(_CPP_wrap_connector, __COUNTER__) { this, CPP_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName (BOOST_PP_REMOVE_PARENS(Type) * const this_ )

#define CPP_WRAP_MEM_NO_ARGS(Type, Name, Return)  CPP_WRAP_MEM_IMPL(Type, Name, Return, BOOST_PP_CAT(_CPP_wrap_mem_##Name##__CPP__, __LINE__))


#define CPP_WRAP_STATIC_MEM_IMPL(Type, Name, Return, Signature, FuncName) \
::cpp::detail::connector BOOST_PP_CAT(_CPP_wrap_connector, __COUNTER__) { this, CPP_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName (BOOST_PP_REMOVE_PARENS(Type) * const CPP_VA_ARGS Signature )

#define CPP_WRAP_STATIC_MEM(Type, Name, Return, Signature)  CPP_WRAP_STATIC_MEM_IMPL(Type, Name, Return, Signature, BOOST_PP_CAT(_CPP_wrap_static_mem_##Name##__CPP__, __LINE__))


#define CPP_WRAP_STATIC_MEM_NO_ARGS_IMPL(Type, Name, Return, FuncName) \
::cpp::detail::connector BOOST_PP_CAT(_CPP_wrap_connector, __COUNTER__) { this, CPP_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName (BOOST_PP_REMOVE_PARENS(Type) * const)

#define CPP_WRAP_STATIC_MEM_NO_ARGS(Type, Name, Return)  CPP_WRAP_STATIC_MEM_IMPL(Type, Name, Return, BOOST_PP_CAT(_CPP_wrap_static_mem_##Name##__CPP__, __LINE__))


#endif
