//
// Created by 张程易 on 10/01/2018.
//

#include "var_ref.h"
#include "var.h"


namespace js_core{


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
            ptr = std::shared_ptr<Var>(val.ptr);
        }
        else{
            ptr = val.ptr;
        }
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

//    VarRef &VarRef::operator=(VarRef &&val) {
//        // fuck here
//        printf("###$$$###\n");
////        if(val.ptr->is_primitive()) {
////            // TODO:: problem here
////            ptr = std::shared_ptr<Var>(val.ptr->clone());
////        }
////        else{
////            ptr = val.ptr;
////        }
////        return *this;
//    }

    VarRef &VarRef::operator=(int val_int) {
        if(ptr->is_primitive()) {
            if(ptr->is_str()) delete ptr->val_str;
            ptr->type = Var::Type::INT;
            ptr->val_int = val_int;
        }
        else{
            ptr = std::make_shared<Var>(val_int);;
        }
        return *this;
    }

    VarRef &VarRef::operator=(bool val_boolean) {
        if(ptr->is_primitive()) {
            if(ptr->is_str()) delete ptr->val_str;
            ptr->type = Var::Type::BOOLEAN;
            ptr->val_int = val_boolean;
        }
        else{
            ptr = std::make_shared<Var>(val_boolean);
        }
        return *this;
    }

    VarRef &VarRef::operator=(double val_double) {
        if(ptr->is_primitive()) {
            if(ptr->is_str()) delete ptr->val_str;
            ptr->type = Var::Type::DOUBLE;
            ptr->val_double = val_double;
        }
        else{
            ptr = std::make_shared<Var>(val_double);;
        }
        return *this;
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

    VarRef VarRef::operator+(const VarRef & val) const{
        VarRef result = VAR_NULL();
        do_add(result, *this, val);
        return result;
    }

    VarRef VarRef::operator-(const VarRef & val) const{
        VarRef result = VAR_NULL();
        do_sub(result, *this, val);
        return result;
    }

    VarRef VarRef::operator*(const VarRef & val) const{
        VarRef result = VAR_NULL();
        do_mul(result, *this, val);
        return result;
    }

    VarRef VarRef::operator/(const VarRef & val) const{
        VarRef result = VAR_NULL();
        do_div(result, *this, val);
        return result;
    }

    bool VarRef::operator<(const VarRef & val) const{
        return do_lt(*this, val);
    }

    bool VarRef::operator<=(const VarRef & val) const{
        return do_le(*this, val);
    }

    bool VarRef::operator>(const VarRef & val) const{
        return !do_le(*this, val);
    }

    bool VarRef::operator>=(const VarRef & val) const{
        return !do_lt(*this, val);
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

    VarRef::VarRef(ArrayVarRef &&val) noexcept {
        ptr = val.ptr;
    }

    KeyValuePair::KeyValuePair(const char *a, const VarRef &b) : a(a), b(b) {}


    VarRef VarRef::operator+(int val) const{
        VarRef result = VAR_NULL();
        do_add(result, *this, val);
        return result;
    }

    VarRef &VarRef::operator+=(int val){
        do_add(*this, *this, val);
        return *this;
    }

    bool VarRef::operator<(int val) const{
        return do_lt(*this, val);
    }

}