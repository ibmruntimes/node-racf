/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2019. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

#include "racf_interface.h"

#include <algorithm>
#include <dynit.h>
#include <node_buffer.h>
#include <numeric>
#include <pwd.h>
#include <sstream>
#include <sys/ps.h>
#include <sys/types.h>
#include <unistd.h>
// Needs to go at the end due to naming collision
#include <grp.h>

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

  // Uppercase user and group
  std::for_each(user.begin(), user.end(), [](char & c){
      c = ::toupper(c);
  });
  std::for_each(group.begin(), group.end(), [](char & c){
      c = ::toupper(c);
  });

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

Napi::Boolean Racf::changePassword(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 3) {
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
  if (!info[2].IsString()) {
    Napi::TypeError::New(env, "Third argument must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  std::string username (static_cast<std::string>(info[0].As<Napi::String>()));
  std::string old_password (static_cast<std::string>(info[1].As<Napi::String>()));
  std::string new_password (static_cast<std::string>(info[2].As<Napi::String>()));

  // Change password via __passwd interface
  int value = __passwd(username.c_str(), old_password.c_str(), new_password.c_str());

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

Napi::String Racf::getUserName(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  char buffer[1024];
  int rc = __getuserid(buffer, 1024);
  
  if (rc < 0) {
	  strerror_r(errno, buffer, 1024);	
    Napi::TypeError::New(env, buffer)
        .ThrowAsJavaScriptException();
    return Napi::String::New(env, std::string(""));
   }
  __e2a_s(buffer);
  return Napi::String::New(env, std::string(buffer));
}

Napi::Boolean Racf::checkPermission(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 4) {
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
  if (!info[2].IsString()) {
    Napi::TypeError::New(env, "Third argument must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }
  if (!info[3].IsString()) {
    Napi::TypeError::New(env, "Fourth argument must be a string")
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  std::string userName (static_cast<std::string>(info[0].As<Napi::String>()));
  std::string className (static_cast<std::string>(info[1].As<Napi::String>()));
  std::string entityName (static_cast<std::string>(info[2].As<Napi::String>()));
  std::string accessLevel (static_cast<std::string>(info[3].As<Napi::String>()));

  // Change password via __passwd interface
  int access_type = __READ_RESOURCE;
  if (accessLevel == "READ")
    access_type = __READ_RESOURCE;
  else if (accessLevel == "UPDATE")
    access_type = __UPDATE_RESOURCE;
  else if (accessLevel == "CONTROL")
    access_type = __CONTROL_RESOURCE;
  else if (accessLevel == "ALTER")
    access_type = __ALTER_RESOURCE;

  int value = __check_resource_auth_np(0, 0, (char*)userName.c_str(), (char*)accessLevel.c_str(), (char*)entityName.c_str(), access_type);

  if (value != 0) {
    char buffer[1024];
	  strerror_r(errno, buffer, 1024);	
    Napi::TypeError::New(env, buffer)
        .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }
  return Napi::Boolean::New(env, true);
}
