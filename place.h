#ifndef PLACE_H
#define PLACE_H

#include <QString>
#include <QWidget>
#include <map>
#include <vector>
using std::pair;
using std::vector;

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

    virtual void addOperator(Operator* op) { _op = op; }
    virtual void addReunion(Reunion* reunion) { _reunions.push_back(reunion); }
    virtual void removeOperator() { _op = nullptr; }
    virtual void removeReunion(Reunion* reunion);

    virtual bool isBase() { return false; }
    virtual bool isEntrance() { return false; }

    Operator* giveOperator() { return _op; }
    vector<Reunion*>& giveReunions() { return _reunions; }
    QString giveId() const;

protected:
    Operator* _op;
    vector<Reunion*> _reunions;
    pair<size_t, size_t> _id;

private:
    Ui::Place* ui;
};

//************* Place有三个子类：不可到达的地块、低地、高地 *************

//* 这里是不可到达地块

class UnreachablePlace : public Place {
public:
    void addOperator(Operator* op) override { }
    void addReunion(Reunion* reunion) override { }
    void removeOperator() override { }
    void removeReunion(Reunion* reunion) override { }
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
