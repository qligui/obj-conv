#include <iostream>
#include <vector>
#include "sw-json/sw_json.hpp"
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
	SW_JSON(A(id, "card_id"), O(name, mail))
};
struct Company {
	std::string  name;
	int64_t master;
	std::vector<User> members;
	Company() :master(0){}
	SW_JSON(O(name, master, members))
};

int main(int argc, char* argv[])
{
	Company compay;
	compay.name = "公司名称";
	compay.master = 2020;
	compay.members.resize(2);
	compay.members[0] = User(1, "xxxxx", "xxx@gmail.com");
	compay.members[1] = User(2, "yyyyyy", "yyy@gmail.com");
	auto json_str = swjson::obj_to_json(compay);
	std::cout << json_str << std::endl;
	Company cmpy2;
	swjson::json_to_obj(json_str, cmpy2);
}