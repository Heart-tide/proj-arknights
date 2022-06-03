#include "operator.h"
#include "gamestate.h"
#include "reunion.h"
#include <QMouseEvent>

Operator::Operator(size_t health,
    int damage,
    int defense,
    size_t interval,
    Place* place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Infected(health, damage, defense, interval, place,
        deployment_time, id, parent, idle_movie, attack_movie)
    , _orientation(orientation)
    , _gamestate(place->getGameState())
    , _is_skill_used(false)
{
    //? 要show才能调用绘图事件！！！
    show();
    _place->addOperator(this);
}

void Operator::action(size_t time)
{
    if (_is_active == false || (time - _last_action_time) < _interval) {
        return;
    }
    Infected* attacked = findAttacked();
    if (attacked) {
        _is_attacking = true;
        _last_action_time = time;
        attack(attacked);
        if (!attacked->isActive()) {
            attacked->hide();
        }
    } else {
        _is_attacking = false;
    }
}

void Operator::attack(Infected* attacked)
{
    //* 类的函数不一定都是成员函数，定义在类外有时不失为一种选择
    attacked->reduceHealth(_damage);
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

//* 找到干员的攻击目标
Infected* Operator::findAttacked() const
{
    bool is_ground_to_air = isGroundToAir();
    for (auto place = _attack_places.cbegin(); place < _attack_places.cend(); place++) {
        for (auto reunion = (*place)->getReunions().cbegin(); reunion < (*place)->getReunions().cend(); reunion++) {
            //* 要不我方干员是可对空的，要不敌方是地面单位
            if (is_ground_to_air || (dynamic_cast<Reunion*>(*reunion)->isFlying() == false)) {
                return (*reunion);
            }
        }
    }
    return nullptr;
}

void Operator::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    //* 画干员
    QImage image;
    if (_is_attacking) {
        _attack_movie->start();
        _idle_movie->stop();
        image = _attack_movie->currentImage();
    } else {
        _idle_movie->start();
        _attack_movie->stop();
        image = _idle_movie->currentImage();
    }
    if (_is_attacked_count > 0) {
        convertToRedImage(image, _is_attacked_count);
        _is_attacked_count--;
    }
    painter.drawImage(QRect(-50, -70, 200, 200), image);
    //* 画血条
    QBrush red_brush(QColor("#EE0000"));
    painter.setBrush(red_brush);
    float rate = 1.0 * _health / _max_health;
    painter.drawRect(15, 5, rate * 80, 5);
    //* 技能发动显示
    if (_is_skill_used) {
        QBrush blue_brush(QColor("#0000EE"));
        painter.setBrush(blue_brush);
        painter.drawRect(90, 10, 5, 5);
    }
    //* 说明血量
    painter.setPen(QPen(Qt::green, 5));
    painter.drawText(15, 10, QString::number(_health) + " / " + QString::number(_max_health));
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

//* 展示我方干员的状态，或撤退我方干员
void Operator::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        printLog("#cc9900", QString("%0").arg(getName()), QString("{ATT}%1 {DEF}%2 {BLK}%3").arg(_damage).arg(_defense).arg(getBlock()));
        printLog("#000000", QString("{SKILL}"), QString("%1").arg(getSkill()));
        for (Place*& place : _attack_places) {
            place->paintAttackArea();
        }
    } else if (event->button() == Qt::RightButton && !_gamestate->isGameOver() && !_gamestate->isPaused()) {
        _gamestate->getDeploymentPoint() += getCost() / 2;
        removeFrom();
        _is_active = false;
        delete _idle_movie;
        delete _attack_movie;
    }
}

void Operator::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        for (Place*& place : _attack_places) {
            place->paintAttackArea();
        }
    }
}

Sniper::Sniper(size_t health,
    int damage,
    int defense,
    size_t interval,
    HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Operator(health, damage, defense, interval, higher_place, deployment_time,
        id, parent, orientation, idle_movie, attack_movie)
{
    setGeometry(_place->x() - 5, _place->y() - 8, 100, 100);
}

Guard::Guard(size_t health,
    int damage,
    int defense,
    size_t interval,
    LowerPlace* lower_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Operator(health, damage, defense, interval, lower_place, deployment_time,
        id, parent, orientation, idle_movie, attack_movie)
{
    setGeometry(_place->x() - 15, _place->y(), 100, 100);
}

Doctor::Doctor(size_t health,
    int damage,
    int defense,
    size_t interval,
    HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Operator(health, damage, defense, interval, higher_place, deployment_time,
        id, parent, orientation, idle_movie, attack_movie)
{
    setGeometry(_place->x() - 5, _place->y() - 8, 100, 100);
}

Infected* Doctor::findAttacked() const
{
    QVector<Infected*> injured_operators;
    for (auto place = _attack_places.cbegin(); place < _attack_places.cend(); place++) {
        Infected* op = (*place)->getOperator();
        if (op != nullptr && op->isHealthReduced()) {
            injured_operators.push_back(op);
        }
    }
    return injured_operators.empty() ? nullptr : getRandomUnit(injured_operators); //* 注意不能除零
}

void Doctor::attack(Infected* attacked)
{
    attacked->increaseHealth(_damage);
}

HoneyBerry::HoneyBerry(HigherPlace* higher_place, size_t deployment_time,
    size_t id, QWidget* parent, Orientation orientation)
    : Doctor(125, 30, 9, 143, higher_place, deployment_time, id, parent, orientation,
        new QMovie("://res/operator/HoneyBerry-idle.gif"),
        new QMovie("://res/operator/HoneyBerry-attack.gif"))
{
}

void HoneyBerry::action(size_t time)
{
    if (_is_active == false || (time - _last_action_time) < _interval) {
        return;
    }
    Infected* attacked = findAttacked();
    srand(static_cast<unsigned>(clock()));
    if (attacked) {
        _is_attacking = true;
        _last_action_time = time;
        attack(attacked);
        if (rand() % 5 == 0) {
            Infected* attacked = findAttacked();
            if (attacked) {
                attack(attacked);
            }
            _is_skill_used = true;
        } else {
            _is_skill_used = false;
        }
    } else {
        _is_attacking = false;
    }
}

Irene::Irene(LowerPlace* lower_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation)
    : Guard(220, 48, 28, 65, lower_place, deployment_time, id, parent, orientation,
        new QMovie("://res/operator/Irene-idle.gif"),
        new QMovie("://res/operator/Irene-attack.gif"))
{
}

void Irene::attack(Infected* attacked)
{
    srand(static_cast<unsigned>(clock()));
    int half_of_attacked_defense = attacked->getDefense() / 2;
    bool use_skill = false;
    if (rand() % 2 == 0) {
        attacked->reduceHealth(_damage + half_of_attacked_defense);
        use_skill = true;
    } else {
        attacked->reduceHealth(_damage);
    }
    if (attacked->isActive()) {
        if (rand() % 2 == 0) {
            attacked->reduceHealth(_damage + half_of_attacked_defense);
            use_skill = true;
        } else {
            attacked->reduceHealth(_damage);
        }
    }
    _is_skill_used = use_skill;
}

Skadi::Skadi(LowerPlace* lower_place, size_t deployment_time,
    size_t id, QWidget* parent, Orientation orientation)
    : Guard(294, 168, 22, 75, lower_place, deployment_time, id, parent, orientation,
        new QMovie("://res/operator/Skadi-idle.gif"),
        new QMovie("://res/operator/Skadi-attack.gif"))
    , _attack_doubled(true)
{
    _is_skill_used = true;
}

void Skadi::action(size_t time)
{
    if (_attack_doubled && time - _deployment_time > 1000) {
        _attack_doubled = false;
        _damage /= 2;
        _is_skill_used = false;
    }
    Operator::action(time);
}

Kroos::Kroos(HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation)
    : Sniper(125, 41, 15, 50, higher_place, deployment_time, id, parent, orientation,
        new QMovie("://res/operator/Kroos-idle.gif"),
        new QMovie("://res/operator/Kroos-attack.gif"))
{
}

void Kroos::attack(Infected* attacked)
{
    srand(static_cast<unsigned>(clock()));
    attacked->reduceHealth(_damage);
    if (rand() % 3 == 0) {
        if (attacked->isActive()) {
            attacked->reduceHealth(_damage);
        }
        _is_skill_used = true;
    } else {
        _is_skill_used = false;
    }
}
