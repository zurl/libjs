# libjs
Write code like javascript in cpp


### demo

```C++

var a = 1;
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