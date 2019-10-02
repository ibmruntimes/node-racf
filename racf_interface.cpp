/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2019. All Rights Reserved.
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
#include <pwd.h>

Napi::Boolean Racf::authenticate(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    Napi::Error::New(env, "Wrong number of arguments passed to authenticate")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "First argument must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }
  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Second argument must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  std::string username (static_cast<std::string>(info[0].As<Napi::String>()));
  std::string password (static_cast<std::string>(info[1].As<Napi::String>()));

  // Authenticate via __passwd interface
  int value = __passwd(username.c_str(), password.c_str(), 0);

  if (value != 0) {
    if (errno == EACCES)  {
       return Napi::Boolean::New(env, false);
    }
    char buffer[1024];
	strerror_r(errno, buffer, 1024);	
    Napi::TypeError::New(env, buffer)
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }
  return Napi::Boolean::New(env, true);
}

Napi::Boolean Racf::isUserInGroup(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    Napi::Error::New(env, "Wrong number of arguments passed to authenticate")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "First argument must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }
  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Second argument must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  std::string user (static_cast<std::string>(info[0].As<Napi::String>()));
  std::string group (static_cast<std::string>(info[1].As<Napi::String>()));

  struct group* grp;
  char   **curr;
  grp = getgrnam(group.c_str());

  if (grp == 0) {
    return Napi::Boolean::New(env, false);
  }

  for (curr=grp->gr_mem; (*curr) != NULL; curr++) {
    if (strcmp(user.c_str(), *curr) == 0)  {
        return Napi::Boolean::New(env, true);
    }
  }
  return Napi::Boolean::New(env, false);
}
