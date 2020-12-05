#include "Object.h"
#include "Game.h"

Object::~Object(){
    if (isSelected){
        mGame->setSelected(nullptr);
    }

    if (mType != "Partical" && mType != "DyingLabel" && mType != "SizeController" && !mGame->getPaused()){
        int parX, parY, parXV, parYV, parSize, parTime;
        for (int i = 0; i < (width + height) / 30 + 3; i++){
            parX = xPos + (rand() % width - width / 2); parY = yPos + (rand() % height - height / 2);
            parXV = xVel + (rand() % 200 - 100); parYV = yVel + (rand() % 200 - 100);
            parSize = rand() % (width + height) / 10 + 8;
            parTime = rand() % 500 + 500;
            mGame->addObject(new Partical(mGame, parTime, parX, parY, parSize, parSize, parXV, parYV, mColor));
        }
    }
}

QString Object::toString(){
    QString result = "";
    result += mType + ":";
    result += QString::number(xPos) + ":";
    result += QString::number(yPos) + ":";
    result += QString::number(xVel) + ":";
    result += QString::number(yVel) + ":";
    result += QString::number(width) + ":";
    result += QString::number(height) + ":";
    result += QString::number(mColor.red()) + ":";
    result += QString::number(mColor.green()) + ":";
    result += QString::number(mColor.blue()) + ":";
    return result;
}

void Object::paint(class QPainter *p){
    if (isSelected){
        p->save();
        QPen pen(QColor(210, 120, 80));
        pen.setWidth(5);
        p->setPen(pen);
        p->drawRect(xPos-width/2, yPos-height/2, width, height);
        p->restore();
    }
} //子类需要重写，用于显示

void Object::update(int deltaTime){
    xPos += xVel * deltaTime / 1000;
    yPos += yVel * deltaTime / 1000;
    xVel += xAcc * deltaTime / 1000;
    yVel += yAcc * deltaTime / 1000;
} //子类需要重写，用于更新
