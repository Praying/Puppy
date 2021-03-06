// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ClientCmd.proto

#ifndef PROTOBUF_ClientCmd_2eproto__INCLUDED
#define PROTOBUF_ClientCmd_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "Cmd.pb.h"
// @@protoc_insertion_point(includes)

namespace protobuf_ClientCmd_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsLoginCmdImpl();
void InitDefaultsLoginCmd();
inline void InitDefaults() {
  InitDefaultsLoginCmd();
}
}  // namespace protobuf_ClientCmd_2eproto
class LoginCmd;
class LoginCmdDefaultTypeInternal;
extern LoginCmdDefaultTypeInternal _LoginCmd_default_instance_;

enum ClientParam {
  CLIENT_PARAM_NULL = 0,
  CLIENT_PARAM_LOGIN_AUTH = 1,
  ClientParam_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  ClientParam_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool ClientParam_IsValid(int value);
const ClientParam ClientParam_MIN = CLIENT_PARAM_NULL;
const ClientParam ClientParam_MAX = CLIENT_PARAM_LOGIN_AUTH;
const int ClientParam_ARRAYSIZE = ClientParam_MAX + 1;

const ::google::protobuf::EnumDescriptor* ClientParam_descriptor();
inline const ::std::string& ClientParam_Name(ClientParam value) {
  return ::google::protobuf::internal::NameOfEnum(
    ClientParam_descriptor(), value);
}
inline bool ClientParam_Parse(
    const ::std::string& name, ClientParam* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ClientParam>(
    ClientParam_descriptor(), name, value);
}
// ===================================================================

class LoginCmd : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:LoginCmd) */ {
 public:
  LoginCmd();
  virtual ~LoginCmd();

  LoginCmd(const LoginCmd& from);

  inline LoginCmd& operator=(const LoginCmd& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  LoginCmd(LoginCmd&& from) noexcept
    : LoginCmd() {
    *this = ::std::move(from);
  }

  inline LoginCmd& operator=(LoginCmd&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const LoginCmd& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const LoginCmd* internal_default_instance() {
    return reinterpret_cast<const LoginCmd*>(
               &_LoginCmd_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(LoginCmd* other);
  friend void swap(LoginCmd& a, LoginCmd& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline LoginCmd* New() const PROTOBUF_FINAL { return New(NULL); }

  LoginCmd* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const LoginCmd& from);
  void MergeFrom(const LoginCmd& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(LoginCmd* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string username = 3;
  void clear_username();
  static const int kUsernameFieldNumber = 3;
  const ::std::string& username() const;
  void set_username(const ::std::string& value);
  #if LANG_CXX11
  void set_username(::std::string&& value);
  #endif
  void set_username(const char* value);
  void set_username(const char* value, size_t size);
  ::std::string* mutable_username();
  ::std::string* release_username();
  void set_allocated_username(::std::string* username);

  // string password = 4;
  void clear_password();
  static const int kPasswordFieldNumber = 4;
  const ::std::string& password() const;
  void set_password(const ::std::string& value);
  #if LANG_CXX11
  void set_password(::std::string&& value);
  #endif
  void set_password(const char* value);
  void set_password(const char* value, size_t size);
  ::std::string* mutable_password();
  ::std::string* release_password();
  void set_allocated_password(::std::string* password);

  // .CommandType cmd = 1;
  void clear_cmd();
  static const int kCmdFieldNumber = 1;
  ::CommandType cmd() const;
  void set_cmd(::CommandType value);

  // .ClientParam param = 2;
  void clear_param();
  static const int kParamFieldNumber = 2;
  ::ClientParam param() const;
  void set_param(::ClientParam value);

  // @@protoc_insertion_point(class_scope:LoginCmd)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr username_;
  ::google::protobuf::internal::ArenaStringPtr password_;
  int cmd_;
  int param_;
  mutable int _cached_size_;
  friend struct ::protobuf_ClientCmd_2eproto::TableStruct;
  friend void ::protobuf_ClientCmd_2eproto::InitDefaultsLoginCmdImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LoginCmd

// .CommandType cmd = 1;
inline void LoginCmd::clear_cmd() {
  cmd_ = 0;
}
inline ::CommandType LoginCmd::cmd() const {
  // @@protoc_insertion_point(field_get:LoginCmd.cmd)
  return static_cast< ::CommandType >(cmd_);
}
inline void LoginCmd::set_cmd(::CommandType value) {
  
  cmd_ = value;
  // @@protoc_insertion_point(field_set:LoginCmd.cmd)
}

// .ClientParam param = 2;
inline void LoginCmd::clear_param() {
  param_ = 0;
}
inline ::ClientParam LoginCmd::param() const {
  // @@protoc_insertion_point(field_get:LoginCmd.param)
  return static_cast< ::ClientParam >(param_);
}
inline void LoginCmd::set_param(::ClientParam value) {
  
  param_ = value;
  // @@protoc_insertion_point(field_set:LoginCmd.param)
}

// string username = 3;
inline void LoginCmd::clear_username() {
  username_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& LoginCmd::username() const {
  // @@protoc_insertion_point(field_get:LoginCmd.username)
  return username_.GetNoArena();
}
inline void LoginCmd::set_username(const ::std::string& value) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:LoginCmd.username)
}
#if LANG_CXX11
inline void LoginCmd::set_username(::std::string&& value) {
  
  username_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:LoginCmd.username)
}
#endif
inline void LoginCmd::set_username(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:LoginCmd.username)
}
inline void LoginCmd::set_username(const char* value, size_t size) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:LoginCmd.username)
}
inline ::std::string* LoginCmd::mutable_username() {
  
  // @@protoc_insertion_point(field_mutable:LoginCmd.username)
  return username_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* LoginCmd::release_username() {
  // @@protoc_insertion_point(field_release:LoginCmd.username)
  
  return username_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginCmd::set_allocated_username(::std::string* username) {
  if (username != NULL) {
    
  } else {
    
  }
  username_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), username);
  // @@protoc_insertion_point(field_set_allocated:LoginCmd.username)
}

// string password = 4;
inline void LoginCmd::clear_password() {
  password_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& LoginCmd::password() const {
  // @@protoc_insertion_point(field_get:LoginCmd.password)
  return password_.GetNoArena();
}
inline void LoginCmd::set_password(const ::std::string& value) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:LoginCmd.password)
}
#if LANG_CXX11
inline void LoginCmd::set_password(::std::string&& value) {
  
  password_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:LoginCmd.password)
}
#endif
inline void LoginCmd::set_password(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:LoginCmd.password)
}
inline void LoginCmd::set_password(const char* value, size_t size) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:LoginCmd.password)
}
inline ::std::string* LoginCmd::mutable_password() {
  
  // @@protoc_insertion_point(field_mutable:LoginCmd.password)
  return password_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* LoginCmd::release_password() {
  // @@protoc_insertion_point(field_release:LoginCmd.password)
  
  return password_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void LoginCmd::set_allocated_password(::std::string* password) {
  if (password != NULL) {
    
  } else {
    
  }
  password_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), password);
  // @@protoc_insertion_point(field_set_allocated:LoginCmd.password)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::ClientParam> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ClientParam>() {
  return ::ClientParam_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ClientCmd_2eproto__INCLUDED
