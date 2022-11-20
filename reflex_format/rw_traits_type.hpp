#ifndef RW_TRAITS_TYPE_HPP_
#define RW_TRAITS_TYPE_HPP_
#include <type_traits>
#include <exception>

namespace reflextraits 
{
#if __cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L)
    //C++14 implement
    template<typename, typename = std::void_t<>>
    struct has_member_condition : std::false_type { };
    template<typename _type>
    struct has_member_condition<_type, std::void_t<decltype(&_type::cond_t_)>> : std::true_type { };
    template<typename _type>
    constexpr bool has_member_condition_v = has_member_condition<_type>::value;
#else
//reference c++11 before implement:traits(url:https://izualzhy.cn/SFINAE-and-enable_if)
template<typename _type>
struct has_member_condition
{
    typedef char yes;
    typedef char no[2];

    template <typename _c>
    static yes& check(decltype(&_c::cond_t_));
    template <typename>
    static no& check(...);

    static bool const value = sizeof(check<_type>(0)) == sizeof(yes);
};
#endif
//type
struct condition_t
{
    typedef bool (*cond_func)(void* obj, void* doc);

    void* parent_;
    cond_func cond_func_;

    condition_t() : parent_(nullptr), cond_func_(nullptr) {}
    void set_value(void* p, cond_func func)
    {
        parent_ = p;
        cond_func_ = func;
    }
};
#ifdef __linux__
 __attribute__((unused)) 
 #endif
static std::string alias_name_conversion(const std::string& key, const std::string& alias_name)
{
    if (alias_name.empty())
    {
        printf("error: alais name empty()\n");
        return "xxxxxx---xxxxx----error-name---xxxxx--xxxxx";
    }
    return alias_name;
}
#if __cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L)
 template<typename _tuple_type, typename _func_type, size_t... _nums>
 void func_tuple(_tuple_type& arg, _func_type&& func, std::index_sequence<_nums...>)
 {
     (func(std::get<_nums>(arg)), ...);
 }
template<typename... _type_args, typename _func_type>
void for_each_tuple(std::tuple<_type_args...>& args, _func_type&& func)
{
    func_tuple(args, func, std::make_index_sequence<sizeof...(_type_args)>{ });
}
#else
//reference https://qastack.cn/programming/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
//unused arguments are given no names.
 template<std::size_t type_num = 0, typename _func_type, typename... _type>
 inline typename std::enable_if<type_num == sizeof...(_type), void>::type
     for_each_tuple(std::tuple<_type...>&, _func_type) { }

template<std::size_t type_num = 0, typename _func_type, typename... _type>
inline typename std::enable_if < type_num < sizeof...(_type), void>::type
    for_each_tuple(std::tuple<_type...>& args, _func_type func)
{
    func(std::get<type_num>(args));
    for_each_tuple<type_num + 1, _func_type, _type...>(args, func);
}
#endif
//reference:https://stackoverflow.com/questions/59103484/why-can-i-not-retrieve-the-index-of-a-variant-and-use-that-to-get-its-content
//get variant index
template<typename _variant_type, typename _type, std::size_t index = 0>
constexpr std::size_t variant_index() {
    //static_assert(std::variant_size_v<_variant_type> > index, "type not found in variant");
    if (std::variant_size_v<_variant_type> <= index)
        return -1;
    if constexpr (index == std::variant_size_v<_variant_type>) {
        return index;
    }
    else if constexpr (std::is_same_v<std::variant_alternative_t<index, _variant_type>, _type>) {
        return index;
    }
    else {
        return variant_index<_variant_type, _type, index + 1>();
    }
}
template<typename _variant_type, typename _type>
constexpr bool has_variant_type_v = variant_index<_variant_type, _type>() != -1;

class reflex_exption : public std::exception
{
public:
    reflex_exption(const std::string& err_msg) : err_msg_(err_msg) {}
    ~reflex_exption() { }
    virtual char const* what() const throw()
    {
        return err_msg_.c_str();
    }
private:
    std::string err_msg_;
};
}
#endif//RW_TRAITS_TYPE_HPP_