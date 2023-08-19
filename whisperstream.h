#ifndef WHISPERSTREAM_H
#define WHISPERSTREAM_H

#include <QObject>
#include <QThread>
#include <QString>
#include <thread>

class WhisperStream : public QObject
{
    Q_OBJECT
public:
    explicit WhisperStream(QObject *parent = nullptr);
    ~WhisperStream();

    void startWhisper();
    void stopWhisper();

    void printf(const char *fmt, ...);

signals:
    void sendMessage(QString msg);

public slots:


private:
    QScopedPointer<QThread> mQThr;
    std::thread mThr;
    bool mDone = false;
    QString mModel = "models/ggml-small.bin";
    QString mLng = "ru";

    void work();

};

#endif // WHISPERSTREAM_H
