#include "operator.h"
#include <QDebug>

Operator::Operator(size_t health,
    int damage,
    size_t interval,
    Place* place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    size_t block,
    Orientation orientation)
    : Infected(health, damage, interval, place,
        deployment_time, id, parent)
    , _block(block)
    , _orientation(orientation)
{
    //? 要show才能调用绘图事件！！！
    show();
}

void Operator::action(size_t time, Infected* attacked)
{
    if (_is_active == false || (time - _last_action_time) < _interval) {
        return;
    }
    if (attacked) { //* 暂时无视阻挡数要求
        _last_action_time = time;
        //* 类的函数不一定都是成员函数，定义在类外有时不失为一种选择
        attacked->reduceHealth(_damage);
        qDebug() << "\tATTACK" << qPrintable(giveName())
                 << "#" << giveID()
                 << "***" << qPrintable(attacked->giveName())
                 << "#" << attacked->giveID()
                 << "HHH -" << _damage
                 << ">>" << attacked->giveHealth();
        if (!attacked->isActive()) {
            qDebug() << "\tKILLED" << qPrintable(attacked->giveName())
                     << "#" << attacked->giveID();
            attacked->hide();
        }
        update();
    }
}

void Operator::addTo(Place* place)
{
    _place = place;
    place->addOperator(this);
    setGeometry(_place->x(), _place->y(), 100, 100);
}

void Operator::removeFrom()
{
    _place->removeOperator();
    _place = nullptr;
}

//* 把干员的朝向显示出来
void Operator::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    //* 画血条
    QBrush red_brush(QColor("#EE0000"));
    painter.setBrush(red_brush);
    float rate = 1.0 * _health / _max_health;
    painter.drawRect(10, 5, rate * 80, 5);
    //* 说明血量
    painter.setPen(QPen(Qt::green, 5));
    painter.drawText(10, 10, QString::number(_health) + " / " + QString::number(_max_health));
    //* 说明朝向
    switch (_orientation) {
    case UP:
        painter.drawText(90, 90, "N");
        break;
    case DOWN:
        painter.drawText(90, 90, "S");
        break;
    case LEFT:
        painter.drawText(90, 90, "W");
        break;
    case RIGHT:
        painter.drawText(90, 90, "E");
        break;
    }
}

Sniper::Sniper(size_t health,
    int damage,
    size_t interval,
    HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation)
    : Operator(health, damage, interval, higher_place, deployment_time, id, parent, 0, orientation)
{
    setGeometry(_place->x() - 5, _place->y() - 8, 100, 100);
}

Guard::Guard(size_t health,
    int damage,
    size_t interval,
    LowerPlace* lower_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    size_t block,
    Orientation orientation)
    : Operator(health, damage, interval, lower_place, deployment_time, id, parent, block, orientation)
{
    setGeometry(_place->x() - 15, _place->y(), 100, 100);
}

Irene::Irene(LowerPlace* lower_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation)
    : Guard(100, 10, 30, lower_place, deployment_time, id, parent, 3, orientation)
{
    _place->addOperator(this);
}

void Irene::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap Irene("://res/operator/Irene.png");
    painter.drawPixmap(0, 0, 100, 100, Irene);
    Operator::paintEvent(event);
}

Kroos::Kroos(HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation)
    : Sniper(100, 5, 20, higher_place, deployment_time, id, parent, orientation)
{
}

void Kroos::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap Kroos("://res/operator/Kroos.png");
    painter.drawPixmap(0, 0, 100, 100, Kroos);
    Operator::paintEvent(event);
}
