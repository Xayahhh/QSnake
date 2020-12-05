#ifndef MAPS_H
#define MAPS_H

#include "widgets.h"


class Map : public QWidget{
    Q_OBJECT
private:
protected:
    GameWidget* mGameWidget;
    Game* mGame;
    Player* mPlayer;
    void paintEvent(QPaintEvent*)override;
public:
    Map(QWidget* parent = nullptr, GameWidget* gameWidget = nullptr);
    ~Map();
};


class Map_Hi : public Map{
    Q_OBJECT
protected:
public:
    Map_Hi(QWidget* parent = nullptr, GameWidget* gameWidget = nullptr);
    ~Map_Hi(){}
};


class Map_First : public Map{
    Q_OBJECT
protected:
public:
    Map_First(QWidget* parent = nullptr, GameWidget* gameWidget = nullptr);
    ~Map_First(){}
};

class Map_Snake_Plain : public Map{
    Q_OBJECT
protected:
public:
    Map_Snake_Plain(QWidget* parent = nullptr, GameWidget* gameWidget = nullptr);
    ~Map_Snake_Plain(){}
};

class Map_Snake_1 : public Map{
    Q_OBJECT
protected:
public:
    Map_Snake_1(QWidget* parent = nullptr, GameWidget* gameWidget = nullptr);
    ~Map_Snake_1(){}
};
class Map_Snake_2 : public Map{
    Q_OBJECT
protected:
public:
    Map_Snake_2(QWidget* parent = nullptr, GameWidget* gameWidget = nullptr);
    ~Map_Snake_2(){}
};
class Map_Snake_3 : public Map{
    Q_OBJECT
protected:
public:
    Map_Snake_3(QWidget* parent = nullptr, GameWidget* gameWidget = nullptr);
    ~Map_Snake_3(){}
};

#endif // MAPS_H
