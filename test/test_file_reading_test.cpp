#define BOOST_TEST_MODULE "test_file_reading"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "toml.hpp"
#include <fstream>

BOOST_AUTO_TEST_CASE(test_example)
{
    std::ifstream ifs("example.toml");
    if(ifs.good())
    {
        toml::Data data = toml::parse(ifs);
        std::string title = toml::get<toml::String>(data["title"]);
        BOOST_CHECK_EQUAL(title, std::string("TOML Example"));

        toml::Table owner = toml::get<toml::Table>(data["owner"]);
        std::string owner_name = toml::get<toml::String>(owner["name"]);
        BOOST_CHECK_EQUAL(owner_name, std::string("Tom Preston-Werner"));
        std::string owner_organization = toml::get<toml::String>(owner["organization"]);
        BOOST_CHECK_EQUAL(owner_organization, std::string("GitHub"));
        std::string owner_bio = toml::get<toml::String>(owner["bio"]);
        BOOST_CHECK_EQUAL(owner_bio, std::string("GitHub Cofounder & CEO\nLikes tater tots and beer."));
        toml::chrono::system_clock::time_point owner_dob = toml::get<toml::Datetime>(owner["dob"]);
        std::tm t;
        t.tm_year = 79;
        t.tm_mon = 4;
        t.tm_mday = 27;
        t.tm_hour = 7;
        t.tm_min = 32;
        t.tm_sec = 0;
        const std::time_t gtime = std::mktime(&t);
        const std::time_t dob = toml::chrono::system_clock::to_time_t(owner_dob);
        BOOST_CHECK_EQUAL(gtime, dob);

        toml::Table database = toml::get<toml::Table>(data["database"]);
        std::string server = toml::get<toml::String>(database["server"]);
        BOOST_CHECK_EQUAL(server, std::string("192.168.1.1"));
#ifdef TOML_ENABLE_CXX11
        std::vector<toml::Integer> ports = toml::get<toml::Array<toml::Integer>>(database["ports"]);
#else 
        std::vector<toml::Integer> ports = toml::get<toml::Array<toml::Integer>::type>(database["ports"]);
#endif
        BOOST_CHECK_EQUAL(ports.at(0), 8001);
        BOOST_CHECK_EQUAL(ports.at(1), 8001);
        BOOST_CHECK_EQUAL(ports.at(2), 8002);

        toml::Integer connection_max = toml::get<toml::Integer>(database["connection_max"]);
        BOOST_CHECK_EQUAL(connection_max, 5000);
        bool enabled = toml::get<toml::Boolean>(database["enabled"]);
        BOOST_CHECK_EQUAL(enabled, true);

        toml::Table servers = toml::get<toml::Table>(data["servers"]);
        toml::Table servers_alpha = toml::get<toml::Table>(servers["alpha"]);
        {
            std::string ip = toml::get<toml::String>(servers_alpha["ip"]);
            BOOST_CHECK_EQUAL(ip, "10.0.0.1");
            std::string dc = toml::get<toml::String>(servers_alpha["dc"]);
            BOOST_CHECK_EQUAL(dc, "eqdc10");
        }
        toml::Table servers_beta = toml::get<toml::Table>(servers["beta"]);
        {
            std::string ip = toml::get<toml::String>(servers_beta["ip"]);
            BOOST_CHECK_EQUAL(ip, "10.0.0.2");
            std::string dc = toml::get<toml::String>(servers_beta["dc"]);
            BOOST_CHECK_EQUAL(dc, "eqdc10");
            std::string country = toml::get<toml::String>(servers_beta["country"]);
            BOOST_CHECK_EQUAL(country, "中国");
        }

        toml::Table clients = toml::get<toml::Table>(data["clients"]);
#ifdef TOML_ENABLE_CXX11
        std::vector<toml::ValueBase> clients_data =
            toml::get<toml::Array<toml::ValueBase> >(clients["data"]);
        std::vector<std::string> first_array =
            toml::get<toml::Array<toml::String> >(clients_data.at(0));
#else 
        std::vector<toml::ValueBase> clients_data =
            toml::get<toml::Array<toml::ValueBase>::type>(clients["data"]);
        std::vector<std::string> first_array =
            toml::get<toml::Array<toml::String>::type>(clients_data.at(0));
#endif

        BOOST_CHECK_EQUAL(first_array.at(0), "gamma");
        BOOST_CHECK_EQUAL(first_array.at(1), "delta");
#ifdef TOML_ENABLE_CXX11
        std::vector<toml::Integer> second_array =
            toml::get<toml::Array<toml::Integer>>(clients_data.at(1));
#else 
        std::vector<toml::Integer> second_array =
            toml::get<toml::Array<toml::Integer>::type>(clients_data.at(1));
#endif
        BOOST_CHECK_EQUAL(second_array.at(0), 1);
        BOOST_CHECK_EQUAL(second_array.at(1), 2);

#ifdef TOML_ENABLE_CXX11
        std::vector<std::string> hosts =
            toml::get<toml::Array<toml::String>>(clients["hosts"]);
#else 
        std::vector<std::string> hosts =
            toml::get<toml::Array<toml::String>::type>(clients["hosts"]);
#endif
        BOOST_CHECK_EQUAL(hosts.at(0), "alpha");
        BOOST_CHECK_EQUAL(hosts.at(1), "omega");

#ifdef TOML_ENABLE_CXX11
        std::vector<toml::Table> products = 
            toml::get<toml::Array<toml::Table>>(data["products"]);
#else 
        std::vector<toml::Table> products = 
            toml::get<toml::Array<toml::Table>::type>(data["products"]);
#endif
        {
            std::string name = toml::get<toml::String>(products.at(0)["name"]);
            BOOST_CHECK_EQUAL(name, "Hammer");
            toml::Integer sku = toml::get<toml::Integer>(products.at(0)["sku"]);
            BOOST_CHECK_EQUAL(sku, 738594937);
        }
        {
            std::string name = toml::get<toml::String>(products.at(1)["name"]);
            BOOST_CHECK_EQUAL(name, "Nail");
            toml::Integer sku = toml::get<toml::Integer>(products.at(1)["sku"]);
            BOOST_CHECK_EQUAL(sku, 284758393);
            std::string color = toml::get<toml::String>(products.at(1)["color"]);
            BOOST_CHECK_EQUAL(color, "gray");
        }
    }
    else
    {
        std::cerr << "file open error." << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(test_hard_example)
{
    std::ifstream ifs("hard_example.toml");
    if(ifs.good())
    {
        toml::Data data = toml::parse(ifs);
        toml::Table the = toml::get<toml::Table>(data["the"]);
        std::string test_string = toml::get<toml::String>(the["test_string"]);
        BOOST_CHECK_EQUAL(test_string, "You'll hate me after this - #");

        toml::Table the_hard = toml::get<toml::Table>(the["hard"]);
#ifdef TOML_ENABLE_CXX11
        std::vector<std::string> test_array =
            toml::get<toml::Array<toml::String>>(the_hard["test_array"]);
#else 
        std::vector<std::string> test_array =
            toml::get<toml::Array<toml::String>::type>(the_hard["test_array"]);
#endif
        BOOST_CHECK_EQUAL(test_array.at(0), "] ");
        BOOST_CHECK_EQUAL(test_array.at(1), " # ");

        std::vector<std::string> test_array2 =
#ifdef TOML_ENABLE_CXX11
            toml::get<toml::Array<toml::String>>(the_hard["test_array2"]);
#else 
            toml::get<toml::Array<toml::String>::type>(the_hard["test_array2"]);
#endif
        BOOST_CHECK_EQUAL(test_array2.at(0), "Test #11 ]proved that");
        BOOST_CHECK_EQUAL(test_array2.at(1), "Experiment #9 was a success");

        std::string another_test_string = 
            toml::get<toml::String>(the_hard["another_test_string"]);
        BOOST_CHECK_EQUAL(another_test_string, " Same thing, but with a string #");
        std::string harder_test_string = 
            toml::get<toml::String>(the_hard["harder_test_string"]);
        BOOST_CHECK_EQUAL(harder_test_string, " And when \"\'s are in the string, along with # \"");

        toml::Table the_hard_bit = toml::get<toml::Table>(the_hard["bit#"]);
        std::string what = toml::get<toml::String>(the_hard_bit["what?"]);
        BOOST_CHECK_EQUAL(what, "You don't think some user won't do that?");
        std::vector<std::string> multi_line_array =
#ifdef TOML_ENABLE_CXX11
            toml::get<toml::Array<toml::String>>(the_hard_bit["multi_line_array"]);
#else 
            toml::get<toml::Array<toml::String>::type>(the_hard_bit["multi_line_array"]);
#endif
        BOOST_CHECK_EQUAL(multi_line_array.at(0), "]");
    }
    else
    {
        std::cerr << "file open error." << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(test_hard_example_error)
{
    {
    std::ifstream err("hard_example_error1.toml");
    if(err.good())
    {
        bool exception_thrown = false;
        try{toml::Data data = toml::parse(err);}
        catch(toml::syntax_error& except)
        {
            exception_thrown = true;
        }
        BOOST_CHECK(exception_thrown);
    }
    else
    {
        std::cerr << "file open error. hard_example_error.toml" << std::endl;
    }
    }

    {
    std::ifstream err("hard_example_error2.toml");
    if(err.good())
    {
        bool exception_thrown = false;
        try{toml::Data data = toml::parse(err);}
        catch(toml::syntax_error& except)
        {
            exception_thrown = true;
        }
        BOOST_CHECK(exception_thrown);
    }
    else
    {
        std::cerr << "file open error. hard_example_error.toml" << std::endl;
    }
    }

    {
    std::ifstream err("hard_example_error3.toml");
    if(err.good())
    {
        bool exception_thrown = false;
        try{toml::Data data = toml::parse(err);}
        catch(toml::syntax_error& except)
        {
            exception_thrown = true;
        }
        BOOST_CHECK(exception_thrown);
    }
    else
    {
        std::cerr << "file open error. hard_example_error.toml" << std::endl;
    }
    }
}

BOOST_AUTO_TEST_CASE(test_hard_example_unicode)
{
    std::ifstream ifs("hard_example_unicode.toml");
    if(ifs.good())
    {
        toml::Data data = toml::parse(ifs);
        toml::Table the = toml::get<toml::Table>(data["the"]);
        std::string test_string = toml::get<toml::String>(the["test_string"]);
        BOOST_CHECK_EQUAL(test_string, "Ýôú'ℓℓ λáƭè ₥è áƒƭèř ƭλïƨ - #");

        toml::Table the_hard = toml::get<toml::Table>(the["hard"]);
        std::vector<std::string> test_array =
#ifdef TOML_ENABLE_CXX11
            toml::get<toml::Array<toml::String>>(the_hard["test_array"]);
#else 
            toml::get<toml::Array<toml::String>::type>(the_hard["test_array"]);
#endif
        BOOST_CHECK_EQUAL(test_array.at(0), "] ");
        BOOST_CHECK_EQUAL(test_array.at(1), " # ");

        std::vector<std::string> test_array2 =
#ifdef TOML_ENABLE_CXX11
            toml::get<toml::Array<toml::String>>(the_hard["test_array2"]);
#else 
            toml::get<toml::Array<toml::String>::type>(the_hard["test_array2"]);
#endif
        BOOST_CHECK_EQUAL(test_array2.at(0), "Tèƨƭ #11 ]ƥřôƲèδ ƭλáƭ");
        BOOST_CHECK_EQUAL(test_array2.at(1), "Éжƥèřï₥èñƭ #9 ωáƨ á ƨúççèƨƨ");

        std::string another_test_string = 
            toml::get<toml::String>(the_hard["another_test_string"]);
        BOOST_CHECK_EQUAL(another_test_string, "§á₥è ƭλïñϱ, βúƭ ωïƭλ á ƨƭřïñϱ #");
        std::string harder_test_string = 
            toml::get<toml::String>(the_hard["harder_test_string"]);
        BOOST_CHECK_EQUAL(harder_test_string, " Âñδ ωλèñ \"'ƨ ářè ïñ ƭλè ƨƭřïñϱ, áℓôñϱ ωïƭλ # \"");

        toml::Table the_hard_bit = toml::get<toml::Table>(the_hard["βïƭ#"]);
        std::string what = toml::get<toml::String>(the_hard_bit["ωλáƭ?"]);
        BOOST_CHECK_EQUAL(what, "Ýôú δôñ'ƭ ƭλïñƙ ƨô₥è úƨèř ωôñ'ƭ δô ƭλáƭ?");
        std::vector<std::string> multi_line_array =
#ifdef TOML_ENABLE_CXX11
            toml::get<toml::Array<toml::String>>(the_hard_bit["multi_line_array"]);
#else 
            toml::get<toml::Array<toml::String>::type>(the_hard_bit["multi_line_array"]);
#endif
        BOOST_CHECK_EQUAL(multi_line_array.at(0), "]");
    }
    else
    {
        std::cerr << "file open error." << std::endl;
    }
}
