// #include<boost/test/unit_test.hpp>
#define BOOST_TEST_MODULE MyTest
#include<boost/test/included/unit_test.hpp>

#include <string>

std::string who()
{
    return "Bill";
}
BOOST_AUTO_TEST_CASE(my_boost_test)
{
    std::string expected_value = "Bill";

    BOOST_CHECK(expected_value == who());
}
