// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: IM.Other.proto

#ifndef PROTOBUF_IM_2eOther_2eproto__INCLUDED
#define PROTOBUF_IM_2eOther_2eproto__INCLUDED

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
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
// @@protoc_insertion_point(includes)

namespace protobuf_IM_2eOther_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void InitDefaultsIMHeartBeatImpl();
void InitDefaultsIMHeartBeat();
inline void InitDefaults() {
  InitDefaultsIMHeartBeat();
}
}  // namespace protobuf_IM_2eOther_2eproto
namespace IM {
namespace Other {
class IMHeartBeat;
class IMHeartBeatDefaultTypeInternal;
extern IMHeartBeatDefaultTypeInternal _IMHeartBeat_default_instance_;
}  // namespace Other
}  // namespace IM
namespace IM {
namespace Other {

// ===================================================================

class IMHeartBeat : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:IM.Other.IMHeartBeat) */ {
 public:
  IMHeartBeat();
  virtual ~IMHeartBeat();

  IMHeartBeat(const IMHeartBeat& from);

  inline IMHeartBeat& operator=(const IMHeartBeat& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  IMHeartBeat(IMHeartBeat&& from) noexcept
    : IMHeartBeat() {
    *this = ::std::move(from);
  }

  inline IMHeartBeat& operator=(IMHeartBeat&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const IMHeartBeat& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const IMHeartBeat* internal_default_instance() {
    return reinterpret_cast<const IMHeartBeat*>(
               &_IMHeartBeat_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(IMHeartBeat* other);
  friend void swap(IMHeartBeat& a, IMHeartBeat& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline IMHeartBeat* New() const PROTOBUF_FINAL { return New(NULL); }

  IMHeartBeat* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const IMHeartBeat& from);
  void MergeFrom(const IMHeartBeat& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  void DiscardUnknownFields();
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(IMHeartBeat* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::std::string GetTypeName() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:IM.Other.IMHeartBeat)
 private:

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  mutable int _cached_size_;
  friend struct ::protobuf_IM_2eOther_2eproto::TableStruct;
  friend void ::protobuf_IM_2eOther_2eproto::InitDefaultsIMHeartBeatImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// IMHeartBeat

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Other
}  // namespace IM

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_IM_2eOther_2eproto__INCLUDED
