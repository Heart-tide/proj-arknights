#include "gamestate.h"
#include "ui_map.h"
#include <QDebug>
#include <QDialog>
#include <QString>
#include <ctime>

GameState::GameState(size_t reunion_stats, size_t create_intervel, size_t total_hp, size_t default_speed, QWidget* parent)
    : QWidget(parent)
    , _map(new Map(this, this))
    , _timer(new QTimer(this))
    , _enemy_stats(reunion_stats)
    , _create_interval(create_intervel)
    , _reunion_dp(0)
    , _time(0)
    , _dp(0)
    , _hp(total_hp)
    , _speed(default_speed)
    , _gameover(false)
{
    connectWithWidgets();

    connect(_timer, &QTimer::timeout, this, &GameState::update);
    _timer->start(20 / _speed); //* 20ms 更新一次！

    resize(static_cast<QWidget*>(this->parent())->size()); //* 将自身大小改为父对象大小
    _map->resize(static_cast<QWidget*>(this->parent())->size());
}

//* 与 ui 中所有的控件 connect
void GameState::connectWithWidgets()
{
    auto ui = _map->ui;
    connect(ui->push_speed, &QPushButton::clicked, this, [=]() {
        _speed = 3 - _speed;
        _timer->setInterval(20 / _speed);
        ui->push_speed->setText(QString("Speed %1x").arg(_speed));
    });
    connect(ui->push_pause, &QPushButton::clicked, this, [=]() {
        if (_timer->isActive()) {
            _timer->stop();
            ui->push_pause->setText("Resume");
        } else if (!_timer->isActive() && !_gameover) {
            _timer->start();
            ui->push_pause->setText("Pause");
        }
    });
}

//* update 函数，与 gamestate 计时器 connect
void GameState::update()
{
    // qDebug() << "[" << _time++ << "]";
    _time++;
    _dp += (_time % 50 == 0); //* 每 50 帧增加一费
    try {
        reunionStragegy();
        reunionAction();
        operatorAction();
    } catch (WinException& e) {
        _timer->stop();
        _gameover = true;
        //* C++ 11 的 Raw String Literals，有助于更方便地书写字符串字面量：R"(……)"，中间可加换行
        _map->ui->label_state->setText(R"(<html><head /><body><p align = "center">
            <span style = " font-size:16pt; font-weight:600;">YOU WIN</ span></ p></ body></ html>)");
    } catch (LoseException& e) {
        _timer->stop();
        _gameover = true;
        _map->ui->label_state->setText(R"(<html><head/><body><p align="center">
            <span style=" font-size:16pt; font-weight:600;">YOU LOSE</span></p></body></html>)");
    }
    _map->ui->label_enemy_stats->setText(QString::number(_enemy_stats + _active_reunions.size()));
    _map->ui->label_hp->setText(QString::number(_hp));
    _map->ui->label_dp->setText(QString::number(_dp));
}

//* 部署一个 Operator 对象
void GameState::deployOperator(size_t choice, Place* place)
{
    if (place->giveOperator() != nullptr) //* 一个格子最多只能有一个干员
        return;
    Operator* op = nullptr;
    Orientation orientation = static_cast<Orientation>(_map->whichOrientation());
    static size_t id_counter = 0;
    switch (choice) {
    case 0:
        if (Irene::cost <= _dp) {
            op = new Irene(dynamic_cast<LowerPlace*>(place), _time, id_counter++, this, orientation);
        }
        break;
    case 1:
        if (Kroos::cost <= _dp) {
            op = new Kroos(dynamic_cast<HigherPlace*>(place), _time, id_counter++, this, orientation);
        }
        break;
    }
    if (op == nullptr) {
        return;
    }
    _dp -= op->giveCost();
    _active_operators.push_back(op);
    qDebug() << "\tCREATE" << qPrintable(op->giveName()) << "#"
             << op->giveID() << "|" << qPrintable(op->givePlace()->giveID());
}

//* 场上所有 active 的 operator 按其生成次序行动
void GameState::operatorAction()
{
    QVector<Operator*> still_active;
    for (auto it = _active_operators.begin(); it < _active_operators.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->action(_time, properAttackedReunion(*it));
            still_active.push_back(*it);
        }
    }
    _active_operators = still_active;
}

//* 计算干员可攻击到的敌人
Infected* GameState::properAttackedReunion(Operator* op) const
{
    int x = 0, y = 0, height = 0, width = 0;
    switch (op->giveOrientation()) {
    case UP:
        height = op->giveAttackArea().second;
        width = op->giveAttackArea().first;
        x = op->givePlace()->showID().second - width / 2;
        y = op->givePlace()->showID().first - (height - 1);
        break;
    case DOWN:
        height = op->giveAttackArea().second;
        width = op->giveAttackArea().first;
        x = op->givePlace()->showID().second - width / 2;
        y = op->givePlace()->showID().first;
        break;
    case LEFT:
        height = op->giveAttackArea().first;
        width = op->giveAttackArea().second;
        x = op->givePlace()->showID().second - (width - 1);
        y = op->givePlace()->showID().first - height / 2;
        break;
    case RIGHT:
        height = op->giveAttackArea().first;
        width = op->giveAttackArea().second;
        x = op->givePlace()->showID().second;
        y = op->givePlace()->showID().first - height / 2;
        break;
    }
    //* 先列后行进行遍历
    //? 从 先行后列 到 先x后y 的转换，需要多加注意！以后都应写成 先x后y，更通用。
    for (int i = x; i < x + width; i++) {
        if (i < 0 || i >= _map->giveWidth()) {
            continue;
        }
        for (int j = y; j < y + height; j++) {
            if (j < 0 || j >= _map->giveHeight()) {
                continue;
            }
            if (!(*_map)[j][i]->giveReunions().empty()) {
                return (*_map)[j][i]->giveReunions()[0];
            }
        }
    }
    return nullptr;
}

//* 生成 Reunion
void GameState::reunionStragegy()
{
    if (_time % _create_interval != 1 || _enemy_stats == 0)
        return;
    _enemy_stats--;
    auto reunion = createRandomReunion();
    reunion->addTo(_map->_entrance);
    _active_reunions.push_back(reunion);
    qDebug() << "\tCREATE" << qPrintable(reunion->giveName()) << "#"
             << reunion->giveID() << "|" << qPrintable(reunion->givePlace()->giveID());
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
    if (still_active.empty() && _enemy_stats == 0) {
        throw WinException();
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

//* 当点击一个格子时调用，若符合条件，则放置一个干员
void deployOperator(GameState* gamestate, Place* place)
{
    int choice = gamestate->_map->whichOperator();
    switch (choice) {
    case -1:
        return;
    case 0:
        if (place->isLower()) {
            gamestate->deployOperator(choice, place);
        }
        break;
    case 1:
        if (place->isHigher()) {
            gamestate->deployOperator(choice, place);
        }
        break;
    }
}
