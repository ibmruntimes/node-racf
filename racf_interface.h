/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2017. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

#ifndef RACF_H
#define RACF_H

#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>
#include <string>

class Racf : public node::ObjectWrap {
 public:
  static void Authenticate(const v8::FunctionCallbackInfo<v8::Value>& args);

 private:
};

#endif
