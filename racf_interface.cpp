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
#include <sys/types.h>
#include <grp.h>

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
        String::NewFromUtf8(isolate, "First argument must be a string")));
    return;
  }
  if (!args[1]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Second argument must be a string password")));
    return;
  }

  std::string username (*v8::String::Utf8Value(args[0]->ToString()));
  std::string password (*v8::String::Utf8Value(args[1]->ToString()));

  // Convert strings from Ascii to Ebcdic for z/OS
  transform(username.begin(), username.end(), username.begin(), [](char c) -> char {
    __a2e_l(&c, 1);
    return c;
  });
  transform(password.begin(), password.end(), password.begin(), [](char c) -> char {
    __a2e_l(&c, 1);
    return c;
  });


  // Authenticate via __passwd interface
  int value = __passwd(username.c_str(), password.c_str(), 0);

  if (value != 0) {
    char buffer[256];
	strerror_r(errno, buffer, 256);	
	__e2a_s(buffer);
	return Nan::ThrowError(buffer);
  }
  args.GetReturnValue().Set(Boolean::New(isolate, true));
}

void Racf::isUserInGroup(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  if (args.Length() != 2) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "First argument must be a string")));
    return;
  }
  if (!args[1]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Second argument must be a string password")));
    return;
  }

  std::string user (*v8::String::Utf8Value(args[0]->ToString()));
  std::string group (*v8::String::Utf8Value(args[1]->ToString()));

  // Convert strings from Ascii to Ebcdic for z/OS
  transform(user.begin(), user.end(), user.begin(), [](char c) -> char {
    __a2e_l(&c, 1);
    return toupper(c);
  });
  transform(group.begin(), group.end(), group.begin(), [](char c) -> char {
    __a2e_l(&c, 1);
    return toupper(c);
  });


  struct group* grp;
  char   **curr;
  grp = getgrnam(group.c_str());

  if (grp == 0) {
    char buffer[256];
	strerror_r(errno, buffer, 256);	
	__e2a_s(buffer);
	return Nan::ThrowError(buffer);
  }

  for (curr=grp->gr_mem; (*curr) != NULL; curr++) {
//#pragma convert("IBM-1047")
  //  printf("%s - %s\n", user.c_str(), *curr);
//#pragma convert(pop)
    if (strcmp(user.c_str(), *curr) == 0)  {
        args.GetReturnValue().Set(Boolean::New(isolate, true));
        return;
    }
  }
  args.GetReturnValue().Set(Boolean::New(isolate, false));
}
