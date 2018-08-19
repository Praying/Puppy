// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: IM.Other.proto

#include "IM.Other.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace IM {
namespace Other {
class IMHeartBeatDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<IMHeartBeat>
      _instance;
} _IMHeartBeat_default_instance_;
}  // namespace Other
}  // namespace IM
namespace protobuf_IM_2eOther_2eproto {
void InitDefaultsIMHeartBeatImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::IM::Other::_IMHeartBeat_default_instance_;
    new (ptr) ::IM::Other::IMHeartBeat();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::IM::Other::IMHeartBeat::InitAsDefaultInstance();
}

void InitDefaultsIMHeartBeat() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsIMHeartBeatImpl);
}

}  // namespace protobuf_IM_2eOther_2eproto
namespace IM {
namespace Other {

// ===================================================================

void IMHeartBeat::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

IMHeartBeat::IMHeartBeat()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_IM_2eOther_2eproto::InitDefaultsIMHeartBeat();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:IM.Other.IMHeartBeat)
}
IMHeartBeat::IMHeartBeat(const IMHeartBeat& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:IM.Other.IMHeartBeat)
}

void IMHeartBeat::SharedCtor() {
  _cached_size_ = 0;
}

IMHeartBeat::~IMHeartBeat() {
  // @@protoc_insertion_point(destructor:IM.Other.IMHeartBeat)
  SharedDtor();
}

void IMHeartBeat::SharedDtor() {
}

void IMHeartBeat::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const IMHeartBeat& IMHeartBeat::default_instance() {
  ::protobuf_IM_2eOther_2eproto::InitDefaultsIMHeartBeat();
  return *internal_default_instance();
}

IMHeartBeat* IMHeartBeat::New(::google::protobuf::Arena* arena) const {
  IMHeartBeat* n = new IMHeartBeat;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void IMHeartBeat::Clear() {
// @@protoc_insertion_point(message_clear_start:IM.Other.IMHeartBeat)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear();
}

bool IMHeartBeat::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::LiteUnknownFieldSetter unknown_fields_setter(
      &_internal_metadata_);
  ::google::protobuf::io::StringOutputStream unknown_fields_output(
      unknown_fields_setter.buffer());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_output, false);
  // @@protoc_insertion_point(parse_start:IM.Other.IMHeartBeat)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormatLite::SkipField(
        input, tag, &unknown_fields_stream));
  }
success:
  // @@protoc_insertion_point(parse_success:IM.Other.IMHeartBeat)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:IM.Other.IMHeartBeat)
  return false;
#undef DO_
}

void IMHeartBeat::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:IM.Other.IMHeartBeat)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  output->WriteRaw((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).data(),
                   static_cast<int>((::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size()));
  // @@protoc_insertion_point(serialize_end:IM.Other.IMHeartBeat)
}

size_t IMHeartBeat::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:IM.Other.IMHeartBeat)
  size_t total_size = 0;

  total_size += (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()).size();

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void IMHeartBeat::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const IMHeartBeat*>(&from));
}

void IMHeartBeat::MergeFrom(const IMHeartBeat& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:IM.Other.IMHeartBeat)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void IMHeartBeat::CopyFrom(const IMHeartBeat& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:IM.Other.IMHeartBeat)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool IMHeartBeat::IsInitialized() const {
  return true;
}

void IMHeartBeat::Swap(IMHeartBeat* other) {
  if (other == this) return;
  InternalSwap(other);
}
void IMHeartBeat::InternalSwap(IMHeartBeat* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::std::string IMHeartBeat::GetTypeName() const {
  return "IM.Other.IMHeartBeat";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Other
}  // namespace IM

// @@protoc_insertion_point(global_scope)
