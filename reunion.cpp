#include "reunion.h"

Reunion::Reunion(size_t health, int damage, size_t interval,
    Place* place, size_t deployment_time, size_t move_speed,
    size_t id, QVector<Place*> route, QWidget* parent,
    QMovie* idle_movie, QMovie* attack_movie)
    : Infected(health, damage, interval, place,
        deployment_time, id, parent, idle_movie, attack_movie)
    , _move_speed(move_speed)
    , _route(route)
{
    _route.pop_back();
    //? _move_speed 原为无符号整数，运算时出大问题！以后运算前先检查各操作数类型！！！
    direction.first = (_route.back()->x() - _place->x()) / 100 * _move_speed;
    direction.second = (_route.back()->y() - _place->y()) / 100 * _move_speed;
    setGeometry(_place->x() + 10, _place->y(), 90, 90);
    //? 要show才能调用绘图事件！！！
    show();
}

//* 移动！
void Reunion::move(size_t& hp)
{
    setGeometry(x() + direction.first, y() + direction.second, 90, 90);
    //* 判断是否到达下一个路径点，误差一个像素点，以便速度可以设为 3
    if (abs(x() - (_route.back()->x() + 10)) <= 1 && abs(y() - _route.back()->y()) <= 1) {
        removeFrom();
        addTo(_route.back());
        _route.pop_back();
        direction.first = (_route.back()->x() - _place->x()) / 100 * _move_speed;
        direction.second = (_route.back()->y() - _place->y()) / 100 * _move_speed;
        if (_place->isBase()) {
            printLog("#ff9933", "BASE", QString("HP %2 -> %3").arg(hp).arg(hp - 1));
            _is_active = false;
            if (--hp == 0) {
                throw LoseGameException();
            }
        }
    }
}

//* 攻击！
void Reunion::attack(Infected* op)
{
    auto op_place = op->givePlace(); //* 提前保存干员所在地块，不然一会受击死亡后就找不到了
    op->reduceHealth(_damage);
    if (!op->isActive()) {
        printLog("#ff9933", "KILL", QString("%1%2 --> %3%4").arg(giveName()).arg(_place->giveID()).arg(op->giveName()).arg(op_place->giveID()));
        op->hide();
    }
}

void Reunion::addTo(Place* place)
{
    _place = place;
    place->addReunion(this);
}

void Reunion::removeFrom()
{
    _place->removeReunion(this);
    _place = nullptr;
}

void Reunion::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    //* 画血条
    QBrush red_brush(QColor("#66CCFF"));
    painter.setBrush(red_brush);
    float rate = 1.0 * _health / _max_health;
    painter.drawRect(20, 10, rate * 80, 5);
    //* 说明血量
    painter.setPen(QPen(Qt::gray, 5));
    painter.drawText(20, 10, QString::number(_health) + " / " + QString::number(_max_health));
}

//* 将对整合运动的鼠标点击传递给其所在的地面
void Reunion::mousePressEvent(QMouseEvent* event)
{
    return _place->mousePressEvent(event);
}

void Reunion::mouseReleaseEvent(QMouseEvent* event)
{
    return _place->mouseReleaseEvent(event);
}

GroundReunion::GroundReunion(size_t health,
    int damage,
    size_t interval,
    Place* place,
    size_t deployment_time,
    size_t move_speed,
    size_t id,
    QVector<Place*> route,
    QWidget* parent,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Reunion(health, damage, interval, place, deployment_time, move_speed, id, route, parent, idle_movie, attack_movie)
{
}

void GroundReunion::action(size_t time, size_t& hp, Infected* op)
{
    //? 220428 因将==误写作=，在此debug半个钟头
    if (op) {
        _is_attacking = true;
        if (time - _last_action_time < _interval)
            return;
        _last_action_time = time;
        attack(op);
    } else {
        _is_attacking = false;
        move(hp);
    }
}

void GroundReunion::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    //* 画敌人
    if (_is_attacking) {
        _attack_movie->start();
        _idle_movie->stop();
        painter.drawPixmap(-35, -70, 200, 200, _attack_movie->currentPixmap());
    } else {
        _idle_movie->start();
        _attack_movie->stop();
        painter.drawPixmap(-35, -70, 200, 200, _idle_movie->currentPixmap());
    }
    Reunion::paintEvent(event);
}

UAV::UAV(size_t health,
    int damage,
    size_t interval,
    Place* place,
    size_t deployment_time,
    size_t move_speed,
    size_t id,
    QVector<Place*> route,
    QWidget* parent,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Reunion(health, damage, interval, place, deployment_time, move_speed, id, route, parent, idle_movie, attack_movie)
{
}

void UAV::action(size_t time, size_t& hp, Infected* op)
{
    if (op && time - _last_action_time >= _interval) {
        //* 攻击！
        _is_attacking = true;
        _last_action_time = time;
        attack(op);
        return;
    } else if (op == nullptr) {
        _is_attacking = false;
    }
    move(hp);
}

void UAV::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    //* 画敌人
    if (_is_attacking) {
        _attack_movie->start();
        _idle_movie->stop();
        painter.drawPixmap(-35, -105, 200, 200, _attack_movie->currentPixmap());
    } else {
        _idle_movie->start();
        _attack_movie->stop();
        painter.drawPixmap(-35, -105, 200, 200, _idle_movie->currentPixmap());
    }
    Reunion::paintEvent(event);
}

Yuan::Yuan(Place* place, size_t deployment_time,
    size_t id, QVector<Place*> route, QWidget* parent)
    : GroundReunion(50, 5, 30, place, deployment_time, 2, id, route, parent,
        new QMovie("://res/reunion/yuan-idle.gif"),
        new QMovie("://res/reunion/yuan-attack.gif")) //* 源石虫攻击力限时 UP !
{
}

Monster::Monster(Place* place, size_t deployment_time,
    size_t id, QVector<Place*> route, QWidget* parent)
    : UAV(25, 10, 50, place, deployment_time, 4, id, route, parent,
        new QMovie("://res/reunion/monster-idle.gif"),
        new QMovie("://res/reunion/monster-attack.gif"))
{
}
