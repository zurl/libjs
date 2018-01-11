//
// Created by 张程易 on 10/01/2018.
//

#ifndef LIBJS_VAR_H
#define LIBJS_VAR_H

#include "exception.h"
#include "function.h"
#include "object.h"
#include "array.h"


namespace js_core {
    class Var {

        friend class VarRef;

        enum class Type {
            INT, DOUBLE, STR, OBJECT, FUNC, NULLTYPE, BOOLEAN, ARRAY
        };

        Type type;

        union {
            int val_int;
            double val_double;
            char *val_str;
            Object *val_object;
        };

    public:

        Var(VAR_NULL _): type(Type::NULLTYPE){}
        Var(VAR_ARRAY _): type(Type::ARRAY), val_object(new Array()){}
        Var(VAR_OBJECT _): type(Type::OBJECT), val_object(new Object()){}

        Var(const char * str) : type(Type::STR) {
            val_str = new char[strlen(str) + 1];
            strcpy(val_str, str);
        }

        Var(int val_int) : val_int(val_int), type(Type::INT) {
            printf("hello %d %d %p\n", type, val_int, this);
        }

        Var(double val_double) : val_double(val_double), type(Type::DOUBLE) {}

        Var(const std::function<VarRef()> &mFunction) : type(Type::FUNC), val_object(new Function<>(mFunction)) {}

        Var(const std::function<VarRef(VarRef)> &mFunction) : type(Type::FUNC),
                                                              val_object(new Function<VarRef>(mFunction)) {}

        Var(const std::function<VarRef(VarRef, VarRef)> &mFunction) : type(Type::FUNC), val_object(
                new Function<VarRef, VarRef>(mFunction)) {}

        Var(const std::function<VarRef(VarRef, VarRef, VarRef)> &mFunction) : type(Type::FUNC), val_object(
                new Function<VarRef, VarRef, VarRef>(mFunction)) {}

        Var(const std::function<VarRef(VarRef, VarRef, VarRef, VarRef)> &mFunction) : type(Type::FUNC), val_object(
                new Function<VarRef, VarRef, VarRef, VarRef>(mFunction)) {}

        Var(const std::function<VarRef(VarRef, VarRef, VarRef, VarRef, VarRef)> &mFunction) : type(Type::FUNC),
                                                                                              val_object(
                                                                                                      new Function<VarRef, VarRef, VarRef, VarRef, VarRef>(
                                                                                                              mFunction)) {}
        Var(const std::initializer_list<KeyValuePair> l)
        : type(Type::OBJECT), val_object(new Object())
        {
            std::cout << "constructed with a " << l.size() << "-element list\n";
        }

        Var(const Var & var) = delete;
        Var &operator=(const Var & var) = delete;
        Var(Var && var) = delete;
        Var &operator=(Var && var) = delete;


        void push(VarRef && ref){
            if(!is_array()) throw TypeException();
            dynamic_cast<Array*>(val_object)->push_back(std::forward<VarRef>(ref));
        }



        VarRef & operator[](const std::string & key){
            if(!is_object()) throw TypeException();
            return val_object->get(key);
        }

        VarRef & operator[](int index){
            if(!is_object()) throw TypeException();
            return val_object->get(index);
        }

        template<typename ...ArgTypes>
        VarRef operator()(ArgTypes &&... args) {
            return dynamic_cast<BaseFunction *>(val_object)
                    ->operator()(std::forward(args)...);
        }


        friend std::ostream &operator<<(std::ostream &os, const Var &var1) {
            switch (var1.type) {
                case Type::INT:
                    os << var1.val_int;
                    break;
                case Type::DOUBLE:
                    os << var1.val_double;
                    break;
                case Type::STR:
                    os << var1.val_str;
                    break;
                case Type::BOOLEAN:
                    os << (var1.val_int ? "true" : "false");
                    break;
                case Type::FUNC:
                    os << "[FUNCTION]";
                    break;
                default:
                    break;
            }
            return os;
        }


        ~Var() {
            printf("ohno %d %d %p\n", type, val_int, this);
            if( type == Type::STR ) delete val_str;
            if( type == Type::OBJECT
                || type == Type::FUNC
                || type == Type::ARRAY ) delete val_object;
        }

        inline bool is_primitive(){
            return type == Type::BOOLEAN || type == Type::INT || type == Type::STR || type == Type::DOUBLE;
        }

        inline bool is_int(){
            return type == Type::INT;
        }

        inline bool is_int_like(){
            return type == Type::INT || type == Type::BOOLEAN;
        }

        inline bool is_double(){
            return type == Type::DOUBLE;
        }

        inline bool is_str(){
            return type == Type::STR;
        }

        inline bool is_boolean(){
            return type == Type::BOOLEAN;
        }

        inline bool is_object(){
            return type == Type::OBJECT || type == Type::FUNC || type == Type::ARRAY;
        }

        inline bool is_naive_object(){
            return type == Type::OBJECT;
        }

        inline bool is_array(){
            return type == Type::ARRAY;
        }

        inline bool is_function(){
            return type == Type::FUNC;
        }

        Var * clone(){
            if( is_str() ){
                return new Var(val_str);
            }
            assert(is_primitive());
            Var *ret = new Var(VAR_NULL());
            memcpy(ret, this, sizeof(Var));
            return ret;
        }

    };


    void create_array_iter(const VarRef &arr);

    template <typename ...T>
    void create_array_iter(const VarRef &arr, VarRef && ref, T&&... args){
        printf("hehe_in\n");
        arr.ptr->push(std::forward<VarRef>(ref));
        create_array_iter(arr, std::forward<T>(args)...);
        printf("hehe_out\n");
    }

    class ArrayVarRef: public VarRef{
    public:
        template <typename ...T>
        explicit ArrayVarRef(T&&... args): VarRef(VAR_ARRAY()){
            create_array_iter(*this, std::forward<T>(args)...);

        }
    };


}

#endif //LIBJS_VAR_H
