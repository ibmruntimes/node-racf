/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2022. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

#include "racf_interface.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("authenticate", Napi::Function::New(env, Racf::authenticate));
  exports.Set("isUserInGroup", Napi::Function::New(env, Racf::isUserInGroup));
  exports.Set("checkPermission", Napi::Function::New(env, Racf::checkPermission));
  exports.Set("changePassword", Napi::Function::New(env, Racf::changePassword));
  exports.Set("getUserName", Napi::Function::New(env, Racf::getUserName));
  return exports;
}

NODE_API_MODULE(racf, Init)
