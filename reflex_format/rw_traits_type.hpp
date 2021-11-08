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

//reference https://qastack.cn/programming/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
//unused arguments are given no names.
template<std::size_t type_num = 0, typename _func_type, typename... _type>
inline typename std::enable_if<type_num == sizeof...(_type), void>::type
for_each_tuple(std::tuple<_type...>&, _func_type){ }

template<std::size_t type_num = 0, typename _func_type, typename... _type>
inline typename std::enable_if < type_num < sizeof...(_type), void>::type
    for_each_tuple(std::tuple<_type...>& args, _func_type func)
{
    func(std::get<type_num>(args));
    for_each_tuple<type_num + 1, _func_type, _type...>(args, func);
}

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