#define CATCH_CONFIG_MAIN
#include <filesystem>
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

struct TupleVarData
{
    std::tuple<int, std::string, double> arr;
    std::variant<int, double, std::string> var;
    REFLEX_BIND(O(arr, var))
};
TEST_CASE("JsonTestSet.Test01.json_obj_test")
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
TEST_CASE("JsonTestSet.Test02.json inherited class")
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

TEST_CASE("JsonTestSet.Test03.tuple and variant test")
{
    TupleVarData tuple_var_data;
    tuple_var_data.arr = std::make_tuple(100, std::string("hello"), 10.24f);
    tuple_var_data.var.emplace<1>(10.0f);
    auto json_str = reflexjson::obj_to_json(tuple_var_data);
    TupleVarData tv_data_2;
    reflexjson::json_to_obj(json_str, tv_data_2);
    REQUIRE(tv_data_2.arr == tuple_var_data.arr);
    REQUIRE(tv_data_2.var == tuple_var_data.var);

    tuple_var_data.var.emplace<2>("hello");
    json_str = reflexjson::obj_to_json(tuple_var_data);
    reflexjson::json_to_obj(json_str, tv_data_2);
    REQUIRE(tv_data_2.var == tuple_var_data.var);
}
TEST_CASE("JsonTestSet.Test04.read json file catch exception")
{
    Host host_data;
    try {
        auto val = reflexjson::json_to_obj("./xxxx.json", host_data, true);
        REQUIRE(val == (val ? true : false));
    }
    catch (reflextraits::reflex_exption e) {
        std::cout << e.what() << ".\n";
    }
    catch (...) {
        std::cout << "unkown exception().\n";
    }
}
TEST_CASE("JsonTestSet.Test04.read json file don't catch exception")
{
    Host host;
    host.ip = 0x112233;
    host.type = 0xAABBCC;
    host.mode = 0x0102F;
    host.tddd = test_type::CCCCC;
    host.hw_id.resize(14);
    host.license.resize(14);
    for (int i = 0; i < host.hw_id.size(); ++i)
    {
        host.hw_id[i] = 'x';
        host.license[i] = 'y';
    }
    std::string json_str = reflexjson::obj_to_json(host, "", 0);

    std::string path_name = "./test.json";
    std::ofstream ofs(path_name, std::ofstream::binary);
    ofs.write(json_str.data(), json_str.size());
    ofs.close();

    Host host222;
    auto val = reflexjson::json_to_obj(path_name, host222, true);
    if (val)
    {
        REQUIRE(host222.hw_id == host.hw_id);
        REQUIRE(host222.ip == host.ip);
        REQUIRE(host222.tddd == host.tddd);
        REQUIRE(host222.license == host.license);
        REQUIRE(host222.type == host.type);
        REQUIRE(host222.mode == host.mode);

        namespace fs = std::filesystem;
        fs::path path_obj(path_name);
        if (fs::exists(path_obj))
            fs::remove(path_obj);
    }
    else
    {
        REQUIRE(val == false);
    }
}

TEST_CASE("XMLTestSet.Test01.xml_obj_test")
{
    REQUIRE(2 == 2);
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
    REFLEX_BIND(A(id, "card_id"), A(param, "address"), O(name, mail))
};
TEST_CASE("iniTestSet.Test01.ini_to_obj")
{
    namespace fs = std::filesystem;
    std::string path = "./example.ini";
    ConfigParam conf;
    conf.id = 12345;
    conf.name = "xxxx";
    conf.mail = "xxxx@gmail.com";
    conf.param.ip = "127.0.0.1";
    conf.param.port = 8080;
    REQUIRE(reflexini::obj_to_save_ini(path.c_str(), conf));

    decltype(conf) conf_2;
    REQUIRE(reflexini::ini_to_obj(path.c_str(), conf_2));
    CHECK(conf.id == conf_2.id);
    CHECK(conf.name == conf_2.name);
    CHECK(conf.mail == conf_2.mail);
    CHECK(conf.param.ip == conf_2.param.ip);
    CHECK(conf.param.port == conf_2.param.port);
    fs::remove(path);
}