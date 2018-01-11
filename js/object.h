//
// Created by 张程易 on 10/01/2018.
//

#ifndef LIBJS_OBJECT_H
#define LIBJS_OBJECT_H

#include "var_ref.h"
namespace js {

    class Object {
    protected:
        std::unordered_map<std::string, VarRef> data;

        VarRef & get_kv(const std::string & key){
            auto iter = data.find(key);
            if(iter != data.end()) return iter->second;
            else {
                data.emplace(std::piecewise_construct,
                             std::forward_as_tuple(key),
                             std::forward_as_tuple(VAR_NULL()));
                return data.find(key)->second;
            }
        }

        void set_kv(const std::string & key, const VarRef & value){
            data.emplace(key, value);
        }

    public:
        virtual VarRef & get(const std::string & key){
            return get_kv(key);
        }

        virtual void set(const std::string & key, const VarRef & value){
            set_kv(key, value);
        }

        virtual VarRef & get(int index){
            return get_kv(std::to_string(index));
        }

        virtual void set(int index, const VarRef & value){
            set_kv(std::to_string(index), value);
        }

        virtual ~Object() {

        }

    };

}
#endif //LIBJS_OBJECT_H
