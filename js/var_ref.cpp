//
// Created by 张程易 on 10/01/2018.
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

#define GENETRATE_COMPARE(OPE) \
    dst.ptr->type = Var::Type::BOOLEAN; \
    if(lhs.ptr->is_int_like() && rhs.ptr->is_int_like()){ \
    dst.ptr->val_int = lhs.ptr->val_int OPE rhs.ptr->val_int; \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_int_like()){ \
    dst.ptr->val_int = lhs.ptr->val_double OPE rhs.ptr->val_int; \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_double()){ \
    dst.ptr->val_int = lhs.ptr->val_double OPE rhs.ptr->val_double; \
    } \
    else if(lhs.ptr->is_int_like() && rhs.ptr->is_double()){ \
    dst.ptr->val_int = lhs.ptr->val_int OPE rhs.ptr->val_double; \
    } \
    else if(lhs.ptr->is_str() && rhs.ptr->is_int()){ \
    dst.ptr->val_int = \
    std::string(lhs.ptr->val_str) OPE std::to_string(rhs.ptr->val_int); \
    } \
    else if(lhs.ptr->is_str() && rhs.ptr->is_double()){ \
    dst.ptr->val_int = \
    std::string(lhs.ptr->val_str) OPE std::to_string(rhs.ptr->val_double); \
    } \
    else if(lhs.ptr->is_str() && rhs.ptr->is_str()){ \
    dst.ptr->val_int = \
    std::string(lhs.ptr->val_str) OPE std::string(rhs.ptr->val_str); \
    } \
    else if(lhs.ptr->is_int() && rhs.ptr->is_str()){ \
    dst.ptr->val_int = \
    std::to_string(lhs.ptr->val_int) OPE std::string(rhs.ptr->val_str); \
    } \
    else if(lhs.ptr->is_double() && rhs.ptr->is_str()){ \
    dst.ptr->val_int = \
    std::to_string(lhs.ptr->val_double) OPE std::string(rhs.ptr->val_str); \
    } \
    else{ \
    throw TypeException(); \
    }


namespace js{



    VarRef::VarRef(const VarRef &val) {
        if(val.ptr->is_primitive()) {
            ptr = std::shared_ptr<Var>(val.ptr->clone());
        }
        else{
            ptr = val.ptr;
        }
        printf("copy\n");
    }

    VarRef::VarRef(VarRef &&val) noexcept {
        if(val.ptr->is_primitive()) {
            ptr = std::shared_ptr<Var>(val.ptr->clone());
        }
        else{
            ptr = std::move(val.ptr);
        }
        printf("move\n");
    }


    VarRef::VarRef(bool val_boolean) {
        ptr = std::make_shared<Var>(VAR_NULL());
        ptr->val_int = val_boolean;
        ptr->type = Var::Type::BOOLEAN;
    }


    VarRef &VarRef::operator=(const VarRef &val) {
        if(val.ptr->is_primitive()) {
            ptr = std::shared_ptr<Var>(val.ptr->clone());
        }
        else{
            ptr = val.ptr;
        }
        return *this;
    }

    VarRef &VarRef::operator=(int val_int) {
        if(ptr->is_int()) {
            ptr->val_int = val_int;
        }
        else{
            ptr = std::make_shared<Var>(val_int);;
        }
        return *this;
    }

    VarRef &VarRef::operator=(double val_double) {
        if(ptr->is_double()) {
            ptr->val_double = val_double;
        }
        else{
            ptr = std::make_shared<Var>(val_double);;
        }
        return *this;
    }

    void VarRef::do_lt(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        GENETRATE_COMPARE(<)
    }

    void VarRef::do_gt(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        GENETRATE_COMPARE(>)
    }

    void VarRef::do_le(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        GENETRATE_COMPARE(<=)
    }

    void VarRef::do_ge(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        GENETRATE_COMPARE(>=)
    }

    void VarRef::do_add(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        char buf[256];
        char * saved_dst_str = nullptr;
        if( dst.ptr->type == Var::Type::STR) saved_dst_str = dst.ptr->val_str;
        GENERATE_MATH(+)
        else if(lhs.ptr->is_str() && rhs.ptr->is_int_like()){
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%d", rhs.ptr->val_int);
            char * ret = new char[strlen(lhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, lhs.ptr->val_str);
            strcpy(ret + strlen(lhs.ptr->val_str), buf);
            dst.ptr->val_str = ret;
        }
        else if(lhs.ptr->is_str() && rhs.ptr->is_double()){
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%lf", rhs.ptr->val_double);
            char * ret = new char[strlen(lhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, lhs.ptr->val_str);
            strcpy(ret + strlen(lhs.ptr->val_str), buf);
            dst.ptr->val_str = ret;
        }
        else if(lhs.ptr->is_int_like() && rhs.ptr->is_str()){
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%d", lhs.ptr->val_int);
            char * ret = new char[strlen(rhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, buf);
            strcpy(ret + strlen(buf), rhs.ptr->val_str);
            dst.ptr->val_str = ret;
        }
        else if(lhs.ptr->is_double() && rhs.ptr->is_str()){
            dst.ptr->type = Var::Type::STR;
            sprintf(buf, "%lf", lhs.ptr->val_double);
            char * ret = new char[strlen(rhs.ptr->val_str) + strlen(buf) + 1];
            strcpy(ret, buf);
            strcpy(ret + strlen(buf), rhs.ptr->val_str);
            dst.ptr->val_str = ret;
        }
        else if(lhs.ptr->is_str() && rhs.ptr->is_str()){
            dst.ptr->type = Var::Type::STR;
            char * ret = new char[strlen(lhs.ptr->val_str) + strlen(rhs.ptr->val_str) + 1];
            strcpy(ret, lhs.ptr->val_str);
            strcpy(ret + strlen(lhs.ptr->val_str), rhs.ptr->val_str);
            dst.ptr->val_str = ret;
        }
        else{
            throw TypeException();
        }
        delete saved_dst_str;
    }

    void VarRef::do_sub(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        GENERATE_MATH(-)
        else{
            throw TypeException();
        }
    }

    void VarRef::do_mul(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        GENERATE_MATH(*)
        else{
            throw TypeException();
        }
    }

    void VarRef::do_div(VarRef & dst, const VarRef & lhs, const VarRef & rhs) {
        GENERATE_MATH(/)
        else{
            throw TypeException();
        }
    }

    VarRef &VarRef::operator+=(const VarRef & val){
        do_add(*this, *this, val);
        return *this;
    }

    VarRef &VarRef::operator-=(const VarRef & val){
        do_sub(*this, *this, val);
        return *this;
    }

    VarRef &VarRef::operator*=(const VarRef & val){
        do_mul(*this, *this, val);
        return *this;
    }

    VarRef &VarRef::operator/=(const VarRef & val){
        do_div(*this, *this, val);
        return *this;
    }

    VarRef VarRef::operator+(const VarRef & val){
        VarRef result = VAR_NULL();
        do_add(result, *this, val);
        return result;
    }

    VarRef VarRef::operator-(const VarRef & val){
        VarRef result = VAR_NULL();
        do_sub(result, *this, val);
        return result;
    }

    VarRef VarRef::operator*(const VarRef & val){
        VarRef result = VAR_NULL();
        do_mul(result, *this, val);
        return result;
    }

    VarRef VarRef::operator/(const VarRef & val){
        VarRef result = VAR_NULL();
        do_div(result, *this, val);
        return result;
    }

    VarRef VarRef::operator<(const VarRef & val){
        VarRef result = VAR_NULL();
        do_lt(result, *this, val);
        return result;
    }

    VarRef VarRef::operator<=(const VarRef & val){
        VarRef result = VAR_NULL();
        do_le(result, *this, val);
        return result;
    }

    VarRef VarRef::operator>(const VarRef & val){
        VarRef result = VAR_NULL();
        do_gt(result, *this, val);
        return result;
    }

    VarRef VarRef::operator>=(const VarRef & val){
        VarRef result = VAR_NULL();
        do_ge(result, *this, val);
        return result;
    }

    VarRef::operator bool() const {
        if( ptr->is_int_like() ) return ptr->val_int != 0;
        else if( ptr->is_double() ) return fabs(ptr->val_double) > 1e-6;
        else if( ptr->is_str() ) return ptr->val_str[0] != 0;
        else return true;
    }

    VarRef & VarRef::operator++(){
        this->operator+=(1);
        return * this;
    }

    VarRef & VarRef::operator--(){
        this->operator-=(1);
        return * this;
    }

    VarRef::VarRef(const std::initializer_list<KeyValuePair> l, ...) {
        ptr = std::make_shared<Var>(VAR_OBJECT());
        for(auto i = l.begin(); i != l.end(); i++){
            printf("debug\n");
            ptr->val_object->set(i->a, i->b);
        }
    }

    KeyValuePair::KeyValuePair(const char *a, const VarRef &b) : a(a), b(b) {}
}