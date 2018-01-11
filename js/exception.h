//
// Created by 张程易 on 10/01/2018.
//

#ifndef LIBJS_EXCEPTION_H
#define LIBJS_EXCEPTION_H

#include "common.h"

namespace js_core{

    class Exception{
    public:
        virtual std::string toString() = 0;
    };

    class TypeException: public Exception{
    public:
        std::string toString() override {
            return "TypeException";
        }
    };

}

#endif //LIBJS_EXCEPTION_H
