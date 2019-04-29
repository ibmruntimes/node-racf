/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2017. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

#include <node.h>
#include "racf_interface.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void InitAll(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(exports, "authenticate", Racf::Authenticate);
  NODE_SET_METHOD(exports, "isUserInGroup", Racf::isUserInGroup);
}

NODE_MODULE(racf, InitAll)
