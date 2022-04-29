#include "gamestate.h"
#include <QDebug>
#include <QString>
#include <ctime>

GameState::GameState(size_t reunion_stats, size_t create_intervel, size_t total_hp, size_t default_speed, QWidget* parent)
    : _map(new Map(3, 4))
    , _timer(new QTimer(this))
    , _reunion_stats(reunion_stats)
    , _create_interval(create_intervel)
    , _reunion_dp(0)
    , _time(0)
    , _dp(0)
    , _hp(total_hp)
    , _speed(1)
    , _is_paused(false)
{
    _map->setParent(this);
    configure();
    connect(_timer, &QTimer::timeout, this, &GameState::update);
    _timer->start(1000 / _speed);
}

//* 游戏初始化
void GameState::configure()
{
    _map->loadMap();
    _map->loadRoutes();
    qDebug() << _map->giveWidth() << "*" << _map->giveHeight() << "大小的地图已生成:\n";
    for (size_t i = 0; i < _map->giveHeight(); i++) {
        QString line("\t");
        for (size_t j = 0; j < _map->giveWidth(); j++) {
            if ((*_map)[i][j]->isBase())
                line += "B  ";
            else if ((*_map)[i][j]->isEntrance())
                line += "E  ";
            else
                line += "*  ";
        }
        qDebug() << qPrintable(line) << "\n";
    }
    qDebug() << "游戏开始了哦!\n";
}

//* update 函数，与 gamestate 计时器 connect
void GameState::update()
{
    qDebug() << "[" << _time++ << "]";
    reunionStragegy();
    reunionAction();
    operatorAction();
}

//* 部署一个 Operator 对象
void GameState::deployOperator(size_t choice, size_t x, size_t y)
{
    if ((*_map)[x][y]->giveOperator() != nullptr) //* 一个格子最多只能有一个干员
        return;
    Operator* op = nullptr;
    static size_t id_counter = 0;
    switch (choice) {
    case 0:
        op = new TestOperator((*_map)[x][y], _time, id_counter++, this);
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
    }
}
