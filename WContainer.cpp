#include "WContainer.h"

#include <QCheckBox>
#include <QPainter>
#include <QVBoxLayout>
#include <QPushButton>

WContainerControl::WContainerControl()
{

}

void WContainerControl::create_body(QWidget *parent)
{
    body = create_scrollarea(parent);
}

void WContainerControl::emplace(const QString &name, QWidget *body, bool visible)
{
    auto c = create_hidden_box(name, body, visible);
    insertToScrollArea(this->body, c->main);
    containers[name] = c;
}

QImage WContainerControl::create_image_switchoff(int w, int h, QColor back, QColor front)
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

QIcon WContainerControl::create_icon_switchoff(int w, int h, QColor back, QColor front)
{
    QPixmap pix = QPixmap::fromImage(create_image_switchoff(w, h, back, front));
    return QIcon(pix);
}

PWContainer WContainerControl::create_hidden_box(const QString &name, QWidget *body, bool visible)
{
    if(!body){
        return {};
    }
    PWContainer pw(new WContainer);

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

    pw->name = name;
    pw->header = butthide;
    pw->body = body;
    pw->main = main;

    return pw;
}

QWidget *WContainerControl::create_scrollarea(QWidget *parent)
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

void WContainerControl::create_example_body(const QString &name)
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

    emplace(name, body);
}

void WContainerControl::insertToScrollArea(QWidget *body, QWidget *w)
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

void WContainerControl::insertToScrollArea(QWidget *body, QLayoutItem *li)
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

void WContainerControl::insertVerticalSpacer(QWidget *scroll)
{
    if(!scroll){
        return;
    }
    QSpacerItem* si = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Expanding);
    insertToScrollArea(scroll, si);
}

QScrollArea *WContainerControl::create_test_scrollarea(QWidget *parent)
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
