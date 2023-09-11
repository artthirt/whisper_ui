#ifndef PAINTSPECTRUM_H
#define PAINTSPECTRUM_H

#include <QWidget>
#include <QMutex>

class PaintSpectrum: public QWidget
{
    Q_OBJECT
public:
    PaintSpectrum(QWidget *parent = nullptr);

public slots:
    void onRecvData(const QByteArray& data);

private:
    QMutex mMut;
    QByteArray mBuf;
    float mHMax = 0;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PAINTSPECTRUM_H
