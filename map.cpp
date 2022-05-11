#include "map.h"
#include "ui_map.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <ctime>

Map::Map(size_t height, size_t width, GameState* gamestate, QWidget* parent)
    : QWidget(parent)
    , _gamestate(gamestate)

{
    loadMap(height, width);
    loadRoutes();
    qDebug() << giveWidth() << "*" << giveHeight() << "大小的地图已生成";
    qDebug() << "游戏开始了哦!\n";
}

Map::~Map()
{
    delete ui;
}

void Map::loadMap(size_t height, size_t width)
{
    for (size_t i = 0; i < height; i++) {
        QVector<Place*> line;
        for (size_t j = 0; j < width; j++) {
            Place* place = nullptr;
            if (i == 0 && j == width - 1) {
                _entrance = new Entrance(_gamestate, this);
                place = _entrance;
            } else if (i == height - 1 && j == 0) {
                _base = new Base(_gamestate, this);
                place = _base;
            } else {
                place = new Place(_gamestate, this);
            }
            line.push_back(place);
            place->setGeometry(j * 100 + 5, i * 100 + 5, 100, 100);
            place->_id = QPair<size_t, size_t>(i, j);
        }
        _lines.push_back(line);
    }
}

void Map::loadRoutes()
{
    //* 测试路径一，右上 --> 左下
    //* 存储时由终点向起点
    //* 随机生成 10 条路径
    size_t width = giveWidth();
    size_t height = giveHeight();
    srand(static_cast<unsigned>(clock()));
    for (size_t p = 0; p < 10; p++) {
        QVector<Place*> route;
        size_t i = height - 1, j = 0;
        route.push_back((*this)[i][j]);
        for (int q = 0; q < height + width - 1; q++) {
            if (i == 0) {
                route.push_back((*this)[i][j++]);
                continue;
            }
            if (j == width - 1) {
                route.push_back((*this)[i--][j]);
                continue;
            }
            if (rand() % 2 == 0) {
                route.push_back((*this)[i--][j]);
            } else {
                route.push_back((*this)[i][j++]);
            }
        }
        _routes.push_back(route);
    }
}

void Map::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap common_place("://res/place/jia.jpg");
    for (size_t i = 0; i < giveHeight(); i++) {
        for (size_t j = 0; j < giveWidth(); j++) {
            Place* place = (*this)[i][j];
            if (place->isBase()) {
                painter.setPen(QPen(Qt::blue, 5));
                painter.drawRect(place->x(), place->y(), place->width() - 5, place->height() - 5);
                painter.drawLine(place->x(), place->y(), place->x() + place->width() - 5, place->y() + place->height() - 5);
                painter.drawLine(place->x() + place->width() - 5, place->y(), place->x(), place->y() + place->height() - 5);
            } else if (place->isEntrance()) {
                painter.setPen(QPen(Qt::red, 5));
                painter.drawRect(place->x(), place->y(), place->height() - 5, place->width() - 5);
                painter.drawLine(place->x(), place->y(), place->x() + place->width() - 5, place->y() + place->height() - 5);
                painter.drawLine(place->x() + place->width() - 5, place->y(), place->x(), place->y() + place->height() - 5);
            } else {
                painter.drawPixmap(place->x(), place->y(), place->height() - 5, place->width() - 5, common_place);
            }
        }
    }
}

QVector<Place*> Map::giveRandomRoute() const
{
    srand(static_cast<unsigned>(clock()));
    auto i = rand() % _routes.size();
    return _routes[i];
}
