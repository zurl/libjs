# libjs
Write code like javascript in cpp

Compatible with C++ 11 ( C++14/17 is ok, but not necessary)

### demo

```C++

var a = 1;

a = "123";

a = array(1, 2, 3, 4, 5);


var b = "abc";

for(var i = 1; i < 10; ++i){
    console.log(i + "result");
}

var foo = [](param a, param b){
    return a + b;
};

var obj = {
    { "hello", [](param a){ return a; } }
};

var arr = array(1, 2, 3, 4, 5);

```
