/*
 * 这个文件用来定义方块基类，所有的地图方块都要继承这个类
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 *
 */

#ifndef BLOCK_H
#define BLOCK_H

#include "Object.h"

class Block : public Object{
private:
    bool isHard; //软硬
    bool isBreakable, isCrossable; //能否被破坏、穿越

    class SizeController* LTSC,* RTSC,* RBSC,* LBSC;
protected:
public:
    virtual void* getPtr() override{return (Block*)this;}

    void paint(class QPainter* p)override; //重写Object的显示函数
    void update(int deltaTime) override; //重写Object的更新函数

    Block(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50, QColor color = QColor(0, 0, 0));
    virtual ~Block();
};

class SnakeBody : public Block{
private:
    bool isHead, isTail;
public:
    virtual void* getPtr() override{return (SnakeBody*)this;}
    void paint(class QPainter* p)override; //重写Object的显示函数
    void update(int deltaTime) override; //重写Object的更新函数

    SnakeBody(class Game* g, float x = 0, float y = 0, int w = 50, int h = 50, QColor color = QColor(0, 0, 0));
    virtual ~SnakeBody();

};

class FoodGenerator : public Block{
private:
    int maxDistance;

    int luTime, suTime, sbTime;
public:
    virtual void* getPtr() override{return (FoodGenerator*)this;}
    virtual QString toString()override;

    void paint(class QPainter* p)override; //重写Object的显示函数
    void update(int deltaTime) override; //重写Object的更新函数

    int getMaxDist(){return maxDistance;}
    void addBasicFood();

    FoodGenerator(class Game* g, int md, float x = 0, float y = 0, int w = 100, int h = 100, QColor color = QColor(80, 155, 80));
    virtual ~FoodGenerator();
};

class SizeController : public Object{
private:
    Block* mBlock;
    int mPos;
public:
    virtual void* getPtr() override{return (SizeController*)this;}

    void update(int deltaTime) override; //重写Object的更新函数

    SizeController(class Game* g, Block* block, int pos, float x = 0, float y = 0);
    virtual ~SizeController();

};

#endif // BLOCK_H
