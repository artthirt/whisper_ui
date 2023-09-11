#ifndef WCONTAINER_H
#define WCONTAINER_H

#include <QImage>
#include <QIcon>
#include <QScrollArea>
#include <QLayoutItem>
#include <QSharedPointer>
#include <QMap>

class WContainer{
public:
    QString name;
    QWidget *header = nullptr;
    QWidget *body = nullptr;
    QWidget *main = nullptr;
};

using PWContainer = QSharedPointer<WContainer>;

class WContainerControl
{
public:
    QWidget *body = nullptr;
    QMap<QString, PWContainer> containers;

    WContainerControl();

    void create_body(QWidget *parent);
    void emplace(const QString& name, QWidget* body, bool visible = false);

    QImage create_image_switchoff(int w, int h, QColor back, QColor front);

    QIcon create_icon_switchoff(int w = 20, int h = 20,
                                QColor back = Qt::white,
                                QColor front = QColor(qRgb(20, 20, 20)));

    PWContainer create_hidden_box(const QString& name, QWidget* body, bool visible = false);

    QWidget* create_scrollarea(QWidget *parent);

    void create_example_body(const QString& name);

    void insertToScrollArea(QWidget* body, QWidget* w);

    void insertToScrollArea(QWidget* body, QLayoutItem* li);

    void insertVerticalSpacer(QWidget* scroll);

    QScrollArea* create_test_scrollarea(QWidget* parent);
};

#endif // WCONTAINER_H
