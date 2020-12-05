/*
 * 这个文件用来实现基本方块类
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 *
 */


#include "Block.h"
#include "Game.h"

void Object::destroy(){
    mGame->removeObject(this);
}

Block::Block(Game* g, float x, float y, int w, int h, QColor color)
    :Object(g)
{
    xPos = x; yPos = y; width = w; height = h;   //初始化方块的位置、大小参数
    mColor = color;
    mType = "Block";
    mInfo = "Basic";

    LTSC = new SizeController(mGame, this, 0, xPos - width/2, yPos - height/2);
    RTSC = new SizeController(mGame, this, 1, xPos + width/2, yPos - height/2);
    RBSC = new SizeController(mGame, this, 2, xPos + width/2, yPos + height/2);
    LBSC = new SizeController(mGame, this, 3, xPos - width/2, yPos + height/2);
    mGame->addObject(LTSC);
    mGame->addObject(RTSC);
    mGame->addObject(RBSC);
    mGame->addObject(LBSC);
}

Block::~Block(){
}

void Block::paint(class QPainter* p){

    if (isVisible){      //随便写的，目前只显示一个小黑块
        p->save();
        p->setBrush(QBrush(mColor));
        p->setPen(Qt::NoPen);
        p->drawRect(int(xPos - width/2) , int(yPos - height/2), width, height);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);
}

void Block::update(int deltaTime){     //用于检测处理碰撞、踩踏、摧毁等事件
    Object::update(deltaTime);
    if(!((LTSC->getX() != xPos - width/2 || LTSC->getY() != yPos - height/2)\
        &&(RTSC->getX() != xPos + width/2 || RTSC->getY() != yPos - height/2)\
            &&(RBSC->getX() != xPos + width/2 || RBSC->getY() != yPos + height/2)\
                &&(LBSC->getX() != xPos - width/2 || LBSC->getY() != yPos + height/2)\
                    )){
    if(LTSC->getX() != xPos - width/2 || LTSC->getY() != yPos - height/2){
        width = RTSC->getX() - LTSC->getX();
        height = LBSC->getY() - LTSC->getY();
        xPos = RBSC->getX() - width / 2;
        yPos = RBSC->getY() - height / 2;
    }
    else if(RTSC->getX() != xPos + width/2 || RTSC->getY() != yPos - height/2){
        width = RTSC->getX() - LTSC->getX();
        height = RBSC->getY() - RTSC->getY();
        xPos = LBSC->getX() + width / 2;
        yPos = LBSC->getY() - height / 2;
    }
    else if(RBSC->getX() != xPos + width/2 || RBSC->getY() != yPos + height/2){
        width = RBSC->getX() - LBSC->getX();
        height = RBSC->getY() - RTSC->getY();
        xPos = LTSC->getX() + width / 2;
        yPos = LTSC->getY() + height / 2;
    }
    else if(LBSC->getX() != xPos - width/2 || LBSC->getY() != yPos + height/2){
        width = RBSC->getX() - LBSC->getX();
        height = LBSC->getY() - LTSC->getY();
        xPos = RTSC->getX() - width / 2;
        yPos = RTSC->getY() + height / 2;
    }
}
    LTSC->setXY(xPos - width/2, yPos - height/2);
    RTSC->setXY(xPos + width/2, yPos - height/2);
    RBSC->setXY(xPos + width/2, yPos + height/2);
    LBSC->setXY(xPos - width/2, yPos + height/2);
}

SnakeBody::SnakeBody(Game* g, float x, float y, int w, int h, QColor color)
    :Block(g, x, y, w, h, color)
{
    mType = "SnakeBody";
    mInfo = "Kind of \b block, body \n of snake";
}

SnakeBody::~SnakeBody(){

}

void SnakeBody::update(int deltaTime){
    Block::update(deltaTime);
}
void SnakeBody::paint(class QPainter *p){
    if (isVisible){      //随便写的，目前只显示一个小黑块
        p->save();
        p->setBrush(QBrush(mColor));
        p->setPen(Qt::NoPen);
        p->drawRect(int(xPos - width/2) , int(yPos - height/2), width, height);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);
}

FoodGenerator::FoodGenerator(Game* g, int md, float x, float y, int w, int h, QColor color)
    :Block(g, x, y, w, h, color)
{
    mType = "FoodGenerator";
    mInfo = "Generate \n food";
    mTime = luTime = suTime = sbTime = 0;
    paintOrder = 5;
    maxDistance = md;

    addBasicFood();addBasicFood();addBasicFood();
}

FoodGenerator::~FoodGenerator(){

}

void FoodGenerator::update(int deltaTime){
    mTime += deltaTime;
    luTime += deltaTime;
    suTime += deltaTime;
    sbTime += deltaTime;

    if (luTime > 30000){
        mGame->addProp(new LifeUpFood(mGame, this));
        luTime = 0;
    }
    if (suTime > 15000){
        mGame->addProp(new SpeedUpFood(mGame, this));
        suTime = 0;
    }
    if (sbTime > 22000){
        mGame->addProp(new ScoreBonusFood(mGame, this));
        sbTime = 0;
    }
}

void FoodGenerator::paint(class QPainter *p){
    if (isVisible){      //随便写的，目前只显示一个小黑块
        p->save();
        p->setBrush(QBrush(mColor));
        p->setPen(Qt::NoPen);
        p->drawRect(int(xPos - width/2) , int(yPos - height/2), width, height);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);
}

void FoodGenerator::addBasicFood(){
    mGame->addProp(new BasicFood(mGame, this));
}

QString FoodGenerator::toString(){
    return Block::toString() + QString::number(maxDistance) + ":";
}

SizeController::SizeController(class Game* g, Block* block, int pos, float x, float y)
    :Object(g){
    xPos = x; yPos = y;
    width = 10; height = 10;
    mType = "SizeController";
    mInfo = "Define size \n of block";
    mPos = pos;
    mBlock = block;
    mColor = QColor(210, 120, 80);
}

SizeController::~SizeController(){

}

void SizeController::update(int){

}
