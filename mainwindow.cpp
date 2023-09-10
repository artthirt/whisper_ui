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

QImage create_image_switchoff(int w, int h, QColor back, QColor front)
{
    QImage img(w, h, QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(img.rect(), back);
    QPen pen(front, 3);
    painter.setPen(pen);
    painter.drawEllipse(4, 4, w-8, h-8);
    painter.drawLine(w/2, 2, w/2, h/2);
    return img;
}

QIcon create_icon_switchoff(int w = 20, int h = 20,
                            QColor back = Qt::white,
                            QColor front = QColor(qRgb(20, 20, 20)))
{
    QPixmap pix = QPixmap::fromImage(create_image_switchoff(w, h, back, front));
    return QIcon(pix);
}

QWidget* create_hidden_box(const QString& name, QWidget* body, bool visible = false)
{
    if(!body){
        return nullptr;
    }
    QWidget* main = new QWidget();
    QVBoxLayout *vl = new QVBoxLayout();
    vl->setSpacing(3);
    vl->setContentsMargins(3, 3, 3, 3);
    main->setLayout(vl);

    QPushButton *butthide = new QPushButton(name);
    butthide->setStyleSheet("QPushButton{text-align:left;background-color:white;"
                            "border: 1px solid #ccc;height:25px;"
                            "padding-left: 3px;border-radius:3px;}"
                            "QPushButton:hover{background-color:#f0f0f0;}"
                            "QPushButton:pressed{background-color:#e0e0e0;}");
    butthide->setIcon(create_icon_switchoff(24, 24));
    int hb = body->height();
    int hp = butthide->height();

    vl->addWidget(butthide);
    vl->addWidget(body);

    body->setVisible(visible);
    body->setObjectName("Body");
    body->setStyleSheet("QWidget#Body{border: 1px solid #aaa; border-radius: 3px;}");

    butthide->setCheckable(true);
    QObject::connect(butthide, &QPushButton::clicked, [body](bool ch){
        body->setVisible(ch);
    });

    return main;
}

QWidget* create_scrollarea(QWidget *parent)
{
    if(parent == nullptr){
        return {};
    }
    parent->setMinimumWidth(200);
    auto scroll = new QScrollArea();
    auto vl = parent->layout();
    if(!vl){
        vl = new QVBoxLayout;
        vl->setSpacing(1);
        vl->setContentsMargins(0, 0, 0, 0);
        parent->setLayout(vl);
    }
    vl->addWidget(scroll);
    QWidget *body = new QWidget;
    body->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scroll->setWidget(body);
    scroll->setWidgetResizable(true);

    return body;
}

QWidget *create_example_body()
{
    QWidget* body = new QWidget;
    QVBoxLayout *vl = new QVBoxLayout();
    body->setLayout(vl);

    QCheckBox* ch1 = new QCheckBox("check 1");
    QCheckBox* ch2 = new QCheckBox("check 2");
    QCheckBox* ch3 = new QCheckBox("check 3");
    vl->addWidget(ch1);
    vl->addWidget(ch2);
    vl->addWidget(ch3);

    return body;
}

void insertToScrollArea(QWidget* body, QWidget* w)
{
    if(!body || !w){
        return;
    }
    auto lt = body->layout();
    if(!lt){
        lt = new QVBoxLayout;
        lt->setSpacing(1);
        lt->setContentsMargins(0, 0, 0, 0);
        body->setLayout(lt);
    }
    lt->addWidget(w);
}

void insertToScrollArea(QWidget* body, QLayoutItem* li)
{
    if(!body || !li){
        return;
    }
    auto lt = body->layout();
    if(!lt){
        lt = new QVBoxLayout;
        lt->setSpacing(1);
        body->setLayout(lt);
    }
    lt->addItem(li);
}

void insertVerticalSpacer(QWidget* scroll)
{
    if(!scroll){
        return;
    }
    QSpacerItem* si = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Expanding);
    insertToScrollArea(scroll, si);
}

QScrollArea* create_test_scrollarea(QWidget* parent)
{
    parent->setMinimumWidth(200);
    QScrollArea *_materialScrollArea = new QScrollArea();

    QWidget *materialContainer = new QWidget();
    QVBoxLayout *materialLay = new QVBoxLayout();
    materialContainer->setLayout(materialLay);
    _materialScrollArea->setWidget(materialContainer);
    _materialScrollArea->setWidgetResizable(true);

    for(int i = 0; i < 50; ++i)
    {
        QCheckBox *chBox = new QCheckBox("Name i");
        materialLay->addWidget(chBox);
    }

    auto vl = parent->layout();
    if(!vl){
        vl = new QVBoxLayout;
        parent->setLayout(vl);
    }
    vl->addWidget(_materialScrollArea);

    return _materialScrollArea;
}

///////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mWhisper.reset(new WhisperStream);
    connect(mWhisper.get(), &WhisperStream::sendMessage, this, &MainWindow::onSendMessage, Qt::QueuedConnection);

    auto tab1 = ui->twTabs->widget(0);
    auto body = create_scrollarea(tab1);

    auto body1 = create_example_body();
    auto main1 = create_hidden_box("box 1", body1);
    insertToScrollArea(body, main1);

    auto body2 = create_example_body();
    auto main2 = create_hidden_box("box 2", body2);
    insertToScrollArea(body, main2);

    for(int i = 0; i < 40; ++i){
        auto bodyi = create_example_body();
        auto maini = create_hidden_box("box item " + QString::number(i + 1), bodyi);
        insertToScrollArea(body, maini);
    }
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

