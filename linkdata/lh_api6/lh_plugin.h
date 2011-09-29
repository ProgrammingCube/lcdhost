#ifndef LH_API6_LH_PLUGIN_H
#define LH_API6_LH_PLUGIN_H

#include <QtPlugin>
#include "lh_object.h"

namespace lh {
namespace api6 {

class plugin : public object
{
    Q_OBJECT
    Q_INTERFACES( lh::api6::object )

public:
    /**
        Definition of the signature area
        The signature area is optional by highly recommended.
        Note that the signature needs to kept in the library. If
        it's not marked as an export, and not referenced anywhere,
        smart linkers are liable to simply remove it.

        \sa LH_EMPTY_SIGNATURE
    */
    typedef struct
    {
        char marker[16]; /**< \sa LH_SIGNATURE_MARKER */
        char sign[256]; /**< 2048-bit RSA signature of the shared plugin's SHA-1 digest, PKCS1 padded */
        char url[128]; /**< URL to the public key */
        int size; /**< sizeof(signature) */
    } signature;

    plugin() : object()
    {}
};

} // namespace api6
} // namespace lh

/**
    Signature marker so SignPlugin can find it
    \sa lh_signature
*/
#define LH_SIGNATURE_MARKER {7,98,120,242,114,174,176,97,178,246,229,116,243,34,2,92}

/**
    Define a blank signature area
    \sa lh_signature
*/
#define LH_SIGNATURE_BLANK { LH_SIGNATURE_MARKER, {0}, {0}, sizeof(lh::api6::plugin::signature) }

/**
  This macro creates the required exported functions
  for your LH_QtPlugin descendant.
  */
#define LH_PLUGIN(classname) \
    Q_EXPORT_PLUGIN2(classname,classname) \
    lh::api6::plugin::signature lh_##classname##_signature = LH_SIGNATURE_BLANK; \
    char lh_##classname##_xml[]

#endif // LH_API6_LH_PLUGIN_H