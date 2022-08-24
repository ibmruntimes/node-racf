/*
 * Licensed Materials - Property of IBM
 * (C) Copyright IBM Corp. 2019. All Rights Reserved.
 * US Government Users Restricted Rights - Use, duplication or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/

#ifndef RACF_H
#define RACF_H

#include <napi.h>
#include <string>

class Racf : public Napi::ObjectWrap<Racf> {
 public:
  static Napi::Boolean authenticate(const Napi::CallbackInfo &info);
  static Napi::Boolean isUserInGroup(const Napi::CallbackInfo &info);
  static Napi::Boolean changePassword(const Napi::CallbackInfo &info);
  static Napi::Boolean checkPermission(const Napi::CallbackInfo &info);
  static Napi::String getUserName(const Napi::CallbackInfo &info);

 private:
};

#endif
