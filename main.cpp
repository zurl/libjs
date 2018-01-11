#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>

#include "js/js.h"
using namespace js;


namespace js_core {
    std::ostream &operator<<(std::ostream &os, const VarRef &var1) {
        os << *(var1.ptr);
        return os;
    }

}

class Console{
public:
    void log(const var & var1){
        std::cout << var1;
    }

} console;

int main() {

    var a = 1;
    a = 2;
    a = 3;

    var uu = false;
    std::cout << uu;

    for(var i = 1; i < 10; ++i){
        console.log(i + "result");
    }

    var * _a = new var(1);
    delete _a;

    var d = array(1, 2, 3, 4, 5);


    std::cout<<"======1.5 phase===" << std::endl;
    var b = {{"1", 1.1},
             {"2", 1},
             {"3", 1.7}};

    std::cout<<"======1 phase===" << std::endl;

    std::cout << b[3];

    std::cout<<"======2 phase===" << std::endl;

    var e = {};

    std::cout<<"======1 phase===" << std::endl;

    std::function<void(void)> aa = []() {

    };

    var k2 = [](param a) -> var {
        std::cout << a << std::endl;
        return a;
    };


    var k = [](param a, param b) -> var {
        std::cout << a << std::endl;
        return a;
    };

    k["fuck"] = 1;
    k();

    std::cout<< k["fuck"] <<std::endl;

    printf("=====\n");
    //uu(VarRef(123), VarRef(123), VarRef(123));
    printf("====invoke end===\n");
    std::cout << a << std::endl;

    std::cout << "Hello, World!" << std::endl;



    var foo = [](param a, param b){
        return a + b;
    };

    var obj = {
            { "hello", [](param a){
                printf("ok!############");
                return a; } }
    };

    obj["hello"](1);

    return 0;
}