/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: message.proto */

#ifndef PROTOBUF_C_message_2eproto__INCLUDED
#define PROTOBUF_C_message_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Response Response;


/* --- enums --- */


/* --- messages --- */

struct _Response {
    ProtobufCMessage base;
    char *stato;
    float risultato_parziale;
};
#define RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&response__descriptor) \
    , NULL, 0 }


/* Response methods */
void response__init
        (Response *message);

size_t response__get_packed_size
        (const Response *message);

size_t response__pack
        (const Response *message,
         uint8_t *out);

size_t response__pack_to_buffer
        (const Response *message,
         ProtobufCBuffer *buffer);

Response *
response__unpack
        (ProtobufCAllocator *allocator,
         size_t len,
         const uint8_t *data);

void response__free_unpacked
        (Response *message,
         ProtobufCAllocator *allocator);

/* --- per-message closures --- */

typedef void (*Response_Closure)
        (const Response *message,
         void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor response__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_message_2eproto__INCLUDED */
