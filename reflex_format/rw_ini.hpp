#ifndef RW_INI_H_
#define RW_INI_H_
#include <stack>
#include <inipp/inipp.h>
#include "rw_traits_type.hpp"

namespace reflexini
{
using namespace reflextraits;
class RWriter
{
    using IniObject = inipp::Ini<char>;
    using Sessions = IniObject::Sections;
public:
    RWriter(){ }
    ~RWriter(){ }
protected:
    void obj_begin(const char* key)
    {
        std::string key_str = key == nullptr ? std::string{} : key;
        stack_session_key_.push(std::move(key_str));
    }
    void obj_end()
    {
        stack_session_key_.pop();
    }
protected:
    std::stack<std::string> stack_session_key_;
    std::unique_ptr<IniObject> ini_obj_{std::make_unique<IniObject>()};
};
class IniReader: public RWriter
{
public:
    IniReader(){ }
    ~IniReader(){ }
    void parse(const char* file_path)
    {
        std::ifstream is(file_path);
        ini_obj_->parse(is);
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_convertible_v<T, std::string>>>
    bool convert(const char* key, T& val)
    {
        const auto& session_key = stack_session_key_.top();
        auto& sessions = ini_obj_->sections[session_key];
        return inipp::get_value(sessions, key, val);
    }
    template<typename _type, std::enable_if_t<has_member_condition_v<_type>, bool> = true>
    bool convert(const char* key, _type& val)
    {
        this->obj_begin(key);
        val.obj_to_struct(*this);
        this->obj_end();
        return true;
    }
    template<typename ...>
    bool convert(...)
    {
        //throw std::exception("unkown type is not convert");
        return true;
    }
};
class IniWriter : public RWriter
{
public:
    IniWriter() = default;

    bool obj_to_ini(const char* file_path)
    {
        std::ofstream os(file_path);
        ini_obj_->generate(os);
        return true;
    }
    template<typename _type, typename = std::enable_if_t<
        std::is_arithmetic_v<_type> || 
        std::is_convertible_v<_type, std::string>>>
    IniWriter& convert(const char* key, const _type& val)
    {
        const auto& session_key = stack_session_key_.top();
        if constexpr (std::is_arithmetic_v<_type>)
        {
            ini_obj_->sections[session_key][key] = std::to_string(val);
        }
        else if constexpr (std::is_convertible_v<_type, std::string>)
        {
            ini_obj_->sections[session_key][key] = val;
        }
        else
        {
            throw std::exception("unkown type.");
        }
        return *this;
    }
    template<typename _type, std::enable_if_t<has_member_condition_v<_type>, bool> = true>
    IniWriter& convert(const char* key,const _type& val)
    {
        this->obj_begin(key);
        val.struct_to_obj(*this, key);
        this->obj_end();
        return *this;
    }
    template<typename ...>
    IniWriter& convert(...)
    {
        //throw std::exception("unkown type is not convert");
        return *this;
    }
};
} // namespace reflexini
#endif //RW_INI_H_