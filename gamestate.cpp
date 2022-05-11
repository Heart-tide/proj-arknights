#include "gamestate.h"
#include <QDebug>
#include <QString>
#include <ctime>

GameState::GameState(size_t reunion_stats, size_t create_intervel, size_t total_hp, size_t default_speed, QWidget* parent)
    : QWidget(parent)
    , _map(new Map(this, this))
    , _timer(new QTimer(this))
    , _reunion_stats(reunion_stats)
    , _create_interval(create_intervel)
    , _reunion_dp(0)
    , _time(0)
    , _dp(0)
    , _hp(total_hp)
    , _speed(default_speed)
    , _is_paused(false)
{
    connect(_timer, &QTimer::timeout, this, &GameState::update);
    _timer->start(20 / _speed); //* 20ms 更新一次！

    resize(static_cast<QWidget*>(this->parent())->size()); //* 将自身大小改为父对象大小
    _map->resize(static_cast<QWidget*>(this->parent())->size());
}

//* update 函数，与 gamestate 计时器 connect
void GameState::update()
{
    // qDebug() << "[" << _time++ << "]";
    _time++;
    reunionStragegy();
    reunionAction();
    operatorAction();
}

//* 部署一个 Operator 对象
void GameState::deployOperator(size_t choice, Place* place)
{
    if (place->giveOperator() != nullptr) //* 一个格子最多只能有一个干员
        return;
    Operator* op = nullptr;
    static size_t id_counter = 0;
    switch (choice) {
    case 0:
        op = new Irene(dynamic_cast<LowerPlace*>(place), _time, id_counter++, this);
        break;
    case 1:
        op = new Kroos(dynamic_cast<HigherPlace*>(place), _time, id_counter++, this);
        break;
    }
    _active_operators.push_back(op);
    qDebug() << "\tCREATE" << qPrintable(op->giveName()) << "#"
             << op->giveId() << "|" << qPrintable(op->givePlace()->giveId());
}

//* 场上所有 active 的 operator 按其生成次序行动
void GameState::operatorAction()
{
    QVector<Operator*> still_active;
    for (auto it = _active_operators.begin(); it < _active_operators.end(); it++) {
        //! TODO 暂时干员只能攻击与自己同格子的单位
        if ((*it)->isActive()) {
            (*it)->action(_time, (*it)->givePlace()->giveReunions());
            still_active.push_back(*it);
        }
    }
    _active_operators = still_active;
}

//* 生成 Reunion
void GameState::reunionStragegy()
{
    if (_time % _create_interval != 1 || _reunion_stats == 0)
        return;
    _reunion_stats--;
    auto reunion = createRandomReunion();
    reunion->addTo(_map->_entrance);
    _active_reunions.push_back(reunion);
    qDebug() << "\tCREATE" << qPrintable(reunion->giveName()) << "#"
             << reunion->giveId() << "|" << qPrintable(reunion->givePlace()->giveId());
}

//* 场上所有 active 的 Reunion 按其生成次序行动
void GameState::reunionAction()
{
    QVector<Reunion*> still_active;
    for (auto it = _active_reunions.begin(); it < _active_reunions.end(); it++) {
        //! TODO 暂时敌军只能攻击与自己同格子的单位
        if ((*it)->isActive()) {
            (*it)->action(_time, _hp, (*it)->givePlace()->giveOperator());
            still_active.push_back(*it);
        }
    }
    _active_reunions = still_active;
}

//* 随机生成一个 Reunion 的对象，可以给不同类型以不同的生成概率
Reunion* GameState::createRandomReunion()
{
    static size_t id_counter = 0;
    srand(static_cast<unsigned>(clock()));
    switch (rand() % 1) {
    case 0:
        return new TestReunion(_map->_entrance, _time, id_counter++,
            _map->giveRandomRoute(), this);
    default:
        return nullptr;
    }
}

void deployOperator(GameState* gamestate, Place* place)
{
    static int cnt = 0;
    if (cnt % 2 == 0)
        gamestate->deployOperator(0, place);
    else
        gamestate->deployOperator(1, place);
    cnt++;
}
