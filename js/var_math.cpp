//
// Created by 张程易 on 11/01/2018.
//

#include "var_ref.h"
#include "var.h"



#define GENERATE_MATH(OPE) \
    if(lhs.ptr->is_int_like() && rhs.ptr->is_int_like()) { \
    dst.ptr->type = Var::Type::INT; \
    dst.ptr->val_int = lhs.ptr->val_int OPE rhs.ptr->val_int; \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_int_like()) { \
    dst.ptr->type = Var::Type::DOUBLE; \
    dst.ptr->val_double = lhs.ptr->val_double OPE rhs.ptr->val_int; \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_double()) { \
    dst.ptr->type = Var::Type::DOUBLE; \
    dst.ptr->val_double = lhs.ptr->val_int OPE rhs.ptr->val_double; \
    } \
    else if(lhs.ptr->is_int_like() && rhs.ptr->is_double()){ \
    dst.ptr->type = Var::Type::DOUBLE; \
    dst.ptr->val_double = lhs.ptr->val_int OPE rhs.ptr->val_int; \
    }

#define GENERATE_COMPARE(OPE) \
    if(lhs.ptr->is_int_like() && rhs.ptr->is_int_like()){ \
        return lhs.ptr->val_int OPE rhs.ptr->val_int; \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_int_like()){ \
        return lhs.ptr->val_double OPE rhs.ptr->val_int; \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_double()){ \
        return lhs.ptr->val_double OPE rhs.ptr->val_double; \
    } \
    else if(lhs.ptr->is_int_like() && rhs.ptr->is_double()){ \
        return lhs.ptr->val_int OPE rhs.ptr->val_double; \
    } \
    else if(lhs.ptr->is_str() && rhs.ptr->is_int_like()){ \
    return \
    std::string(lhs.ptr->val_str) OPE std::to_string(rhs.ptr->val_int); \
    } \
    else if(lhs.ptr->is_str() && rhs.ptr->is_double()){ \
    return \
    std::string(lhs.ptr->val_str) OPE std::to_string(rhs.ptr->val_double); \
    } \
    else if(lhs.ptr->is_str() && rhs.ptr->is_str()){ \
    return  \
    std::string(lhs.ptr->val_str) OPE std::string(rhs.ptr->val_str); \
    } \
    else if(lhs.ptr->is_int_like() && rhs.ptr->is_str()){ \
    return \
    std::to_string(lhs.ptr->val_int) OPE std::string(rhs.ptr->val_str); \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_str()){ \
    return \
    std::to_string(lhs.ptr->val_double) OPE std::string(rhs.ptr->val_str); \
    } \
    else{ \
    throw TypeException(); \
    }

#define GENERATR_COMPARE_RHS(OPE) \
    if(lhs.ptr->is_int_like()){ \
        return lhs.ptr->val_int OPE rhs; \
    } \
    else if(lhs.ptr->is_double()){ \
        return lhs.ptr->val_double OPE rhs; \
    } \
    else if(lhs.ptr->is_str()){ \
        return \
        std::string(lhs.ptr->val_str) OPE std::to_string(rhs);\
    }\
    else{ \
    throw TypeException(); \
    }

namespace js_core {

    bool VarRef::do_lt(const VarRef &lhs, const VarRef &rhs) {
        GENERATE_COMPARE(<)
    }

    bool VarRef::do_le(const VarRef &lhs, const VarRef &rhs) {
        GENERATE_COMPARE(<=)
    }

    bool VarRef::do_lt(const VarRef &lhs, int rhs) {
        GENERATR_COMPARE_RHS(<)
    }



    void VarRef::do_add(VarRef &dst, const VarRef &lhs, const VarRef &rhs) {
        char buf[256];
        char *saved_dst_str = nullptr;
        if (dst.ptr->type == Var::Type::STR) saved_dst_str = dst.ptr->val_str;
        GENERATE_MATH(+)
        else if (lhs.ptr->is_str() && rhs.ptr->is_int_like()) {
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%d", rhs.ptr->val_int);
            char *ret = new char[strlen(lhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, lhs.ptr->val_str);
            strcpy(ret + strlen(lhs.ptr->val_str), buf);
            dst.ptr->val_str = ret;
        } else if (lhs.ptr->is_str() && rhs.ptr->is_double()) {
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%lf", rhs.ptr->val_double);
            char *ret = new char[strlen(lhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, lhs.ptr->val_str);
            strcpy(ret + strlen(lhs.ptr->val_str), buf);
            dst.ptr->val_str = ret;
        } else if (lhs.ptr->is_int_like() && rhs.ptr->is_str()) {
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%d", lhs.ptr->val_int);
            char *ret = new char[strlen(rhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, buf);
            strcpy(ret + strlen(buf), rhs.ptr->val_str);
            dst.ptr->val_str = ret;
        } else if (lhs.ptr->is_double() && rhs.ptr->is_str()) {
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%lf", lhs.ptr->val_double);
            char *ret = new char[strlen(rhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, buf);
            strcpy(ret + strlen(buf), rhs.ptr->val_str);
            dst.ptr->val_str = ret;
        } else if (lhs.ptr->is_str() && rhs.ptr->is_str()) {
            dst.ptr->type = Var::Type::STR;
            char *ret = new char[strlen(lhs.ptr->val_str) + strlen(rhs.ptr->val_str) + 1];
            strcpy(ret, lhs.ptr->val_str);
            strcpy(ret + strlen(lhs.ptr->val_str), rhs.ptr->val_str);
            dst.ptr->val_str = ret;
        } else {
            throw TypeException();
        }
        delete saved_dst_str;
    }

    void VarRef::do_sub(VarRef &dst, const VarRef &lhs, const VarRef &rhs) {
        GENERATE_MATH(-)
        else {
            throw TypeException();
        }
    }

    void VarRef::do_mul(VarRef &dst, const VarRef &lhs, const VarRef &rhs) {
        GENERATE_MATH(*)
        else {
            throw TypeException();
        }
    }

    void VarRef::do_div(VarRef &dst, const VarRef &lhs, const VarRef &rhs) {
        GENERATE_MATH(/)
        else {
            throw TypeException();
        }
    }

    void VarRef::do_add(VarRef &dst, const VarRef &lhs, int rhs) {
        char buf[256];
        char *saved_dst_str = nullptr;
        if (dst.ptr->type == Var::Type::STR) saved_dst_str = dst.ptr->val_str;
        if (lhs.ptr->is_int_like() ){
            dst.ptr->type = Var::Type::INT;
            dst.ptr->val_int = lhs.ptr->val_int + rhs;
        }
        else if (lhs.ptr->is_double() ){
            dst.ptr->type = Var::Type::DOUBLE;
            dst.ptr->val_double = lhs.ptr->val_double + rhs;
        }
        else if (lhs.ptr->is_str() ){
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%d", rhs);
            char *ret = new char[strlen(lhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, lhs.ptr->val_str);
            strcpy(ret + strlen(lhs.ptr->val_str), buf);
            dst.ptr->val_str = ret;
        }
        else {
            throw TypeException();
        }
        delete saved_dst_str;
    }

}