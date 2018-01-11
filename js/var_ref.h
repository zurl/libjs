//
// Created by 张程易 on 10/01/2018.
//

#ifndef LIBJS_VAR_REF_H
#define LIBJS_VAR_REF_H

#include "common.h"

namespace js_core {
    class KeyValuePair;
    class Var;
    class ArrayVarRef;

    class VarRef {
    public:
        std::shared_ptr<Var> ptr;

        VarRef(VAR_ARRAY _){
            ptr = std::make_shared<Var>(_);
        }

        VarRef(ArrayVarRef && val) noexcept;

        VarRef(const VarRef &val);

        VarRef(VarRef &&val) noexcept;

        VarRef(bool val_boolean);

        template<typename T>
        VarRef(const T &val) {
            ptr = std::make_shared<Var>(val);
        }

        VarRef(const std::initializer_list<KeyValuePair> l, ...);

        VarRef &operator=(const VarRef & val);
        VarRef &operator=(VarRef && val);
        VarRef &operator=(int val_int);
        VarRef &operator=(double val_double);
        VarRef &operator=(bool val_boolean);

        VarRef & operator--();
        VarRef & operator++();
        VarRef &operator+=(const VarRef & val);
        VarRef &operator-=(const VarRef & val);
        VarRef &operator*=(const VarRef & val);
        VarRef &operator/=(const VarRef & val);
        VarRef operator+(const VarRef & val) const;
        VarRef operator-(const VarRef & val) const;
        VarRef operator*(const VarRef & val) const;
        VarRef operator/(const VarRef & val) const;
        bool operator<(const VarRef & val) const;
        bool operator>(const VarRef & val) const;
        bool operator>=(const VarRef & val) const;
        bool operator<=(const VarRef & val) const;

        VarRef operator+(int val) const;
        VarRef &operator+=(int val);
        bool operator<(int val) const;

        explicit operator bool() const;


        static void do_add(VarRef & dst, const VarRef & lhs, const VarRef & rhs);
        static void do_sub(VarRef & dst, const VarRef & lhs, const VarRef & rhs);
        static void do_mul(VarRef & dst, const VarRef & lhs, const VarRef & rhs);
        static void do_div(VarRef & dst, const VarRef & lhs, const VarRef & rhs);

        static void do_add(VarRef & dst, const VarRef & lhs, int rhs);
//        static void do_mul(VarRef & dst, const VarRef & lhs, int rhs);
//        static void do_sub(VarRef & dst, const VarRef & lhs, int rhs);
//        static void do_div(VarRef & dst, const VarRef & lhs, int rhs);
//        static void do_add(VarRef & dst, int lhs, const VarRef & rhs);
//        static void do_sub(VarRef & dst, int lhs, const VarRef & rhs);
//        static void do_div(VarRef & dst, int lhs, const VarRef & rhs);

        static bool do_lt(const VarRef & lhs, const VarRef & rhs);
        static bool do_le(const VarRef & lhs, const VarRef & rhs);
        static bool do_lt(const VarRef & lhs, int rhs);
        // static bool do_lt(int lhs, const VarRef & rhs);


        template<typename ...ArgTypes>
        VarRef operator()(ArgTypes &&... args) {
            return (*ptr)(std::forward(args)...);
        }

        template<typename T>
        VarRef &operator[](const T &key) {
            return (*ptr)[key];
        }

        friend std::ostream &operator<<(std::ostream &os, const VarRef &var1);
    };


    class KeyValuePair {
    public:
        const char *a;
        VarRef b;
        KeyValuePair(const char * a, const VarRef & b);
    };


}
#endif //LIBJS_VAR_REF_H
