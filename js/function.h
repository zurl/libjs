//
// Created by 张程易 on 10/01/2018.
//

#ifndef LIBJS_FUNCTION_H
#define LIBJS_FUNCTION_H

#include "var_ref.h"
#include "object.h"

namespace js_core {
    class BaseFunction: public Object {
    public:

        template<typename ...ArgTypes>
        VarRef operator()(ArgTypes &&... args) {
            std::vector<VarRef> argvec;
            argvec.reserve(7);
            return pack(argvec, std::forward<ArgTypes>(args)...);
        }


        VarRef pack(std::vector<VarRef> &argvec) {
            return invoke(argvec);
        }

        template<class ...FuncArgs>
        VarRef pack(std::vector<VarRef> &argvec, VarRef &&var, FuncArgs &&... args) {
            argvec.emplace_back(std::move(var));
            return pack(argvec, std::forward<FuncArgs>(args)...);
        }

        virtual VarRef invoke(const std::vector<VarRef> &arguments) = 0;

        virtual ~BaseFunction() {

        }

    };

    template<class ..._ArgTypes>
    class Function : public BaseFunction {
        std::function<VarRef(_ArgTypes...)> func;
    public:

        template<class T>
        Function(T f) : func(f) {};

        template<int now, class ...FuncArgs>
        typename std::enable_if<
                now == sizeof...(_ArgTypes), VarRef>::type
        unpack(const std::vector<VarRef> &argvec, FuncArgs &&... args) {
            return func(std::forward<FuncArgs>(args)...);
        }

        template<int now, class ...FuncArgs>
        typename std::enable_if<
                now != sizeof...(_ArgTypes), VarRef>::type
        unpack(const std::vector<VarRef> &argvec, FuncArgs &&... args) {
            if (now < argvec.size())
                return unpack<now + 1>(
                        argvec, std::forward<FuncArgs>(args)..., argvec[now]);
            else return unpack<now + 1>(argvec, std::forward<FuncArgs>(args)..., VarRef(1));
        }

        virtual VarRef invoke(const std::vector<VarRef> &arguments) override {
            return unpack<0>(arguments);
        }
    };
}
#endif //LIBJS_FUNCTION_H
