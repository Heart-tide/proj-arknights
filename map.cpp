#include "map.h"
#include "ui_map.h"
#include <ctime>

Line::Line(size_t size)
{
    Place* places = new Place[size];
    for (size_t i = 0; i < size; i++) {
        addPlace(places + i);
    }
}

Line::Line(QVector<Place*> places)
    : _places(places)
{
    for (auto& place : _places) {
        place->setParent(this);
    }
}

void Line::addPlace(Place* place)
{
    place->setParent(this);
    _places.push_back(place);
}

Map::Map()
    : _base(new Base)
    , _entrance(new Entrance)
{
}

Map::Map(size_t height, size_t width)
    : Map()
{
    for (size_t i = 0; i < height; i++) {
        Line* line = new Line(width);
        addLine(line);
    }
}

Map::Map(QVector<Line*> lines)
    : Map()
{
    for (auto it = lines.begin(); it < lines.end(); it++) {
        addLine(*it);
    }
}

Map::~Map()
{
    delete ui;
}

void Map::addLine(Line* line)
{
    line->setParent(this);
    _lines.push_back(line);
}

void Map::loadMap()
{
    //* 一阶段测试地图，从右上到左下
    (*this)[0][giveWidth() - 1] = _entrance;
    (*this)[giveHeight() - 1][0] = _base;
    //* 赋予每个格子以id
    for (size_t i = 0; i < giveHeight(); i++) {
        for (size_t j = 0; j < giveWidth(); j++) {
            (*this)[i][j]->_id = QPair<size_t, size_t>(i, j);
        }
    }
}

void Map::loadRoutes()
{
    //* 一阶段测试路径，右上 --> 左上 --> 左下
    //* 存储时由终点向起点
    size_t width = giveWidth();
    size_t height = giveHeight();
    QVector<Place*> route;
    for (int i = height - 1; i != -1; i--) {
        route.push_back((*this)[i][0]);
    }
    for (size_t i = 1; i != width; i++) {
        route.push_back((*this)[0][i]);
    }
    _routes.push_back(route);
}

QVector<Place*> Map::giveRandomRoute() const
{
    srand(static_cast<unsigned>(clock()));
    auto i = rand() % _routes.size();
    return _routes[i];
}
