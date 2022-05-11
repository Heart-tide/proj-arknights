#ifndef PLACE_H
#define PLACE_H

#include <QPair>
#include <QString>
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

    Infected* giveOperator() { return _op; }
    QVector<Infected*>& giveReunions() { return _reunions; }
    QString giveId() const;

    void mousePressEvent(QMouseEvent* event) override;

protected:
    GameState* _gamestate;
    Infected* _op;
    QVector<Infected*> _reunions;
    QPair<size_t, size_t> _id;
};

//************* Place 有三个子类：不可到达的地块、低地、高地 *************

//* 这里是不可到达地块

class UnreachablePlace : public Place {
public:
    UnreachablePlace(GameState* gamestate, QWidget* parent);

    void addOperator(Infected*) override { }
    void addReunion(Infected*) override { }
    void removeOperator() override { }
    void removeReunion(Infected*) override { }
};
class Base : public UnreachablePlace {
    Q_OBJECT

public:
    Base(GameState* gamestate, QWidget* parent);
    bool isBase() override { return true; }
};

class Entrance : public UnreachablePlace {
    Q_OBJECT

public:
    Entrance(GameState* gamestate, QWidget* parent);
    bool isEntrance() override { return true; }
};

#endif // PLACE_H
