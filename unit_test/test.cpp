#define CATCH_CONFIG_MAIN
#include <reflex_format/reflex_format.hpp>
#include <unit_test/catch.hpp>

enum class test_type : int32_t
{
    AAAA = 0,
    BBBBB,
    CCCCC,
};
struct Host
{
    std::vector<char> hw_id;
    std::vector<char> license;
    test_type tddd;
    uint32_t ip;
    uint32_t type;
    uint32_t mode;
    std::vector<int> reserve;
    REFLEX_BIND(O(hw_id, license, ip, type, mode, tddd))
};

TEST_CASE("json_obj_test")
{
	Host host;
	host.tddd = test_type::CCCCC;
	host.hw_id.resize(14);
	host.license.resize(14);
	for (int i = 0; i < host.hw_id.size(); ++i)
	{
		host.hw_id[i] = 'x';
		host.license[i] = 'x';
	}
	auto json_str = swjson::obj_to_json(host);
	std::cout << json_str << std::endl;
	Host host222;
	swjson::json_to_obj(json_str, host222);
	REQUIRE(host222.hw_id == host.hw_id);
	REQUIRE(host222.ip == host.ip);
	REQUIRE(host222.tddd == host.tddd);
	REQUIRE(host222.license == host.license);
	REQUIRE(host222.type == host.type);
	REQUIRE(host222.mode == host.mode);
}

TEST_CASE("xml_obj_test")
{
	REQUIRE(2 == 2);
}