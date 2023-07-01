#include <iostream>
#include <vector>
#include "reflex_format/reflex_format.hpp"

//#define XML_CONVERT
//#define JSON_CONVERT
#define INI_CONVERT
/*
 *A:key 取别名
 *O:建立映射关系
 */
using namespace std;
struct User
{
    int64_t id;
    std::string  name;
    std::string  mail;
    User(int64_t i = 0, const string& n = "", const string& m = "") :id(i), name(n), mail(m) {}
    REFLEX_BIND(A(id, "card_id"), O(name, mail))
};
struct Company 
{
    std::string  name;
    int64_t master;
    std::vector<User> members;
    User person;
    Company() :master(0) {}
    REFLEX_BIND(O(name, master, members, person))
};

struct Worker : public Company
{
    std::string address;
    REFLEX_BIND(I(Company), O(address))
};
void ini_demo();
/*
struct User
{
    int64_t id;
    std::string  name;
    std::string  mail;
    User(int64_t i = 0, const string& n = "", const string& m = "") :id(i), name(n), mail(m) {}
public:
    reflextraits::condition_t cond_t_;
    template<typename _doc> void xml_to_struct(_doc& obj)
    {
        obj.convert(swtraits::alias_name_conversion("id", "card_id").c_str(), this->id);
        obj.convert("name", this->name); obj.convert("mail", this->mail);
    }
    template <typename _obj_type>
    void struct_to_xml(_obj_type& obj, const char* root) const
    {
        obj.convert(swtraits::alias_name_conversion("id", "card_id").c_str(), this->id);
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
    reflextraits::condition_t cond_t_;
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
struct Worker : public Company
{
    std::string address;
public:
    reflextraits::condition_t cond_t_;
    template<typename _doc>
    void obj_to_struct(_doc& obj)
    {
        Company::obj_to_struct(obj);
        obj.convert("address", this->address);
    }
    template <typename _obj_type>
    void struct_to_obj(_obj_type& obj, const char* root) const
    {
        Company::struct_to_obj(obj, root);
        obj.convert("address", this->address);
    }
}*/
int main(int argc, char* argv[])
{
    Company compay;
    compay.name = "cg";
    compay.master = 2020;
    compay.members.resize(2);
    compay.members[0] = User(1, "xxxxx", "xxx@gmail.com");
    compay.members[1] = User(2, "yyyyyy", "yyy@gmail.com");
    compay.person = User(1, "zzzzzz", "zzz@gmail.com");
#ifdef XML_CONVERT
    auto xml_str = reflexxml::obj_to_xml(compay, "config");
    reflexxml::obj_to_save_xml_file("./example.xml", compay, "config");
    std::cout << xml_str << std::endl;
    Company cmpy2;
    reflexxml::xml_to_obj("./example.xml", cmpy2, false, true);
#endif
#ifdef JSON_CONVERT
    auto json_str = reflexjson::obj_to_json(compay);
    //std::cout << json_str << std::endl;
    json_str = "{\"name\":\"cg\",\"master\":2020, \"members\":[{\"card_id\":1,\"name\":\"xxxxx\",\"mail\":\"xxx@gmail.com\"},"
    "{\"card_id\":2,\"name\":\"yyyyyy\",\"mail\":\"yyy@gmail.com\"}],\"person\":{\"card_id\":1,\"name\":\"zzzzzz\",\"mail\":\"zzz@gmail.com\"}}";
    Company cmpy3;
    try
    {
        reflexjson::json_to_obj(json_str, cmpy3);
    }
    catch (const std::exception& e)
    {
        printf("%s.", e.what());
    }
#endif
    ini_demo();
    system("pause");
}
//ini demo
struct OrgParam
{
    std::string ip;
    uint16_t port;
    REFLEX_BIND(O(ip, port))
};
struct ConfigParam
{
    int id;
    std::string name;
    std::string mail;
    OrgParam param;
    REFLEX_BIND(A(id, "card_id"),A(param, "address"), O(name, mail))
};
void ini_demo()
{
#ifdef INI_CONVERT
    std::string path = "./example.ini";
    ConfigParam conf;
    conf.id = 12345;
    conf.name = "xxxx";
    conf.mail = "xxxx@gmail.com";
    conf.param.ip = "127.0.0.1";
    conf.param.port = 8080;

    reflexini::obj_to_save_ini(path.c_str(), conf);
    reflexini::ini_to_obj(path.c_str(), conf);
#endif
}
