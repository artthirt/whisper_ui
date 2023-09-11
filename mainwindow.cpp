#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QCheckBox>
#include <QSpacerItem>
#include <QGroupBox>
#include <QImage>
#include <QPainter>
#include <QPen>

#include "WContainer.h"

///////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mWhisper.reset(new WhisperStream);
    connect(mWhisper.get(), &WhisperStream::sendMessage, this, &MainWindow::onSendMessage, Qt::QueuedConnection);

//    WContainerControl ctrl;

//    auto tab1 = ui->twTabs->widget(0);
//    ctrl.create_body(tab1);

//    ctrl.create_example_body("box 1");

//    ctrl.create_example_body("box 2");

//    for(int i = 0; i < 40; ++i){
//        ctrl.create_example_body("box item " + QString::number(i + 1));
//    }
//    auto test1 = create_test_scrollarea(tab1);


    //insertVerticalSpacer(scroll);
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

