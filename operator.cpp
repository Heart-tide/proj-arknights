#include "operator.h"
#include <QDebug>

Operator::Operator(size_t health,
    int damage,
    size_t interval,
    Place* place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    size_t block)
    : Infected(health, damage, interval, place,
        deployment_time, id, parent)
    , _block(block)
{
    //? 要show才能调用绘图事件！！！
    show();
}

void Operator::action(size_t time, QVector<Infected*>& reunions)
{
    if (_is_active == false || (time - _last_action_time) < _interval) {
        return;
    }
    if (!reunions.empty()) { //* 暂时无视阻挡数要求
        _last_action_time = time;
        //* 类的函数不一定都是成员函数，定义在类外有时不失为一种选择
        auto attacked = giveRandomInfected(reunions);
        attacked->reduceHealth(_damage);
        qDebug() << "\tATTACK" << qPrintable(giveName())
                 << "#" << giveId()
                 << "***" << qPrintable(attacked->giveName())
                 << "#" << attacked->giveId()
                 << "HHH -" << _damage
                 << ">>" << attacked->giveHealth();
        if (!attacked->isActive()) {
            qDebug() << "\tKILLED" << qPrintable(attacked->giveName())
                     << "#" << attacked->giveId();
            attacked->hide();
        }
    }
    update();
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

Sniper::Sniper(size_t health,
    int damage,
    size_t interval,
    HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent)
    : Operator(health, damage, interval, higher_place, deployment_time, id, parent, 0)
{
    setGeometry(_place->x() - 5, _place->y() - 13, 100, 100);
}

//* 把远程单位的血条画出来
void Sniper::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QBrush red_brush(QColor("#EE0000"));
    painter.setBrush(red_brush);
    float rate = 1.0 * _health / _max_health;
    painter.drawRect(10, 5, rate * 80, 5);
}

Guard::Guard(size_t health,
    int damage,
    size_t interval,
    LowerPlace* lower_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    size_t block)
    : Operator(health, damage, interval, lower_place, deployment_time, id, parent, block)
{
    setGeometry(_place->x() - 15, _place->y(), 100, 100);
}

//* 把近战单位的血条画出来
void Guard::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QBrush red_brush(QColor("#EE0000"));
    painter.setBrush(red_brush);
    float rate = 1.0 * _health / _max_health;
    painter.drawRect(10, 0, rate * 80, 5);
}

Irene::Irene(LowerPlace* lower_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent)
    : Guard(100, 10, 20, lower_place, deployment_time, id, parent, 3)
{
    _place->addOperator(this);
}

void Irene::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap Irene("://res/operator/Irene.png");
    painter.drawPixmap(0, 0, 100, 100, Irene);
    Guard::paintEvent(event);
}

Kroos::Kroos(HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent)
    : Sniper(100, 10, 20, higher_place, deployment_time, id, parent)
{
}

void Kroos::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap Kroos("://res/operator/Kroos.png");
    painter.drawPixmap(0, 0, 100, 100, Kroos);
    Sniper::paintEvent(event);
}
