#ifndef PROP_H
#define PROP_H

#include "Object.h"
#include <QRandomGenerator>

class Prop: public Object{
private:

protected:
    bool isCollected;
    class Being* mOwner;
    enum Status{

    };

public:
    virtual void* getPtr() override{return (Prop*)this;}

    virtual void paint(QPainter* ) override{}

    virtual void setOwner(class Being* owner){mOwner = owner;}
    virtual void collected(class Being* being);
    Prop(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50);
    ~Prop();
};


class BasicFood: public Prop{
protected:
    int minX, minY;
    int maxX, maxY;
    class FoodGenerator* mGenerator;
public:
    virtual void* getPtr() override{return (BasicFood*)this;}

    virtual void paint(QPainter* ) override;
    virtual void collected(class Being* being) override;
    BasicFood(class Game* g, class FoodGenerator* ge = nullptr);
    BasicFood(class Game* g, float x, float y);
    ~BasicFood();
};

class LifeUpFood: public BasicFood{
public:
    virtual void* getPtr() override{return (LifeUpFood*)this;}

    virtual void collected(class Being* being) override;
    virtual void update(int deltaTime) override;

    LifeUpFood(class Game* g, class FoodGenerator* ge = nullptr);
    LifeUpFood(class Game* g, float x, float y);
    ~LifeUpFood();
};

class SpeedUpFood:public BasicFood{
public:
    virtual void* getPtr() override{return (SpeedUpFood*)this;}

    virtual void collected(class Being* being) override;
    virtual void update(int deltaTime) override;

    SpeedUpFood(class Game* g, class FoodGenerator* ge = nullptr);
    SpeedUpFood(class Game* g, float x, float y);
    ~SpeedUpFood();
};

class ScoreBonusFood:public BasicFood{
public:
    virtual void* getPtr() override{return (ScoreBonusFood*)this;}

    virtual void collected(class Being* being) override;
    virtual void update(int deltaTime) override;

    ScoreBonusFood(class Game* g, class FoodGenerator* ge = nullptr);
    ScoreBonusFood(class Game* g, float x, float y);
    ~ScoreBonusFood();
};


#endif // PROP_H
