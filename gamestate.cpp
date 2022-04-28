#include "gamestate.h"
#include <QDebug>
#include <QString>
#include <ctime>

GameState::GameState(size_t create_intervel, size_t total_hp,
    size_t default_speed, QWidget* parent)
    : _map(new Map(3, 4))
    , _timer(new QTimer(this))
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

void GameState::update()
{
    if (_time == 31) { //* 先跑 30 ticks
        _timer->stop();
        return;
    }
    qDebug() << "[" << _time << "]";
    _time++;
    reunionStragegy();
    reunionActions();
    operatorActions();
}

void GameState::deployOperator(size_t choice, size_t x, size_t y)
{
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

void GameState::operatorActions()
{
    vector<Operator*> still_active;
    for (auto it = _active_operators.begin();
         it < _active_operators.end(); it++) {
        if ((*it)->isActive()) {
            vector<Infected*> attackeds; //* 暂时干员只能攻击与自己同格子的单位
            for (size_t i = 0; i < (*it)->givePlace()->giveReunions().size(); i++)
                attackeds.push_back((*it)->givePlace()->giveReunions()[i]);
            (*it)->action(_time, attackeds);
            still_active.push_back(*it);
        }
    }
    _active_operators = still_active;
}

void GameState::reunionStragegy()
{
    if (_time % _create_interval != 1)
        return;
    static size_t count = 0;
    if (count >= 2) //* 出俩就得了!!!
        return;
    count++;
    auto reunion = giveRandomReunion();
    reunion->addTo(_map->_entrance);
    _active_reunions.push_back(reunion);
    qDebug() << "\tCREATE" << qPrintable(reunion->giveName()) << "#"
             << reunion->giveId() << "|" << qPrintable(reunion->givePlace()->giveId());
}

void GameState::reunionActions()
{
    vector<Reunion*> still_active;
    for (auto it = _active_reunions.begin();
         it < _active_reunions.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->action(_time, _hp, (*it)->givePlace()->giveOperator());
            still_active.push_back(*it);
        }
    }
    _active_reunions = still_active;
}

Reunion* GameState::giveRandomReunion()
{
    static size_t id_counter = 0;
    srand(static_cast<unsigned>(clock()));
    switch (rand() % 1) {
    case 0:
        return new TestReunion(_map->_entrance, _time, id_counter++,
            _map->giveRandomRoute(), this);
    }
}
