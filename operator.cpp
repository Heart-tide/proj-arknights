#include "operator.h"
#include <QDebug>

Operator::Operator(size_t health, int damage, size_t interval,
    Place* place, size_t deployment_time, size_t id, QWidget* parent,
    size_t block)
    : Infected(health, damage, interval, place,
        deployment_time, id, parent)
    , _block(block)
{
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
        }
    }
}

void Operator::addTo(Place* place)
{
    _place = place;
    place->addOperator(this);
}

void Operator::removeFrom()
{
    _place->removeOperator();
    _place = nullptr;
}

TestOperator::TestOperator(Place* place, size_t deployment_time,
    size_t id, QWidget* parent)
    : Operator(100, 10, 5, place, deployment_time, id, parent, 3)
{
    _place->addOperator(this);
}
