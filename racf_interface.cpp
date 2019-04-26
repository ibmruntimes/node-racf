/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2017. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

#include "racf_interface.h"
#include <node_buffer.h>
#include <unistd.h>
#include <dynit.h>
#include <sstream>
#include <numeric>

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::Exception;
using v8::HandleScope;
using v8::Handle;
using v8::Array;
using v8::Integer;
using v8::Boolean;
using v8::MaybeLocal;

void Racf::Authenticate(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  if (args.Length() != 2) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }
  if (!args[1]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  std::string username (*v8::String::Utf8Value(args[0]->ToString()));
  std::string password (*v8::String::Utf8Value(args[1]->ToString()));
  transform(username.begin(), username.end(), username.begin(), [](char c) -> char {
    __a2e_l(&c, 1);
    return c;
  });
  transform(password.begin(), password.end(), password.begin(), [](char c) -> char {
    __a2e_l(&c, 1);
    return c;
  });

  int value = __passwd(username.c_str(), password.c_str(), 0);

  if (value != 0) {
	perror("RACF");	
  }
  args.GetReturnValue().Set(Boolean::New(isolate, value == 0));
}
