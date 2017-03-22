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

namespace mw
{

class wrapper_already_set : std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
    using std::runtime_error::operator=;
};

namespace detail
{
    template<typename T>
    struct tag {};

    struct dummy;

    template<typename T, T func>
    struct connector_impl
    {
        static void    connect() {   connect_impl(func);}
        static void disconnect() {disconnect_impl(func);}

        static void    connect_impl(T);
        static void disconnect_impl(T);
    };

    struct connector
    {

        void (*impl_ptr)();

        template<typename ConnImpl>
        connector(const ConnImpl & cn_impl) :  impl_ptr(&ConnImpl::disconnect)
        {
            ConnImpl::connect();
        }
        ~connector()
        {
            impl_ptr();
        }
    };

}
}

#define MW_VA_ARGS(...) ,##__VA_ARGS__

#define MW_WRAP_MAKE_SCOPE_OP(d, state, x) BOOST_PP_CAT(BOOST_PP_CAT(state, __mw__), x)
#define MW_WRAP_MAKE_SCOPE(...) BOOST_PP_CAT(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), BOOST_PP_LIST_FOLD_LEFT(MW_WRAP_MAKE_SCOPE_OP, , BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)))

#define MW_WRAP_FN_FIX_4(Scope, Name, Return, Signature)  \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) BOOST_PP_CAT(BOOST_PP_CAT(_mw_wrap_fn_fix_, MW_WRAP_MAKE_SCOPE(BOOST_PP_REMOVE_PARENS(Scope))), __mw__##Name)  \
    ( ::mw::detail::tag<BOOST_PP_REMOVE_PARENS(Return)> * MW_VA_ARGS Signature )


#define MW_WRAP_FN_FIX_3(Name, Return, Signature)  MW_WRAP_FN_FIX_4( (), Name, Return, Signature)

#define MW_WRAP_FN_FIX(...) BOOST_PP_OVERLOAD(MW_WRAP_FN_FIX_, __VA_ARGS__)(__VA_ARGS__)

#define MW_WRAP_MEM_FIX(Type, Name, Return, Signature) \
BOOST_SYMBOL_EXPORT Return BOOST_PP_CAT(_mw_wrap_mem_fix_, Name) (BOOST_PP_REMOVE_PARENS(Type) * const this_ MW_VA_ARGS Signature, ::mw::detail::tag<Return> * = nullptr)

#define MW_WRAP_STATIC_MEM_FIX(Type, Name, Return, Signature) \
BOOST_SYMBOL_EXPORT Return BOOST_PP_CAT(_mw_wrap_static_mem_fix_, Name) (BOOST_PP_REMOVE_PARENS(Type) * const MW_VA_ARGS Signature, ::mw::detail::tag<Return> * = nullptr)

#define MW_WRAP_MAKE_CONNECTOR_IMPL(Value) ::mw::detail::connector_impl<decltype(Value), Value>()

#define MW_WRAP_FN_IMPL(Scope, Name, Return, Signature, FuncName) \
::mw::detail::connector BOOST_PP_CAT(_mw_wrap_connector, __COUNTER__) { MW_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName  \
    ( ::mw::detail::tag<BOOST_PP_REMOVE_PARENS(Return)> * MW_VA_ARGS Signature )


#define MW_WRAP_FN_4(Scope, Name, Return, Signature)  MW_WRAP_FN_IMPL(Scope, Name, Return, Signature, BOOST_PP_CAT(_mw_wrap_fn_ , BOOST_PP_CAT( MW_WRAP_MAKE_SCOPE(BOOST_PP_REMOVE_PARENS(Scope)), BOOST_PP_CAT(_##Name##_, __LINE__))))
#define MW_WRAP_FN_3(Name, Return, Signature)         MW_WRAP_FN_4( (), Name, Return, Signature)

#define MW_WRAP_FN(...) BOOST_PP_OVERLOAD(MW_WRAP_FN_, __VA_ARGS__)(__VA_ARGS__)

#define MW_WRAP_MEM_IMPL(Type, Name, Return, Signature, FuncName) \
::mw::detail::connector BOOST_PP_CAT(_mw_wrap_connector, __COUNTER__) { MW_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName (BOOST_PP_REMOVE_PARENS(Type) * const this_, ::mw::detail::tag<BOOST_PP_REMOVE_PARENS(Return)> * MW_VA_ARGS Signature )

#define MW_WRAP_MEM(Type, Name, Return, Signature)  MW_WRAP_MEM_IMPL(Type, Name, Return, Signature, BOOST_PP_CAT(_mw_wrap_mem_##Name##_, __LINE__))


#define MW_WRAP_STATIC_MEM_IMPL(Type, Name, Return, Signature, FuncName) \
::mw::detail::connector BOOST_PP_CAT(_mw_wrap_connector, __COUNTER__) { MW_WRAP_MAKE_CONNECTOR_IMPL (& std::remove_pointer_t<decltype(this)>:: FuncName )}; \
BOOST_SYMBOL_EXPORT BOOST_PP_REMOVE_PARENS(Return) FuncName (BOOST_PP_REMOVE_PARENS(Type) * const, ::mw::detail::tag<BOOST_PP_REMOVE_PARENS(Return)> * MW_VA_ARGS Signature )

#define MW_WRAP_STATIC_MEM(Type, Name, Return, Signature)  MW_WRAP_STATIC_MEM_IMPL(Type, Name, Return, Signature, BOOST_PP_CAT(_mw_wrap_static_mem_##Name##_, __LINE__))

