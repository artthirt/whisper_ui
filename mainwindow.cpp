#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mWhisper.reset(new WhisperStream);
    connect(mWhisper.get(), &WhisperStream::sendMessage, this, &MainWindow::onSendMessage, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbStart_clicked()
{
    mWhisper->startWhisper();
}

void MainWindow::on_pbStop_clicked()
{
    mWhisper->stopWhisper();
}

void MainWindow::onSendMessage(QString msg)
{
    if(msg == "\33[2K\r"){
        ui->teLog->setFocus();
        QTextCursor storeCursorPos = ui->teLog->textCursor();
        ui->teLog->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->teLog->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        ui->teLog->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
        ui->teLog->textCursor().removeSelectedText();
        ui->teLog->textCursor().deletePreviousChar();
        ui->teLog->setTextCursor(storeCursorPos);
        //qDebug("line removed");
        return;
    }
    msg = msg.replace("\n", "");
    ui->teLog->appendPlainText(msg);

}

