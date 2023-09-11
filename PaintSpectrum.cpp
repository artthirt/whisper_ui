#include "PaintSpectrum.h"
#include <QPainter>

PaintSpectrum::PaintSpectrum(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(false);
}

void PaintSpectrum::onRecvData(const QByteArray &data)
{
    QMutexLocker lock(&mMut);
    mBuf = data;
    update();
}

void PaintSpectrum::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    if(mBuf.isEmpty() || !mMut.tryLock()){
        return;
    }

    auto buf = mBuf;
    mMut.unlock();

    float *fd = reinterpret_cast<float*>(buf.data());
    size_t sz = buf.size() / 4;

    float fmins = 99999999999.f;
    float fmaxs = -99999999999.f;
    for(size_t i = 0; i < sz; ++i){
        fmins = fmin(fmins, fd[i]);
        fmaxs = fmax(fmaxs, fd[i]);
    }
    float hmax = fmax(mHMax, fmax(fabs(fmins), fabs(fmaxs)));
    mHMax = hmax;

    float dx = 1. * width() / sz;
    float h = height()/2;
    QPolygon ply;
    int prevx = -1;
    for(size_t i = 0; i < sz; ++i){
        float x = i * dx;
        float y = h - fd[i] * h/hmax;
        if(prevx == int(x) && !ply.empty()){
            ply.back() = QPoint(x, y);
        }else{
            ply << QPoint(x, y);
        }
        prevx = x;
    }
    painter.setPen(Qt::black);
    painter.drawLines(ply);
}
