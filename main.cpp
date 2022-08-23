#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>


struct StringWrapper {

    StringWrapper(const std::string& s) : strField(s) {
        std::cerr << "Create string " << reinterpret_cast<void*>(this) << std::endl;
        std::cerr << "Data " << reinterpret_cast<void*>(strField.data()) << std::endl;
        BinaryPrint(this);
    }

    StringWrapper(const char* s) : strField(s) {
        std::cerr << "Create string " << reinterpret_cast<void*>(this) << std::endl;
        std::cerr << "Data " << reinterpret_cast<void*>(strField.data()) << std::endl;
        BinaryPrint(this);
    }

    StringWrapper(const StringWrapper& s) {
        std::cerr << "Copy string from " << reinterpret_cast<const void*>(&s) << " to " << reinterpret_cast<void*>(this) << std::endl;
        strField = s.strField;
        std::cerr << "Data " << reinterpret_cast<void*>(strField.data()) << std::endl;
        BinaryPrint(&s);
        BinaryPrint(this);
    }

    StringWrapper& operator=(const StringWrapper& s) {
        std::cerr << "Copy string from " << reinterpret_cast<const void*>(&s) << " to " << reinterpret_cast<void*>(this) << std::endl;
        strField = s.strField;
        std::cerr << "Data " << reinterpret_cast<void*>(strField.data()) << std::endl;
        BinaryPrint(&s);
        BinaryPrint(this);
        return *this;
    }

    StringWrapper(StringWrapper&& s) {
        std::cerr << "Move string from " << reinterpret_cast<const void*>(&s) << " to " << reinterpret_cast<void*>(this) << std::endl;
        strField = std::move(s.strField);
        std::cerr << "Data " << reinterpret_cast<void*>(strField.data()) << std::endl;
        BinaryPrint(&s);
        BinaryPrint(this);
    }

    StringWrapper& operator=(StringWrapper&& s) {
        std::cerr << "Move string from " << reinterpret_cast<const void*>(&s) << " to " << reinterpret_cast<void*>(this) << std::endl;
        strField = std::move(s.strField);
        std::cerr << "Data " << reinterpret_cast<void*>(strField.data()) << std::endl;
        BinaryPrint(&s);
        BinaryPrint(this);
        return *this;
    }

    ~StringWrapper() {
        std::cerr << "Delete string " << reinterpret_cast<void*>(this) << std::endl;
        std::cerr << "Data " << reinterpret_cast<void*>(strField.data()) << std::endl;
        BinaryPrint(this);
    }

    void BinaryPrint(const StringWrapper* what) {

        const char* c = reinterpret_cast<const char*>(what);

        for(auto i = 0u; i < sizeof(StringWrapper); i++) {
            std::cerr << std::hex << (int)*c++ << " ";
        }
        std::cerr << std::dec << std::endl;

    };
    std::string strField;
};

struct TestStruct {
    //explicit TestStruct(const std::string& str) : strField(str) {}
    StringWrapper strField;
};

boost::asio::awaitable<bool> TestFunction(TestStruct param) {
    std::cout << param.strField.strField << std::endl;
    co_return true;
}

boost::asio::awaitable<void> TestCoroutine() {
    // This does not work
    co_await TestFunction(TestStruct{"    1    2"});
}

int main()
{
    boost::asio::thread_pool ctx(1);

    boost::asio::co_spawn(
        ctx,
        []() { return TestCoroutine(); },
        boost::asio::use_future).wait();

    ctx.join();

    return 0;
}
