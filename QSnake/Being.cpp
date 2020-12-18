/*
 * 这个文件用来实现生物类
 *
 * 修改日志：
 * JZ   2020/10/20   00:24   创建
 * JZ   2020/10/23   01:25   增加玩家类Player
 */

#include "Being.h"
#include "Prop.h"

void Being::paint(class QPainter *p){
    if (isVisible){      //随便写的，目前只显示一个小黑块
        p->save();
        p->setBrush(QBrush(Qt::black));
        p->drawRect(int(xPos - 25), int(yPos - 25), 50, 50);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);
}

void Being::update(int deltaTime){
    Object::update(deltaTime);
}

Being::Being(Game* g, float x, float y, int w, int h)
    :Object(g){
    curHealth = maxHealth = 100;
    lifeNum = 1;
    yAcc = 4400;
    xPos = x; yPos = y;
    width = w; height = h;
    mStatus = Stay;
    mType = "Being";
    mInfo = "Basic";
}

void Being::addProp(class Prop *prop){
    mProps.push_back(prop);
}

void Being::useProp(class Prop *prop){
    mProps.removeOne(prop);
}

Being::~Being(){
}


void Player::paint(class QPainter *p){
    if (isVisible){      //显示一个小黑块，上面有个笑脸
        p->save();
        p->setPen(Qt::NoPen);
        p->setBrush(QBrush(Qt::black));
        p->drawRect(int(xPos - 25), int(yPos - 25), 50, 50);   //以pos为中心绘制方块
        p->setBrush(QBrush(Qt::white));
        p->drawRect(int(xPos - 15) , int(yPos - 15), 10, 10);
        p->drawRect(int(xPos + 5) , int(yPos - 15), 10, 10);
        p->drawRect(int(xPos - 15) , int(yPos + 10), 30, 8);
        p->drawRect(int(xPos - 15), int(yPos + 2), 8, 10);
        p->drawRect(int(xPos + 7) , int(yPos + 2), 8, 10);
        p->restore();
    }
    Object::paint(p);

}

void Player::update(int deltaTime){
    Being::update(deltaTime);
    float crashPossTop, crashPossBottom, crashPossLeft, crashPossRight;
    bool crashOnBlockTop = false, crashOnBlockBottom = false, crashOnBlockLeft = false, crashOnBlockRight = false;
    for (auto i : *mGame->getBlocks()){
        crashPossTop = crashPossBottom = crashPossLeft = crashPossRight = -1;
        if (i->getY() - i->getH()/2 - yPos - height/2 + 2*yVel * deltaTime / 1000>= 0 && yVel > 0)
            crashPossTop = (i->getY() - i->getH()/2 - yPos - height/2 + 2*yVel * deltaTime / 1000) / yVel;

        if (yPos - height/2 - i->getY() - i->getH()/2 - 2*yVel * deltaTime / 1000>= 0 && yVel < 0)
            crashPossBottom = - (yPos - height/2 - i->getY() - i->getH()/2 - 2*yVel * deltaTime / 1000) / yVel;

        if (i->getX() - i->getW()/2 - xPos - width/2 + 2*xVel * deltaTime / 1000>= 0 && xVel > 0)
            crashPossLeft = (i->getX() - i->getW()/2 - xPos - width/2 + 2*xVel * deltaTime / 1000) / xVel;

        if (xPos - width/2 - i->getX() - i->getW()/2 - 2*xVel * deltaTime / 1000>= 0 && xVel < 0)
            crashPossRight = - (xPos - width/2 - i->getX() - i->getW()/2 - 2*xVel * deltaTime / 1000) / xVel;

        if (qAbs(i->getY() - yPos)*2 < height + i->getH() && (i->getX() - xPos)*2 <= i->getW() + width && i->getX() - xPos != 0)
            if (crashPossLeft >= 0 && (crashPossLeft < crashPossTop || crashPossTop == -1) && (crashPossLeft < crashPossBottom || crashPossBottom == -1)) {
                crashOnBlockLeft = true;
                //qDebug() << "Crashed Left: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                xPos = i->getX() - i->getW()/2 - width/2;
        }
        if (qAbs(i->getY() - yPos)*2 < height + i->getH() && (xPos - i->getX())*2 <= i->getW() + width && i->getX() - xPos != 0){
            if (crashPossRight >= 0 && (crashPossRight < crashPossTop || crashPossTop == -1) && (crashPossRight < crashPossBottom || crashPossBottom == -1)){
                //qDebug() << "Crashed Right: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                crashOnBlockRight = true;
                xPos = i->getX() + i->getW()/2 + width/2;
        }}
        if (qAbs(i->getX() - xPos)*2 < width + i->getW() && (i->getY() - yPos)*2 <= i->getH() + height && i->getY() - yPos != 0)
            if (crashPossTop >= 0 && (crashPossTop < crashPossLeft || crashPossLeft == -1)&& (crashPossTop < crashPossRight ||  crashPossRight == -1)){
                crashOnBlockTop = true;
                //qDebug() << "Crashed Top: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                yPos = i->getY() - i->getH()/2 - height/2;
        }
        if (qAbs(i->getX() - xPos)*2 < width + i->getW() && (yPos - i->getY())*2 <= i->getH() + height && i->getY() - yPos != 0)
            if (crashPossBottom >= 0 && (crashPossBottom < crashPossLeft || crashPossLeft == -1) && (crashPossBottom < crashPossRight || crashPossRight == -1)){
                crashOnBlockBottom = true;
                //qDebug() << "Crashed Bottom: " << xPos << ", " << yPos << "  " << i->getX() << ", " << i->getY();
                yPos = i->getY() + i->getH()/2 + height/2;
        }
    }
    if (crashOnBlockTop){
        yVel = 0;
        mStatus = Stay;
    }
    if (crashOnBlockBottom){
        yVel = 0;
    }
    if (crashOnBlockLeft){
        xVel = 0;
    }
    if (crashOnBlockRight){
        xVel = 0;
    }
}

void Player::processInput(QVector<QString>* vec){
    if ((vec->contains("w"))and mStatus != Jump){
        yVel = -1500; mStatus = Jump;
    }
    if (vec->contains("a"))
        xVel = -200;
    else if (vec->contains("d"))
        xVel = 200;
    else xVel = 0;

}

void Player::addScore(int s){
    score += s;
}

Player::Player(Game* g, float x, float y, int w, int h)
    :Being(g, x, y, w, h){
    width = 50;
    height = 50;
    mType = "Player";
    mInfo = "Basic";
}

Player::~Player(){
}

void Snake::paint(class QPainter *p){

    if (isVisible){
        p->save();
        if (strongTime > 0)
            p->setBrush(QBrush(QColor(255, 215, 0)));
        else p->setBrush(QBrush(Qt::white));
        p->setPen(Qt::NoPen);
        p->drawRect(int(xPos - 15*width/50) , int(yPos - 15*width/50), 10*width/50, 10*width/50);
        p->drawRect(int(xPos + 5*width/50) , int(yPos - 15*width/50), 10*width/50, 10*width/50);
        p->drawRect(int(xPos - 15*width/50) , int(yPos + 10*width/50), 30*width/50, 8*width/50);
        p->drawRect(int(xPos - 15*width/50), int(yPos + 2*width/50), 8*width/50, 10*width/50);
        p->drawRect(int(xPos + 7*width/50) , int(yPos + 2*width/50), 8*width/50, 10*width/50);
        p->restore();
    }

    Object::paint(p);
}

void Snake::update(int deltaTime){
    mTime += deltaTime;
    if (strongTime > 0) strongTime -= deltaTime;
    updateIntv = 1000 / qAbs(xVel+yVel) * width;
    if (speedUpTimeLast > 0){
        mTime += deltaTime;
        speedUpTimeLast -= deltaTime;
    }
    if (mTime >= updateIntv){
        deltaTime = updateIntv;
        if (xVel)xPos += xVel/qAbs(xVel) * width;
        if (yVel)yPos += yVel/qAbs(yVel) * width;

        xVel_LastUpdated = xVel;
        yVel_LastUpdated = yVel;

        float foreX = head->getX(), foreY = head->getY();
        head->setXY(xPos, yPos);

        int index = 0;
        for(;index < mBlocks.length();index++){
            if (!mGame->getBlocks()->contains(mBlocks[index]))
                break;
            if (mBlocks[index] != head){
                float tempX = mBlocks[index]->getX(), tempY = mBlocks[index]->getY();
                mBlocks[index]->setXY(foreX, foreY);
                foreX = tempX; foreY = tempY;
            }
        }
        int Index = index;
        for(;index < mBlocks.length();index++)
            if (mGame->getBlocks()->contains(mBlocks[index]))
                mGame->removeObject(mBlocks[index]);
        mBlocks.remove(Index, mBlocks.length() - Index);

        if (growNum){
            Block* newBlock = new SnakeBody(mGame, foreX, foreY, width, width, mColor);
            mBlocks.push_back(newBlock);
            mGame->addBlock(newBlock);
            growNum--;
        }

        for (auto i : *mGame->getBlocks()){
            if (i != head && qAbs(i->getY() - yPos)*2 < height + i->getH() && qAbs(i->getX() - xPos)*2 < i->getW() + width){
                if (strongTime <= 0)
                    curHealth -= 100;
                else mGame->addObject(new DyingLabel(mGame, 800, xPos - 90, yPos, 200, 30, 0, -180, "invincible!", QColor(255, 215, 0)));
            }
        }

        for (auto i : *mGame->getProps()){
            if ( qAbs(i->getY() - yPos)*2 < height + i->getH() && qAbs(i->getX() - xPos)*2 < i->getW() + width){
                i->collected(this);
            }
        }


        if (curHealth <= 0){
            lifeNum --;
            score = score / 2;
            if (lifeNum <= 0){
                mGame->removeObject(this);
            }
            else{
                curHealth = maxHealth;
                for(auto i : mBlocks){
                    mGame->removeObject(i);
                }
                mBlocks.clear();
                head = new SnakeBody(mGame, initX, initY, width, width, mColor);
                mBlocks.push_back(head);
                mGame->addBlock(head);
                xPos = initX; yPos = initY;
                strongTime = 2500;
            }
        }

        mTime -= updateIntv;
    }
}

void Snake::processInput(QVector<QString>* vec){
    if (yVel_LastUpdated == 0){
        if (vec->contains(upKey)){
            yVel = -qAbs(xVel + yVel); xVel = 0;
        }
        else if(vec->contains(downKey)){
            yVel = qAbs(xVel + yVel); xVel = 0;
        }
    }
    if (xVel_LastUpdated == 0){
        if (vec->contains(leftKey)){
            xVel = -qAbs(xVel + yVel); yVel = 0;
        }
        else if (vec->contains(rightKey)){
            xVel = qAbs(xVel + yVel); yVel = 0;
        }
    }
}

Snake::Snake(Game* g, float x, float y, int w, int h, QColor color, float vel)
    :Player(g, x, y, w, h){
    initX = x; initY = y;
    width = height = w;

    mVel = xVel = vel;
    yVel = 0;
    mColor = color; paintOrder = 10;
    mType = "Snake";
    mInfo = "A hungry sn\n ake looking\n for food.";

    head = new SnakeBody(mGame, xPos, yPos, width, width, mColor);
    mBlocks.push_back(head);

    for(auto i : mBlocks){
        mGame->addBlock(i);
    }
}

Snake::~Snake(){
    for(auto i : mBlocks){
        mGame->removeObject(i);
    }
}

void Snake::useProp(class Prop *prop){
    Being::useProp(prop);
}


void Snake::speedUp(int time){
    speedUpTimeLast = time;
}

AISnake::AISnake(class Game* g, float x, float y, int w, int h, QColor color, float vel)
    :Snake(g, x, y, w, h, color, vel){
    mType = "AISnake";
    mInfo = "AI control\n ed snake";
}

void AISnake::processInput(QVector<QString> *){
    QVector<Prop*> props = *mGame->getProps();
    QVector<Block*> blocks = *mGame->getBlocks();
    float v = qAbs(xVel + yVel);
    if (props.length() > 0 && blocks.length() > 0){
        int dis = (props[0]->getX() - xPos)*(props[0]->getX() - xPos) + (props[0]->getY() - yPos)*(props[0]->getY() - yPos);
        int tar = 0;
        for(int i = 1; i < props.length();i++)
            if (dis > (props[i]->getX() - xPos)*(props[i]->getX() - xPos) + (props[0]->getY() - yPos)*(props[0]->getY() - yPos))
            {
                dis = (props[i]->getX() - xPos)*(props[i]->getX() - xPos) + (props[0]->getY() - yPos)*(props[0]->getY() - yPos);
                tar = i;
            }

        bool flagRight = false, flagLeft = false, flagUp = false, flagDown = false;
        for(auto i: blocks)if (qAbs(i->getY() - yPos)*2 < height + i->getH() && qAbs(i->getX() - xPos - width)*2 < i->getW() + width){ flagRight = true; break;}
        for(auto i: blocks)if (qAbs(i->getY() - yPos)*2 < height + i->getH() && qAbs(i->getX() - xPos + width)*2 < i->getW() + width){ flagLeft = true; break;}
        for(auto i: blocks)if (qAbs(i->getY() - yPos + width)*2 < height + i->getH() && qAbs(i->getX() - xPos)*2 < i->getW() + width){ flagUp = true; break;}
        for(auto i: blocks)if (qAbs(i->getY() - yPos - width)*2 < height + i->getH() && qAbs(i->getX() - xPos)*2 < i->getW() + width){ flagDown = true; break;}

        if (props[tar]->getX() > xPos + width/2){
            if(!flagRight){xVel = v; yVel = 0;}
            else{
                if (props[tar]->getY() > yPos && !flagDown){xVel = 0;yVel = v;}
                else if (!flagUp){xVel = 0; yVel = -v;}
                else {xVel = -v;yVel = 0;}
            }
        }
        else if(props[tar]->getX() < xPos - width/2){
            if(!flagLeft){xVel = -v; yVel = 0;}
            else{
                if (props[tar]->getY() > yPos && !flagDown){xVel = 0;yVel = v;}
                else if (!flagUp){xVel = 0; yVel = -v;}
                else {xVel = v;yVel = 0;}
            }
        }
        else{
            if (props[tar]->getY() > yPos && !flagDown){xVel = 0;yVel = v;}
            else if (!flagUp){xVel = 0; yVel = -v;}
            else if (!flagLeft){xVel = -v;yVel = 0;}
            else {xVel = v;yVel = 0;}
        }
    }
}

AISnake::~AISnake(){
}
