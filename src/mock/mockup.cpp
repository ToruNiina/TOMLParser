// this is mockup of data structures and function that makes proper data structure.
#include "mockup.hpp"

int main()
{
    // [a.b.c.d] {{{
    {
    // case:
    // [a.b.c.d]
    // key = "value"
    std::vector<std::string> name{"a", "b", "c", "d"};
    auto contents = std::make_shared<table_value>();
    contents->value[std::string("key")] = std::make_shared<string_value>("value");
    auto root = make_nested_table(name.cbegin(), name.cend(), contents, false);

    auto data = std::dynamic_pointer_cast<table_value>(root);
    if(!data)
    {
        std::cerr << "no data" << std::endl;
        return 1;
    }

    auto a = std::dynamic_pointer_cast<table_value>(data->value["a"]);
    if(!a)
    {
        std::cerr << "no a" << std::endl;
        return 1;
    }

    auto b = std::dynamic_pointer_cast<table_value>(a->value["b"]);
    if(!b)
    {
        std::cerr << "no b" << std::endl;
        return 1;
    }

    auto c = std::dynamic_pointer_cast<table_value>(b->value["c"]);
    if(!c)
    {
        std::cerr << "no c" << std::endl;
        return 1;
    }

    auto d = std::dynamic_pointer_cast<table_value>(c->value["d"]);
    if(!d)
    {
        std::cerr << "no d" << std::endl;
        return 1;
    }

    auto key = std::dynamic_pointer_cast<string_value>(d->value["key"]);
    if(!key)
    {
        std::cerr << "no key" << std::endl;
        return 1;
    }

    std::cerr << "case: one nested table" << std::endl;
    std::cerr << "[a.b.c.d]" << std::endl;
    std::cerr << "  key = " << key->value << std::endl;
    std::cerr << std::endl;
    }// }}}

    // [[a.b.c.d]] * 2 {{{
    {
    std::vector<std::string> name{"a", "b", "c", "d"};
    auto contents = std::make_shared<table_value>();
    contents->value[std::string("key")] = std::make_shared<string_value>("value1");
    auto root = std::make_shared<table_value>();
    search_and_make_table(root, name.cbegin(), name.cend(), contents, true);

    auto contents2 = std::make_shared<table_value>();
    contents2->value[std::string("key")] = std::make_shared<string_value>("value2");
    search_and_make_table(root, name.cbegin(), name.cend(), contents2, true);

    auto data = std::dynamic_pointer_cast<table_value>(root);
    if(!data)
    {
        std::cerr << "no data" << std::endl;
        return 1;
    }

    auto a = std::dynamic_pointer_cast<table_value>(data->value["a"]);
    if(!a)
    {
        std::cerr << "no a" << std::endl;
        return 1;
    }

    auto b = std::dynamic_pointer_cast<table_value>(a->value["b"]);
    if(!b)
    {
        std::cerr << "no b" << std::endl;
        return 1;
    }

    auto c = std::dynamic_pointer_cast<table_value>(b->value["c"]);
    if(!c)
    {
        std::cerr << "no c" << std::endl;
        return 1;
    }

    auto d = std::dynamic_pointer_cast<array_value>(c->value["d"]);
    if(!d)
    {
        std::cerr << "no d" << std::endl;
        return 1;
    }

    auto abcd1 = std::dynamic_pointer_cast<table_value>(d->value.at(0));
    if(!abcd1)
    {
        std::cerr << "no abcd1" << std::endl;
        return 1;
    }
    auto key1 = std::dynamic_pointer_cast<string_value>(abcd1->value["key"]);
    if(!key1)
    {
        std::cerr << "no key1" << std::endl;
        return 1;
    }

    auto abcd2 = std::dynamic_pointer_cast<table_value>(d->value.at(1));
    if(!abcd2)
    {
        std::cerr << "no abcd2" << std::endl;
        return 1;
    }
    auto key2 = std::dynamic_pointer_cast<string_value>(abcd2->value["key"]);
    if(!key2)
    {
        std::cerr << "no key2" << std::endl;
        return 1;
    } 

    std::cerr << "case: nested array_of_table" << std::endl;
    std::cerr << "[[a.b.c.d]]" << std::endl;
    std::cerr << "  key = " << key1->value << std::endl;
    std::cerr << "[[a.b.c.d]]" << std::endl;
    std::cerr << "  key = " << key2->value << std::endl;
    std::cerr << std::endl;

    }
    // }}}

    // [a] [a.b] {{{
    {
    auto root = std::make_shared<table_value>();

    std::vector<std::string> name{"a"};
    auto contents = std::make_shared<table_value>();
    contents->value[std::string("key")] = std::make_shared<string_value>("value1");
    search_and_make_table(root, name.cbegin(), name.cend(), contents, false);

    std::vector<std::string> nested_name{"a", "b"};
    auto contents2 = std::make_shared<table_value>();
    contents2->value[std::string("key")] = std::make_shared<string_value>("value2");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents2, false);

    auto data = std::dynamic_pointer_cast<table_value>(root);
    if(!data)
    {
        std::cerr << "no data" << std::endl;
        return 1;
    }

    auto a = std::dynamic_pointer_cast<table_value>(data->value["a"]);
    if(!a)
    {
        std::cerr << "no a" << std::endl;
        return 1;
    }
    auto key1 = std::dynamic_pointer_cast<string_value>(a->value["key"]);
    if(!key1)
    {
        std::cerr << "no key1" << std::endl;
        return 1;
    }
    auto ab = std::dynamic_pointer_cast<table_value>(a->value["b"]);
    if(!ab)
    {
        std::cerr << "no a.b" << std::endl;
        return 1;
    }
    auto key2 = std::dynamic_pointer_cast<string_value>(ab->value["key"]);
    if(!key2)
    {
        std::cerr << "no key2" << std::endl;
        return 1;
    }

    std::cerr << "case: one key-value pair and one nested table" << std::endl;
    std::cerr << "[a]" << std::endl;
    std::cerr << "key = " << key1->value << std::endl;
    std::cerr << "[a.b]" << std::endl;
    std::cerr << "key = " << key2->value << std::endl;
    std::cerr << std::endl;
    }
    // }}}

    // [a] [[a.b]] * 2 {{{
    {
    auto root = std::make_shared<table_value>();

    std::vector<std::string> name{"a"};
    auto contents = std::make_shared<table_value>();
    contents->value[std::string("key")] = std::make_shared<string_value>("value1");
    search_and_make_table(root, name.cbegin(), name.cend(), contents, false);

    std::vector<std::string> nested_name{"a", "b"};
    auto contents2 = std::make_shared<table_value>();
    contents2->value[std::string("key")] = std::make_shared<string_value>("value2");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents2, true);

//     std::vector<std::string> nested_name{"a", "b"};
    auto contents3 = std::make_shared<table_value>();
    contents3->value[std::string("key")] = std::make_shared<string_value>("value3");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents3, true);

    auto data = std::dynamic_pointer_cast<table_value>(root);
    if(!data)
    {
        std::cerr << "no data" << std::endl;
        return 1;
    }

    auto a = std::dynamic_pointer_cast<table_value>(data->value["a"]);
    if(!a)
    {
        std::cerr << "no a" << std::endl;
        return 1;
    }
    auto key1 = std::dynamic_pointer_cast<string_value>(a->value["key"]);
    if(!key1)
    {
        std::cerr << "no key1" << std::endl;
        return 1;
    }
    auto ab_arr = std::dynamic_pointer_cast<array_value>(a->value["b"]);
    if(!ab_arr)
    {
        std::cerr << "no a.b" << std::endl;
        return 1;
    }
    auto ab0 = std::dynamic_pointer_cast<table_value>(ab_arr->value.at(0));
    if(!ab0)
    {
        std::cerr << "no a.b[0]" << std::endl;;
        return 1;
    }
    auto key2 = std::dynamic_pointer_cast<string_value>(ab0->value["key"]);
    if(!key2)
    {
        std::cerr << "no key2" << std::endl;
        return 1;
    }
    auto ab1 = std::dynamic_pointer_cast<table_value>(ab_arr->value.at(1));
    if(!ab1)
    {
        std::cerr << "no a.b[1]" << std::endl;;
        return 1;
    }
    auto key3 = std::dynamic_pointer_cast<string_value>(ab1->value["key"]);
    if(!key3)
    {
        std::cerr << "no key3" << std::endl;
        return 1;
    }

    std::cerr << "case: one key-value pair and one nested table" << std::endl;
    std::cerr << "[a]" << std::endl;
    std::cerr << "key = " << key1->value << std::endl;
    std::cerr << "[[a.b]]" << std::endl;
    std::cerr << "key = " << key2->value << std::endl;
    std::cerr << "[[a.b]]" << std::endl;
    std::cerr << "key = " << key3->value << std::endl;
    std::cerr << std::endl;
    }
    // }}}

    // [[a]] *2 [b] {{{
    {
    auto root = std::make_shared<table_value>();

    std::vector<std::string> name{"a"};
    auto contents1 = std::make_shared<table_value>();
    contents1->value[std::string("key")] = std::make_shared<string_value>("value1");
    search_and_make_table(root, name.cbegin(), name.cend(), contents1, true);

    std::vector<std::string> nested_name{"a", "b"};
    auto contents2 = std::make_shared<table_value>();
    contents2->value[std::string("key")] = std::make_shared<string_value>("value2");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents2, false);

    auto contents3 = std::make_shared<table_value>();
    contents3->value[std::string("key")] = std::make_shared<string_value>("value3");
    search_and_make_table(root, name.cbegin(), name.cend(), contents3, true);

    auto contents4 = std::make_shared<table_value>();
    contents4->value[std::string("key")] = std::make_shared<string_value>("value4");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents4, false);

    auto data = std::dynamic_pointer_cast<table_value>(root);
    if(!data)
    {
        std::cerr << "no data" << std::endl;
        return 1;
    }

    auto a_arr = std::dynamic_pointer_cast<array_value>(data->value["a"]);
    if(!a_arr)
    {
        std::cerr << "no a_arr" << std::endl;
        return 1;
    }

    auto a0 = std::dynamic_pointer_cast<table_value>(a_arr->value.at(0));
    if(!a0)
    {
        std::cerr << "no a0" << std::endl;
        return 1;
    }
    auto key1 = std::dynamic_pointer_cast<string_value>(a0->value["key"]);
    if(!key1)
    {
        std::cerr << "no key1" << std::endl;
        return 1;
    }
    auto ab0 = std::dynamic_pointer_cast<table_value>(a0->value["b"]);
    if(!ab0)
    {
        std::cerr << "no a[0].b" << std::endl;
        return 1;
    }
    auto key2 = std::dynamic_pointer_cast<string_value>(ab0->value["key"]);
    if(!key2)
    {
        std::cerr << "no a[0].b[key]" << std::endl;
        return 1;
    }

    auto a1 = std::dynamic_pointer_cast<table_value>(a_arr->value.at(1));
    if(!a1)
    {
        std::cerr << "no a1" << std::endl;
        return 1;
    }
    auto key3 = std::dynamic_pointer_cast<string_value>(a1->value["key"]);
    if(!key3)
    {
        std::cerr << "no key3" << std::endl;
        return 1;
    }
    auto ab1 = std::dynamic_pointer_cast<table_value>(a1->value["b"]);
    if(!ab1)
    {
        std::cerr << "no a[1].b" << std::endl;
        return 1;
    }
    auto key4 = std::dynamic_pointer_cast<string_value>(ab1->value["key"]);
    if(!key4)
    {
        std::cerr << "no a[1].b[key]" << std::endl;
        return 1;
    }

    std::cerr << "case: array_of_table and its subtable" << std::endl;
    std::cerr << "[[a]]" << std::endl;
    std::cerr << "key = " << key1->value << std::endl;
    std::cerr << "[a.b]" << std::endl;
    std::cerr << "key = " << key2->value << std::endl;
    std::cerr << "[[a]]" << std::endl;
    std::cerr << "key = " << key3->value << std::endl;
    std::cerr << "[a.b]" << std::endl;
    std::cerr << "key = " << key4->value << std::endl;
    }
    // }}}
    
    // [[a]] *2 [[b]] * 2, 1 {{{
    {
    auto root = std::make_shared<table_value>();

    const std::vector<std::string> name{"a"};
    const std::vector<std::string> nested_name{"a", "b"};

    auto contents1 = std::make_shared<table_value>();
    contents1->value[std::string("key")] = std::make_shared<string_value>("value1");
    search_and_make_table(root, name.cbegin(), name.cend(), contents1, true);

    auto contents2 = std::make_shared<table_value>();
    contents2->value[std::string("key")] = std::make_shared<string_value>("value2");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents2, true);

    auto contents3 = std::make_shared<table_value>();
    contents3->value[std::string("key")] = std::make_shared<string_value>("value3");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents3, true);

    auto contents4 = std::make_shared<table_value>();
    contents4->value[std::string("key")] = std::make_shared<string_value>("value4");
    search_and_make_table(root, name.cbegin(), name.cend(), contents4, true);

    auto contents5 = std::make_shared<table_value>();
    contents5->value[std::string("key")] = std::make_shared<string_value>("value5");
    search_and_make_table(root, nested_name.cbegin(), nested_name.cend(), contents5, true);

    auto data = std::dynamic_pointer_cast<table_value>(root);
    if(!data)
    {
        std::cerr << "no data" << std::endl;
        return 1;
    }

    auto a_arr = std::dynamic_pointer_cast<array_value>(data->value["a"]);
    if(!a_arr)
    {
        std::cerr << "no a_arr" << std::endl;
        return 1;
    }

    auto a0 = std::dynamic_pointer_cast<table_value>(a_arr->value.at(0));
    if(!a0)
    {
        std::cerr << "no a[0]" << std::endl;
        return 1;
    }
    auto key1 = std::dynamic_pointer_cast<string_value>(a0->value["key"]);
    if(!key1)
    {
        std::cerr << "no a[0].key" << std::endl;
        return 1;
    }
    auto a0b = std::dynamic_pointer_cast<array_value>(a0->value["b"]);
    if(!a0b)
    {
        std::cerr << "no a[0].b" << std::endl;
    }

    auto a0b0 = std::dynamic_pointer_cast<table_value>(a0b->value.at(0));
    if(!a0b0)
    {
        std::cerr << "no a[0].b[0]" << std::endl;
        return 1;
    }
    auto key2 = std::dynamic_pointer_cast<string_value>(a0b0->value["key"]);
    if(!key2)
    {
        std::cerr << "no a[0].b[0][key]" << std::endl;
        return 1;
    }
    auto a0b1 = std::dynamic_pointer_cast<table_value>(a0b->value.at(1));
    if(!a0b1)
    {
        std::cerr << "no a[0].b[1]" << std::endl;
        return 1;
    }
    auto key3 = std::dynamic_pointer_cast<string_value>(a0b1->value["key"]);
    if(!key3)
    {
        std::cerr << "no a[0].b[1][key]" << std::endl;
        return 1;
    }

    auto a1 = std::dynamic_pointer_cast<table_value>(a_arr->value.at(1));
    if(!a1)
    {
        std::cerr << "no a[1]" << std::endl;
        return 1;
    }
    auto key4 = std::dynamic_pointer_cast<string_value>(a1->value["key"]);
    if(!key4)
    {
        std::cerr << "no a[1].key" << std::endl;
        return 1;
    }
    auto a1b = std::dynamic_pointer_cast<array_value>(a1->value["b"]);
    if(!a1b)
    {
        std::cerr << "no a[1].b" << std::endl;
        return 1;
    }
    auto a1b0 = std::dynamic_pointer_cast<table_value>(a1b->value.at(0));
    if(!a1b0)
    {
        std::cerr << "no a[1].b[0]" << std::endl;
        return 1;
    }
    auto key5 = std::dynamic_pointer_cast<string_value>(a1b0->value["key"]);
    if(!key5)
    {
        std::cerr << "no a[1].b[0][key]" << std::endl;
        return 1;
    }

    std::cerr << "case: array_of_table and its sub array-of-table" << std::endl;
    std::cerr << "[[a]]" << std::endl;
    std::cerr << "key = " << key1->value << std::endl;
    std::cerr << "[[a.b]]" << std::endl;
    std::cerr << "key = " << key2->value << std::endl;
    std::cerr << "[[a.b]]" << std::endl;
    std::cerr << "key = " << key3->value << std::endl;
    std::cerr << "[[a]]" << std::endl;
    std::cerr << "key = " << key4->value << std::endl;
    std::cerr << "[[a.b]]" << std::endl;
    std::cerr << "key = " << key5->value << std::endl;
    }
    // }}}

    return 0;
}
