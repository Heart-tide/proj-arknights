#include "operator.h"

Operator::Operator(size_t health,
    int damage,
    size_t interval,
    Place* place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    size_t block,
    Orientation orientation,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Infected(health, damage, interval, place,
        deployment_time, id, parent, idle_movie, attack_movie)
    , _block(block)
    , _orientation(orientation)
{
    //? 要show才能调用绘图事件！！！
    show();
    _place->addOperator(this);
}

void Operator::action(size_t time, Infected* attacked)
{
    if (_is_active == false || (time - _last_action_time) < _interval) {
        return;
    }
    if (attacked) { //* 暂时无视阻挡数要求
        _is_attacking = true;
        _last_action_time = time;
        auto attacked_place = attacked->givePlace();
        //* 类的函数不一定都是成员函数，定义在类外有时不失为一种选择
        attacked->reduceHealth(_damage);
        if (!attacked->isActive()) {
            // C++ 11 的 Raw String Literals，有助于更方便地书写字符串字面量：R"(……)"，中间可加换行
            printLog("#ee0000", "KILL", QString("%1%2 --> %3%4").arg(giveName()).arg(_place->giveID()).arg(attacked->giveName()).arg(attacked_place->giveID()));
            attacked->hide();
        }
    } else {
        _is_attacking = false;
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
    //* 画干员
    if (_is_attacking) {
        _attack_movie->start();
        _idle_movie->stop();
        painter.drawPixmap(-50, -70, 200, 200, _attack_movie->currentPixmap());
    } else {
        _idle_movie->start();
        _attack_movie->stop();
        painter.drawPixmap(-50, -70, 200, 200, _idle_movie->currentPixmap());
    }
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
    Orientation orientation,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Operator(health, damage, interval, higher_place, deployment_time,
        id, parent, 0, orientation, idle_movie, attack_movie)
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
    Orientation orientation,
    QMovie* idle_movie,
    QMovie* attack_movie)
    : Operator(health, damage, interval, lower_place, deployment_time,
        id, parent, block, orientation, idle_movie, attack_movie)
{
    setGeometry(_place->x() - 15, _place->y(), 100, 100);
}

Irene::Irene(LowerPlace* lower_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation)
    : Guard(70, 10, 30, lower_place, deployment_time, id, parent, 3, orientation,
        new QMovie("://res/operator/Irene-idle.gif"),
        new QMovie("://res/operator/Irene-attack.gif"))
{
}

Kroos::Kroos(HigherPlace* higher_place,
    size_t deployment_time,
    size_t id,
    QWidget* parent,
    Orientation orientation)
    : Sniper(40, 7, 20, higher_place, deployment_time, id, parent, orientation,
        new QMovie("://res/operator/Kroos-idle.gif"),
        new QMovie("://res/operator/Kroos-attack.gif"))
{
}
