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
    virtual void removeOperator(Operator* op) { _op = nullptr; }
    virtual void removeReunion(Reunion* reunion);

    virtual bool is_base() { return false; }
    virtual bool is_entrance() { return false; }
    QString giveId() const;

protected:
    Operator* _op;
    vector<Reunion*> _reunions;
    pair<size_t, size_t> _id;

private:
    Ui::Place* ui;
};

//************* Place有三个子类：不可到达的地块、低地、高地 *************

class Base : public Place {
    Q_OBJECT

public:
    Base() = default;
    ~Base() = default;

    bool is_base() override { return true; }
};

class Entrance : public Place {
    Q_OBJECT

public:
    Entrance() = default;
    ~Entrance() = default;

    bool is_entrance() override { return true; }

private:
};

#endif // PLACE_H
