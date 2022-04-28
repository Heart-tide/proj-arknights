#ifndef MAP_H
#define MAP_H

#include "place.h"
#include <QWidget>
#include <vector>
using std::vector;

class GameState;

//* 一行，由很多格子组成
class Line : public QWidget {
    Q_OBJECT

public:
    Line() = default;
    explicit Line(size_t size);
    explicit Line(vector<Place*> places);
    ~Line() = default;

    Place*& operator[](size_t i) { return _places[i]; }
    const Place* operator[](size_t i) const { return _places[i]; }

    void addPlace(Place* place);
    size_t giveSize() const { return _places.size(); }

private:
    vector<Place*> _places;
};

//* 一张地图，由很多行组成
namespace Ui {
class Map;
}

class Map : public QWidget {
    Q_OBJECT

public:
    friend class GameState;

    Map();
    Map(size_t height, size_t width);
    Map(vector<Line*> lines);
    ~Map();

    Line& operator[](size_t i) { return *_lines[i]; }
    const Line& operator[](size_t i) const { return *_lines[i]; }

    void addLine(Line* line);
    void loadMap();
    void loadRoutes(); //* 只按坐标保存路径，载入时换算成Place*链表，并存入entrance中

    size_t giveHeight() const { return _lines.size(); }
    size_t giveWidth() const { return _lines.empty() ? 0 : _lines[0]->giveSize(); }
    vector<Place*> giveRandomRoute() const;

private:
    Ui::Map* ui;

    vector<Line*> _lines;
    Base* _base;
    Entrance* _entrance;
    vector<vector<Place*>> _routes;
};

#endif // MAP_H
