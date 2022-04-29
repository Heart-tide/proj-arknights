#ifndef PLACE_H
#define PLACE_H

#include <QPair>
#include <QString>
#include <QVector>
#include <QWidget>

class Map;
class Infected;
class Operator;
class Reunion;

//* 一个格子，很普通
namespace Ui {
class Place;
}

class Place : public QWidget {
    Q_OBJECT

public:
    friend class Map;

    Place();
    virtual ~Place();

    virtual void addOperator(Infected* op);
    virtual void addReunion(Infected* reunion) { _reunions.push_back(reunion); }
    virtual void removeOperator() { _op = nullptr; }
    virtual void removeReunion(Infected* reunion);

    virtual bool isBase() { return false; }
    virtual bool isEntrance() { return false; }

    Infected* giveOperator() { return _op; }
    QVector<Infected*>& giveReunions() { return _reunions; }
    QString giveId() const;

protected:
    Infected* _op;
    QVector<Infected*> _reunions;
    QPair<size_t, size_t> _id;

private:
    Ui::Place* ui;
};

//************* Place 有三个子类：不可到达的地块、低地、高地 *************

//* 这里是不可到达地块

class UnreachablePlace : public Place {
public:
    void addOperator(Infected* op) override { }
    void addReunion(Infected* reunion) override { }
    void removeOperator() override { }
    void removeReunion(Infected* reunion) override { }
};
class Base : public Place {
    Q_OBJECT

public:
    bool isBase() override { return true; }
};

class Entrance : public Place {
    Q_OBJECT

public:
    bool isEntrance() override { return true; }
};

#endif // PLACE_H
