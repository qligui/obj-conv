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
struct Config 
{
    std::string url;
    REFLEX_BIND(O(url))
};

struct HostTest: Host,Config
{
    std::string other;
    REFLEX_BIND(O(other),I(Host, Config))
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
    auto json_str = reflexjson::obj_to_json(host);
    Host host222;
    reflexjson::json_to_obj(json_str, host222);
    REQUIRE(host222.hw_id == host.hw_id);
    REQUIRE(host222.ip == host.ip);
    REQUIRE(host222.tddd == host.tddd);
    REQUIRE(host222.license == host.license);
    REQUIRE(host222.type == host.type);
    REQUIRE(host222.mode == host.mode);
}
TEST_CASE("json inherited class")
{
    HostTest host;
    host.tddd = test_type::CCCCC;
    host.hw_id.resize(14);
    host.license.resize(14);
    host.url = "https://www.baidu.com";
    host.other = "other name";
    for (int i = 0; i < host.hw_id.size(); ++i)
    {
        host.hw_id[i] = 'x';
        host.license[i] = 'x';
    }
    auto json_str = reflexjson::obj_to_json(host);
    HostTest host222;
    reflexjson::json_to_obj(json_str, host222);
    REQUIRE(host222.hw_id == host.hw_id);
    REQUIRE(host222.ip == host.ip);
    REQUIRE(host222.tddd == host.tddd);
    REQUIRE(host222.license == host.license);
    REQUIRE(host222.type == host.type);
    REQUIRE(host222.mode == host.mode);
    REQUIRE(host222.url == host.url);
    REQUIRE(host222.other == host.other);
}

TEST_CASE("xml_obj_test")
{
    REQUIRE(2 == 2);
}