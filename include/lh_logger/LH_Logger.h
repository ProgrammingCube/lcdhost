
#include <QtGlobal>
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <QHash>

#if defined(LH_LOGGER_LIBRARY)
# define LH_LOGGER_EXPORT Q_DECL_EXPORT
#else
# define LH_LOGGER_EXPORT Q_DECL_IMPORT
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
typedef QtMessageHandler lh_log_handler_t;
#else
typedef QtMsgHandler lh_log_handler_t;
#endif

class LH_LOGGER_EXPORT LH_Logger : public QObject
{
    Q_OBJECT

public:
    static LH_Logger *lock();

    LH_Logger(const QString &app_name = QString(), QObject *parent = 0);
    virtual ~LH_Logger();

    QString fileName() const;
    void removeOld(int days_old = 30) const;
    void log(QtMsgType type, QString msg);
    void unlock() const;

signals:
    void logged(uint msgtime, int msgtype, QString msgtext) const;

protected:
    static QMutex mutex_;
    static LH_Logger *instance_;

    QString app_name_;
    QString log_path_;
    QByteArray file_name_;
};

LH_LOGGER_EXPORT QString lh_data_dir();
LH_LOGGER_EXPORT QString lh_log_dir();

#if defined(Q_OS_WIN)
LH_LOGGER_EXPORT QString Win32Message( int dw );
#endif
