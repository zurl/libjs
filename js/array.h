//
// Created by 张程易 on 11/01/2018.
//

#ifndef LIBJS_ARRAY_H
#define LIBJS_ARRAY_H

#include <cstdio>
#include "common.h"
#include "object.h"

namespace js_core{

    class Array: public Object{

        const int DENSE_MAX_INTERVAL = 10;

        bool is_sparse = false;
        int max_size = 0;

        std::vector<VarRef> array;

        void convert_to_sparse(){
            for(int i = 0; i < array.size(); i++){
                set_kv(std::to_string(i), array[i]);
            }
            array.clear();
            is_sparse = true;
        }

        VarRef &get_array(int index) {
            if( index > array.size() + DENSE_MAX_INTERVAL){
                convert_to_sparse();
                return get_kv(std::to_string(index));
            }
            while( index >= array.size() ){
                array.emplace_back(VAR_NULL());
            }
            return array[index];
        }

        void set_array(int index, const VarRef &value){
            if( index > array.size() + DENSE_MAX_INTERVAL){
                convert_to_sparse();
                set_kv(std::to_string(index), value);
                return;
            }
            while( index >= array.size() ){
                array.emplace_back(VAR_NULL());
            }
            array[index] = value;
            return;
        }

    public:
        Array(): array() {
            array.reserve(10);
        }

        void push_back(VarRef && value){
            if( is_sparse ){
                set_kv(std::to_string(max_size++), std::move(value));
            }
            else{
                array.emplace_back(std::move(value));
            }
        }

        VarRef &get(const std::string &key) override {
            for(int i = 0; i < key.length(); i++){
                if(! (key[i] >= '0' && key[i] <= '9') ){
                    return get_kv(key);
                }
            }
            int index;
            sscanf(key.c_str(), "%d", &index);
            max_size = std::max(index, max_size);
            if( is_sparse ) return get_kv(key);
            return get_array(index);
        }

        void set(const std::string &key, const VarRef &value) override {
            for(int i = 0; i < key.length(); i++){
                if(! (key[i] >= '0' && key[i] <= '9') ){
                    set_kv(key, value);
                }
            }
            int index;
            sscanf(key.c_str(), "%d", &index);
            max_size = std::max(index, max_size);
            if( is_sparse ) set_kv(key, value);
            return set_array(index, value);
        }

        VarRef &get(int index) override {
            max_size = std::max(index, max_size);
            if( is_sparse ) return get_kv(std::to_string(index));
            return get_array(index);
        }

        void set(int index, const VarRef &value) override {
            max_size = std::max(index, max_size);
            if( is_sparse ) set_kv(std::to_string(index), value);
            return set_array(index, value);
        }

    };

}

#endif //LIBJS_ARRAY_H
