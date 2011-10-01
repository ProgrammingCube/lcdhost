#ifndef LH_API6_LH_SOURCE_H
#define LH_API6_LH_SOURCE_H

#include "lh_linkable.h"

namespace lh {
namespace api6 {

/**
  source is a convenience class for creating
  a data source that need no UI or to be saved.
  */
class source : public linkable
{
    Q_OBJECT
    Q_INTERFACES( lh::api6::object lh::api6::linkable )

public:
    explicit source(
            object & parent,
            const char * ident,
            const QString & path,
            const QVariant & val = QVariant(),
            const QVariant & min = QVariant(),
            const QVariant & max = QVariant()
            ) :
        linkable( parent, ident, path, true, val, min, max )
    {
    }
};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::source, "se.linkdata.lh_source/6.0" )

#endif // LH_API6_LH_SOURCE_H
