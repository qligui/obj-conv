#include <iostream>
#include <vector>
#include "sw-protocol/sw_protocol.hpp"
/*
 *A:key 取别名
 *O:建立映射关系
 */
using namespace std;
//struct User
//{
//    int64_t id;
//    std::string  name;
//    std::string  mail;
//    User(int64_t i = 0, const string& n = "", const string& m = "") :id(i), name(n), mail(m) {}
//    SW_PROTOCOL(A(id, "card_id"), O(name, mail))
//};
//struct Company {
//    std::string  name;
//    int64_t master;
//    std::vector<User> members;
//    Company() :master(0) {}
//    SW_PROTOCOL(O(name, master, members))
//};

struct User
{
    int64_t id;
    std::string  name;
    std::string  mail;
    User(int64_t i = 0, const string& n = "", const string& m = "") :id(i), name(n), mail(m) {}
public:
    swjson::condition_t cond_t_;
    template<typename _doc> void xml_to_struct(_doc& obj)
    {
        obj.convert(swjson::alias_name_conversion("id", "card_id").c_str(), this->id);
        obj.convert("name", this->name); obj.convert("mail", this->mail);
    }
    template <typename _obj_type>
    void struct_to_xml(_obj_type& obj, const char* root) const
    {
        obj.convert(swjson::alias_name_conversion("id", "card_id").c_str(), this->id);
        obj.convert("name", this->name); obj.convert("mail", this->mail);
    }
};
struct Company {
    std::string  name;
    int64_t master;
    std::vector<User> members;
    User person;
    Company() :master(0) {}
public:
    swjson::condition_t cond_t_;
    template<typename _doc>
    void xml_to_struct(_doc& obj) {
        obj.convert("name", this->name);
        obj.convert("master", this->master);
        obj.convert("members", this->members);
        obj.convert("person", this->person);
    }
    template <typename _obj_type>
    void struct_to_xml(_obj_type& obj, const char* root) const
    {
        obj.convert("name", this->name);
        obj.convert("master", this->master);
        obj.convert("members", this->members);
        obj.convert("person", this->person);
    }
};
int main(int argc, char* argv[])
{
    Company compay;
    compay.name = "cg";
    compay.master = 2020;
    compay.members.resize(2);
    compay.members[0] = User(1, "xxxxx", "xxx@gmail.com");
    compay.members[1] = User(2, "yyyyyy", "yyy@gmail.com");
    compay.person = User(1, "zzzzzz", "zzz@gmail.com");
    //auto xml_str = swxml::obj_to_xml(compay, "config");
    swxml::obj_to_save_xml_file("./example.xml", compay, "config");
    //std::cout << xml_str << std::endl;
    Company cmpy2;
    swxml::xml_to_obj("./example.xml", cmpy2, false, true);
    getchar();
}
