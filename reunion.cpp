#include "reunion.h"
#include <cmath>

Reunion::Reunion(size_t health, int damage, int defense, size_t interval,
    size_t deployment_time, float move_speed,
    size_t id, QVector<Place*> route, QWidget* parent,
    QMovie* idle_movie, QMovie* attack_movie)
    : Infected(health, damage, defense, interval, route.back(),
        deployment_time, id, parent, idle_movie, attack_movie)
    , _move_speed(move_speed)
    , _route(route)
{
    addTo(_route.back());
    _route.pop_back();
    //? _move_speed 原为无符号整数，运算时出大问题！以后运算前先检查各操作数类型！！！
    chooseDirection();
    setGeometry(_place->x() + 10, _place->y(), 90, 90);
    //? 要show才能调用绘图事件！！！
    show();
}

//* 移动！
void Reunion::move(size_t& hp, Map* map)
{
    //* 将移动的余数补全，并进行一次移动
    _remaining_direction[0] += _remaining_direction[1];
    int next_x = x() + _direction[0].x();
    int next_y = y() + _direction[0].y();
    if (_remaining_direction[0].x() > 1) {
        next_x += _direction[1].x();
        _remaining_direction[0].setX(_remaining_direction[0].x() - 1);
    }
    if (_remaining_direction[0].y() > 1) {
        next_y += _direction[1].y();
        //? 220515将setY写作setY，在此debug半个小时
        _remaining_direction[0].setY(_remaining_direction[0].y() - 1);
    }
    setGeometry(next_x, next_y, 90, 90);
    //* 每次移动都寻找临近格子并与下一路径点作比较
    Place* adjacent_place = adjacentPlace(map);
    int delta_x = abs(x() - (adjacent_place->x() + 10));
    int delta_y = abs(y() - adjacent_place->y());
    //* 误差 30 个像素，寻找临近格子作归属
    if (_place != adjacent_place && delta_x <= 30 && delta_y <= 30) {
        removeFrom();
        addTo(adjacent_place);
    } else if (delta_x <= 2 && delta_y <= 2) {
        if (adjacent_place == _route.back()) {
            _route.pop_back();
            chooseDirection();
        }
        if (_route.empty()) {
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

Place* Reunion::adjacentPlace(Map* map)
{
    auto adjacentPlaceCoordinate = [=](int xy) {
        if (xy % 100 >= 50) {
            xy += 100;
        }
        return xy / 100;
    };
    int place_j = adjacentPlaceCoordinate(x());
    int place_i = adjacentPlaceCoordinate(y());
    return (*map)[place_i][place_j];
}

void Reunion::chooseDirection()
{
    if (_route.empty()) {
        return;
    }
    QVector2D delta_distance(_route.back()->pos() - _place->pos());
    QVector2D direction_float(delta_distance.normalized() * _move_speed);
    //* 将 float 型隐式转化为 int 型，向 0 取整
    _direction[0].setX(direction_float.x());
    _direction[0].setY(direction_float.y());
    _direction[1].setX(direction_float.x() > 0 ? 1 : -1);
    _direction[1].setY(direction_float.y() > 0 ? 1 : -1);
    //* 这里存储移动方向余数的绝对值
    _remaining_direction[0] = _remaining_direction[1]
        = QVector2D(abs(direction_float.x() - _direction[0].x()), abs(direction_float.y() - _direction[0].y()));
}

GroundReunion::GroundReunion(size_t health,
    int damage,
    int defense,
    size_t interval,
    size_t deployment_time,
    float move_speed,
    size_t id,
    QVector<Place*> route,
    QWidget* parent,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Reunion(health, damage, defense, interval, deployment_time, move_speed, id, route, parent, idle_movie, attack_movie)
{
}

void GroundReunion::action(size_t time, size_t& hp, Infected* op, Map* map)
{
    //? 220428 因将==误写作=，在此debug半个钟头
    //* 干员会对阻挡最后所接敌人进行阻挡
    if (op && op->giveBlock() >= op->givePlace()->giveReunions().size()) {
        _is_attacking = true;
        if (time - _last_action_time < _interval)
            return;
        _last_action_time = time;
        attack(op);
    } else {
        _is_attacking = false;
        move(hp, map);
    }
}

void GroundReunion::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    //* 画地面敌人
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
    int defense,
    size_t interval,
    size_t deployment_time,
    float move_speed,
    size_t id,
    QVector<Place*> route,
    QWidget* parent,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Reunion(health, damage, defense, interval, deployment_time, move_speed, id, route, parent, idle_movie, attack_movie)
{
}

void UAV::action(size_t time, size_t& hp, Infected* op, Map* map)
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
    move(hp, map);
}

void UAV::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    //* 画无人机
    if (_is_attacking) {
        _attack_movie->start();
        _idle_movie->stop();
        painter.drawPixmap(-50, -100, 200, 200, _attack_movie->currentPixmap());
    } else {
        _idle_movie->start();
        _attack_movie->stop();
        painter.drawPixmap(-50, -100, 200, 200, _idle_movie->currentPixmap());
    }
    Reunion::paintEvent(event);
}

Yuan::Yuan(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent)
    : GroundReunion(55, 13, 0, 85, deployment_time, 2, id, route, parent,
        new QMovie("://res/reunion/yuan-idle.gif"),
        new QMovie("://res/reunion/yuan-attack.gif")) //* 源石虫攻击力限时 UP 已结束 !
{
}

Soldier::Soldier(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent)
    : GroundReunion(140, 24, 10, 120, deployment_time, 1.8, id, route, parent,
        new QMovie("://res/reunion/soldier-idle.gif"),
        new QMovie("://res/reunion/soldier-attack.gif"))
{
}

Revenger::Revenger(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent)
    : GroundReunion(500, 30, 20, 115, deployment_time, 1.3, id, route, parent,
        new QMovie("://res/reunion/revenger-idle.gif"),
        new QMovie("://res/reunion/revenger-attack.gif"))
    , _low_health(false)
{
}

void Revenger::attack(Infected* op)
{
    if (_low_health == false && _health <= 0.5 * _max_health) {
        _low_health = true;
        _damage *= 2;
    }
    Reunion::attack(op);
}

Monster::Monster(size_t deployment_time, size_t id, QVector<Place*> route, QWidget* parent)
    : UAV(155, 22, 5, 150, deployment_time, 1.8, id, route, parent,
        new QMovie("://res/reunion/monster-idle.gif"),
        new QMovie("://res/reunion/monster-attack.gif"))
{
}
