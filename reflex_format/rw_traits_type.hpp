#ifndef RW_TRAITS_TYPE_HPP_
#define RW_TRAITS_TYPE_HPP_
#include <type_traits>

namespace reflextraits {
/* c++11 before implement:traits(url:https://izualzhy.cn/SFINAE-and-enable_if)
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
*/
//C++17 implement
template<typename, typename=std::void_t<>>
struct has_member_condition : std::false_type{ };
template<typename _type>
struct has_member_condition<_type, std::void_t<decltype(&_type::cond_t_)>> : std::true_type{ };

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
        printf("Error:alais name empty()\n");
        return "xxxxxx---xxxxx----error-name---xxxxx--xxxxx";
    }
    return alias_name;
}
//reference https://qastack.cn/programming/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
 template<std::size_t TypeNum = 0, typename Func, typename... Tp>
 inline typename std::enable_if<TypeNum == sizeof...(Tp), void>::type
     for_each_tuple(std::tuple<Tp...>&, Func) // Unused arguments are given no names.
 { }

 template<std::size_t TypeNum = 0, typename FuncT, typename... Tp>
 inline typename std::enable_if < TypeNum < sizeof...(Tp), void>::type
     for_each_tuple(std::tuple<Tp...>& args, FuncT func)
 {
     func(std::get<TypeNum>(args));
     for_each_tuple<TypeNum + 1, FuncT, Tp...>(args, func);
 }
}
#endif//RW_TRAITS_TYPE_HPP_