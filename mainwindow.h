#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "whisperstream.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbStart_clicked();

    void on_pbStop_clicked();

    void onSendMessage(QString msg);

private:
    Ui::MainWindow *ui;

    QScopedPointer<WhisperStream> mWhisper;

    void keyControl(int key);
    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_H
