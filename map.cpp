#include "map.h"
#include "ui_map.h"
#include <QDir>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QTextStream>
#include <ctime>

Map::Map(GameState* gamestate, QWidget* parent)
    : QWidget(parent)
    , _gamestate(gamestate)
    , ui(new Ui::Map)
    , _operatorSelected(-1)
{
    ui->setupUi(this);
    connectOperators();
    loadMap();
    loadRoutes();
    printLog("000000", "", QString("地图生成成功, 大小 %1 * %2").arg(giveHeight()).arg(giveWidth()));
    printLog("000000", "", "游戏开始了哦!");
}

Map::~Map()
{
    delete ui;
}

void Map::connectOperators()
{
    connect(ui->photo_irene, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 0;
        printLog("cc00cc", "CHOOSE", "Irene");
    });
    connect(ui->photo_kroos, &QPushButton::clicked, this, [=]() {
        _operatorSelected = 1;
        printLog("cc00cc", "CHOOSE", "Kroos");
    });
}

void Map::loadMap()
{
    //* currentPath 指出 exe 文件所在目录
    QFile file(QDir::currentPath() + "/map.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    printLog("000000", "", "地图文件读取成功!", this);
    QTextStream in(&file);
    size_t height = 0;
    size_t width = 0;
    in >> height >> width;

    for (size_t i = 0; i < height; i++) {
        QVector<Place*> line;
        for (size_t j = 0; j < width; j++) {
            //* 读取一个格子
            Place* place = nullptr;
            int place_type = 0;
            in >> place_type;
            switch (place_type) {
            case 0:
                _base = new Base(_gamestate, this);
                place = _base;
                break; //? 忘记加 break，在此 debug 半个小时
            case 1:
                _entrance = new Entrance(_gamestate, this);
                place = _entrance;
                break;
            case 2:
                place = new LowerPlace(_gamestate, this);
                break;
            case 3:
                place = new HigherPlace(_gamestate, this);
                break;
            case 4:
                place = new UnreachablePlace(_gamestate, this);
                break;
            }
            line.push_back(place);
            place->setGeometry(j * 100, i * 100 + 5, 100, 100);
            place->_id = QPair<size_t, size_t>(j, i); //* 按 x,y 顺序赋予 id
        }
        _lines.push_back(line);
    }
}

void Map::loadRoutes()
{
    //* 右上 --> 左下
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
            if (i == 0 || !(*this)[i - 1][j]->isReachable()) {
                route.push_back((*this)[i][j++]);
                continue;
            }
            if (j == width - 1 || !(*this)[i][j + 1]->isReachable()) {
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

void printLog(const QString& color, const QString& type, const QString& info, Map* map_init)
{
    static Map* map = nullptr;
    if (map_init != nullptr) {
        map = map_init;
    }
    if (map != nullptr) {
        map->ui->text_log->appendHtml(QString(R"(<html><head/><body><p>
                    <span style=" font-weight:600;color:#%1;">
                    %2 </span>%3</p></body></html>)")
                                          .arg(color)
                                          .arg(type)
                                          .arg(info));
    }
}

void Map::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    //? 修改完资源文件后勿忘及时在 Qt 中保存
    QPixmap common_place("://res/place/jia.jpg");
    QPixmap higher_place("://res/place/higher.png");
    QPixmap lower_place("://res/place/lower.png");
    auto drawSpecialPlace = [&](Place* place) {
        painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), lower_place);
        painter.drawRect(place->x() + 2, place->y() + 2, place->width() - 4, place->height() - 4);
        painter.drawLine(place->x() + 2, place->y() + 2, place->x() + place->width() - 2, place->y() + place->height() - 2);
        painter.drawLine(place->x() + place->width() - 2, place->y() + 2, place->x() + 2, place->y() + place->height() - 2);
    };
    for (size_t i = 0; i < giveHeight(); i++) {
        for (size_t j = 0; j < giveWidth(); j++) {
            Place* place = (*this)[i][j];
            if (place->isBase()) {
                painter.setPen(QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap));
                drawSpecialPlace(place);
            } else if (place->isEntrance()) {
                painter.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap));
                drawSpecialPlace(place);
            } else if (place->isLower()) {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), lower_place);
            } else if (place->isHigher()) {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), higher_place);
            } else {
                painter.drawPixmap(place->x(), place->y(), place->height(), place->width(), common_place);
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
