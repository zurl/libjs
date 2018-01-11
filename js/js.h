//
// Created by 张程易 on 10/01/2018.
//

#ifndef LIBJS_JS_H
#define LIBJS_JS_H

#include "var_ref.h"
#include "var.h"
#include "function.h"
#include "object.h"

namespace js {
    using var = js_core::VarRef;
    using param = const var &;
    using array = js_core::ArrayVarRef;
}


#endif //LIBJS_JS_H
