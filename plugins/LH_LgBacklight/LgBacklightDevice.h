#ifndef LGBACKLIGHTDEVICE_H
#define LGBACKLIGHTDEVICE_H

#include <QString>
#include <QByteArray>
#include <QColor>

class LH_HidDevice;

typedef struct LgBacklightReport_
{
    uchar hidprefix;
    uchar bReportId;
    uchar Red;
    uchar Green;
    uchar Blue;
} LgBacklightReport;

class LgBacklightDevice : public QObject
{
    Q_OBJECT

    LH_HidDevice *hd_;
    QColor color_;

public:
    LgBacklightDevice(LH_HidDevice *hd, QObject *parent);

    LH_HidDevice *hd() const { return hd_; }
    QColor color() const { return color_; }
    void setColor(QColor c);

    QColor getDeviceColor();
    bool setDeviceColor(const QColor &c);

signals:
    void colorChanged();
};

#endif // LGBACKLIGHTDEVICE_H