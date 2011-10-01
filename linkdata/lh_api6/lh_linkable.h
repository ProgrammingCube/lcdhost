#ifndef LH_API6_LH_LINKABLE_H
#define LH_API6_LH_LINKABLE_H

#include <QVariant>
#include <QString>
#include "lh_object.h"

namespace lh {
namespace api6 {

/**
  linkable provides the basic data linking
  functionality, and storage for the value, minimum and
  maximum properties. Those three are the ones transferred
  when data linking. Note that this object doesn't save
  itself between sessions, nor does it provide an UI.
  */
class linkable : public object
{
    Q_OBJECT
    Q_INTERFACES( lh::api6::object )
    Q_PROPERTY( QString linkOut READ linkOut WRITE setLinkOut STORED isStored )
    Q_PROPERTY( QString linkIn READ linkIn WRITE setLinkIn STORED isStored )
    Q_PROPERTY( QString linkFilter READ linkFilter WRITE setLinkFilter STORED isStored )
    Q_PROPERTY( QVariant value READ value WRITE setValue STORED isStored )
    Q_PROPERTY( QVariant minimum READ minimum WRITE setMinimum STORED isStored )
    Q_PROPERTY( QVariant maximum READ maximum WRITE setMaximum STORED isStored )

    QString link_out_;
    QString link_in_;
    QString link_filter_;
    QVariant value_;
    QVariant minimum_;
    QVariant maximum_;

public:
    linkable( object & parent,
              const char * ident,
              const QString & path = QString(),
              bool is_source = false,
              const QVariant & val = QVariant(),
              const QVariant & min = QVariant(),
              const QVariant & max = QVariant()
              ) :
        object( parent, ident ),
        link_out_( is_source ? path : QString() ),
        link_in_( is_source ? QString() : path ),
        value_( val ),
        minimum_( min ),
        maximum_( max )
    {
    }

    virtual bool isStored() const { return false; }

    const QString & linkOut() const { return link_out_; }
    const QString & linkIn() const { return link_in_; }
    const QString & linkFilter() const { return link_filter_; }
    const QVariant & value() const { return value_; }
    const QVariant & minimum() const { return minimum_; }
    const QVariant & maximum() const { return maximum_; }
    bool isSource() const { return !link_out_.isEmpty(); }
    bool isSink() const { return !link_in_.isEmpty(); }

signals:
    void linkOutChanged( const QString & );
    void linkInChanged( const QString & );
    void linkFilterChanged( const QString & );
    void valueChanged( const QVariant & );
    void minimumChanged( const QVariant & );
    void maximumChanged( const QVariant & );

public slots:
    void setLinkOut( const QString & path = QString() );
    void setLinkIn( const QString & path = QString() );
    void setLinkFilter( const QString & filter = QString() );
    void setValue( const QVariant & v );
    void setMinimum( const QVariant & v );
    void setMaximum( const QVariant & v );
};

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::linkable, "se.linkdata.lh_linkable/6.0" )

#endif // LH_API6_LH_LINKABLE_H
