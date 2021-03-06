#ifndef PLACE_H
#define PLACE_H

#include "commondef.h"
#include <QPair>
#include <QPoint>
#include <QVector>
#include <QWidget>

class GameState;
class Map;
class Infected;
class Operator;
class Reunion;

//* 一个格子，很普通

class Place : public QWidget {
    Q_OBJECT

public:
    friend class Map;

    Place(GameState* gamestate, QWidget* parent);

    virtual void addOperator(Infected* op);
    virtual void addReunion(Infected* reunion) { _reunions.push_back(reunion); }
    virtual void removeOperator() { _op = nullptr; }
    virtual void removeReunion(Infected* reunion);

    virtual bool isBase() { return false; }
    virtual bool isEntrance() { return false; }
    virtual bool isLower() { return false; }
    virtual bool isHigher() { return false; }
    GameState* getGameState() { return _gamestate; }
    Infected* getOperator() { return _op; }
    QVector<Infected*>& getReunions() { return _reunions; }

    QString getID() const; //* 返回 QString 数据
    QPair<size_t, size_t> showID() const { return _id; } //* 返回 id 本身

    void paintAttackArea();

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

protected:
    GameState* _gamestate;
    Infected* _op;
    QVector<Infected*> _reunions;
    QPair<size_t, size_t> _id;
    QPoint _prees_point;
    bool _isPainted;
};

//************* Place 有三个子类：不可到达的地块、低地、高地 *************

//* 这里是特殊地块

//* 特殊地块，编号 4
class SpecialPlace : public Place {
    Q_OBJECT

public:
    SpecialPlace(GameState* gamestate, QWidget* parent);

    void addOperator(Infected*) override { }
    void addReunion(Infected*) override { }
    void removeOperator() override { }
    void removeReunion(Infected*) override { }
};
//* 我方基地，编号 0
class Base : public SpecialPlace {
    Q_OBJECT

public:
    Base(GameState* gamestate, QWidget* parent);
    bool isBase() override { return true; }
};

//* 敌方入口，编号 1
class Entrance : public SpecialPlace {
    Q_OBJECT

public:
    Entrance(GameState* gamestate, QWidget* parent);
    bool isEntrance() override { return true; }
};

//* 这里是低地和高地

//* 低地，编号 2
class LowerPlace : public Place {
    Q_OBJECT

public:
    LowerPlace(GameState* gamestate, QWidget* parent);
    bool isLower() override { return true; }
};

//* 高地，编号 3
class HigherPlace : public Place {
    Q_OBJECT

public:
    HigherPlace(GameState* gamestate, QWidget* parent);
    bool isHigher() override { return true; }
};

#endif // PLACE_H
