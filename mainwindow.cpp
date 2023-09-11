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

#include <Windows.h>

#include "WContainer.h"

///////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    ui->pbFocus->setStyleSheet("QPushButton{border: 0;background-color:white;height: 30px;width 40px;}"
                               "QPushButton:focus{border: 2px dotted #444;}");
    ui->pbFocus->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbStart_clicked()
{
    auto args = ui->leArgs->text().split(' ');
    for(auto &it: args){
        it = it.trimmed();
    }
    args.push_front("app");
    mWhisper.reset(new WhisperStream(args));
    connect(mWhisper.get(), &WhisperStream::sendMessage, this, &MainWindow::onSendMessage, Qt::QueuedConnection);
    connect(mWhisper.get(), &WhisperStream::sendSound, ui->widgetSound, &PaintSpectrum::onRecvData, Qt::QueuedConnection);
    mWhisper->startWhisper();
}

void MainWindow::on_pbStop_clicked()
{
    if(mWhisper)
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

void updatePos(const POINT &pos, int dx, int dy)
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;

    Input.mi.dx = (LONG)pos.x + dx;
    Input.mi.dy = (LONG)pos.y + dy;

    // set move cursor directly
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &Input, sizeof(INPUT));

}

void mouseClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,0,0,0,0);
}

void drawOnScreen(int x, int y)
{
    /* Calling GetDC with argument 0 retrieves the desktop's DC */
    HDC hDC_Desktop = GetDC(0);

    /* Draw a simple blue rectangle on the desktop */
    RECT rect = { x - 10, y + 10, x + 10, y + 10 };
    HBRUSH blueBrush=CreateSolidBrush(RGB(0,0,255));
    FillRect(hDC_Desktop, &rect, blueBrush);
}

//#define DBG(x) qDebug("dbg: %d", x)
void DBG(int x)
{
    qDebug("dbg: %d", x);

    POINT pos;
    ::GetCursorPos(&pos);
    drawOnScreen(pos.x, pos.y);
}

void MainWindow::keyControl(int key)
{
    POINT pos;
    ::GetCursorPos(&pos);
    qDebug("mouse current pos: %d %d", pos.x, pos.y);
    if(key == 'W'){
        DBG(::SetCursorPos(pos.x, pos.y - 15));
    }
    if(key == 'A'){
        DBG(::SetCursorPos(pos.x - 15, pos.y));
    }
    if(key == 'S'){
        DBG(::SetCursorPos(pos.x, pos.y + 15));
    }
    if(key == 'D'){
        DBG(::SetCursorPos(pos.x + 15, pos.y));
    }
    if(key == 'Q'){
        mouseClick();
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->pbFocus && event->type() == QEvent::KeyPress){
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        qDebug("key %d", ke->key());
        keyControl(ke->key());
    }
    return QMainWindow::eventFilter(watched, event);
}
