#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>


struct TestStruct {
    std::string strField;
};

struct TestStructWithCtor {
    explicit TestStructWithCtor(const std::string& str) : strField(str) {}
    std::string strField;
};


boost::asio::awaitable<bool> TestFunction(TestStruct param) {
    std::cout << param.strField << std::endl;
    co_return true;
}

boost::asio::awaitable<bool> TestFunction2(TestStructWithCtor param) {
    std::cout << param.strField << std::endl;
    co_return true;
}

boost::asio::awaitable<void> TestCoroutine() {
    auto testStruct = TestStruct{"some string"};

    // This works
    // co_await TestFunction(testStruct);
    // co_await TestFunction2(TestStructWithCtor("some string"));
    // co_await TestFunction2(TestStructWithCtor{.strField = "some string"});

    // This causes mem issue
    co_await TestFunction(TestStruct{"some string"});
}

int main()
{
    boost::asio::io_context io_context;

    boost::asio::co_spawn(
        io_context,
        []() { return TestCoroutine(); },
        boost::asio::detached);

    io_context.run();
    return 0;
}
