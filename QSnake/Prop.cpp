#include "Prop.h"
#include "Game.h"

Prop::Prop(class Game* g, float x, float y, int w, int h)
    :Object(g){
    xPos = x; yPos = y; width = w; height = h;
    mType = "Prop";
    mInfo = "Basic";
}

Prop::~Prop(){

}

void Prop::collected(class Being *being){
    setOwner(being);
    being->addProp(this);
}


BasicFood::BasicFood(class Game* g, class FoodGenerator* ge)
    :Prop(g, 0, 0, 20, 20){
    mGenerator = ge;
    bool flag;
    if (ge){
        minX = ge->getX() - ge->getMaxDist(); minY = ge->getY() - ge->getMaxDist();
        maxX = ge->getX() + ge->getMaxDist(); maxY = ge->getY() + ge->getMaxDist();
        do{
            flag = false;
            xPos = rand() % ((maxX - minX) / 50) * 50 + minX;
            yPos = rand() % ((maxY - minY) / 50) * 50 + minY;
            for (auto i: *mGame->getBlocks()){
                if (qAbs(i->getY() - yPos)*2 < height + i->getH() && qAbs(i->getX() - xPos)*2 < i->getW() + width)
                    flag = true;
            }
        }while (flag);
    }
    mType = "BasicFood";
    mInfo = "Basic food \n for snakes";
}

BasicFood::BasicFood(class Game* g, float x, float y)
    :Prop(g, x, y, 20, 20){
    mGenerator = nullptr;
    xPos = x; yPos = y;
    mType = "BasicFood";
    mInfo = "Basic food \n for snakes";
}

BasicFood::~BasicFood(){
}

void BasicFood::collected(class Being *being){
    Prop::collected(being);
    being->useProp(this);
    int s = rand() % 2 + 1;
    static_cast<Snake*>(being->getPtr())->grow(s);
    static_cast<Snake*>(being->getPtr())->addScore(s);
    mGame->addObject(new DyingLabel(mGame, 600, xPos + 90, yPos - 20, 200, 30, 0, -100, QString::number(s), QColor(150, 150, 150)));
    mGame->removeProp(this);
    if (mGenerator && mGame->getBlocks()->contains(mGenerator))
        mGenerator->addBasicFood();
}

void BasicFood::paint(QPainter *p){

    if (isVisible){      //随便写的，目前只显示一个小黑块
        p->save();
        p->setBrush(QBrush(mColor));
        p->setPen(Qt::NoPen);
        p->drawRect(int(xPos - width/2) , int(yPos - height/2), width, height);   //以pos为中心绘制方块
        p->restore();
    }
    Object::paint(p);
}

LifeUpFood::LifeUpFood(class Game* g, class FoodGenerator* ge)
    :BasicFood(g, ge){
    mColor = QColor(100, 255, 100);
    mType = "LifeUpFood";
    mInfo = "Special Food \n To Gain An \n Extra Life";
}

LifeUpFood::LifeUpFood(class Game* g, float x, float y)
    :BasicFood(g, x, y){
    mColor = QColor(100, 255, 100);
    mType = "LifeUpFood";
    mInfo = "Special Food \n To Gain An \n Extra Life";
}
LifeUpFood::~LifeUpFood(){

}

void LifeUpFood::update(int deltaTime){
    mTime += deltaTime;
    if (mTime > 500){
        int parX, parY;
        parX = xPos + (rand() % width + width) * (rand() % 3 - 1) / 2;
        parY = yPos + (rand() % height + height) * (rand() % 3 - 1) / 2;
        mGame->addObject(new Partical(mGame, 800, parX, parY, 8, 8, 0, 0, mColor));
        mTime = 0;
    }
}

void LifeUpFood::collected(class Being *being){
    Prop::collected(being);
    being->useProp(this);
    static_cast<Snake*>(being->getPtr())->addLife(1);
    mGame->addObject(new DyingLabel(mGame, 800, xPos + 90, yPos - 20, 200, 30, 0, -100, "+1 life", mColor));
    mGame->removeProp(this);
}


SpeedUpFood::SpeedUpFood(class Game* g, class FoodGenerator* ge)
    :BasicFood(g, ge){
    mColor = QColor(100, 105, 255);
    mType = "SpeedUpFood";
    mInfo = "Special Food \n To Gain Doub \n le Speed for \n 5s";
}
SpeedUpFood::SpeedUpFood(class Game* g, float x, float y)
    :BasicFood(g, x, y){
    mColor = QColor(100, 105, 255);
    mType = "SpeedUpFood";
    mInfo = "Special Food \n To Gain Doub \n le Speed for \n 5s";
}

SpeedUpFood::~SpeedUpFood(){

}

void SpeedUpFood::update(int deltaTime){
    mTime += deltaTime;
    if (mTime > 600){
        int parX, parY;
        parX = xPos + (rand() % width + width) * (rand() % 3 - 1) / 2;
        parY = yPos + (rand() % height + height) * (rand() % 3 - 1) / 2;
        mGame->addObject(new Partical(mGame, 800, parX, parY, 8, 8, 0, 0, mColor));
        mTime = 0;
    }
}

void SpeedUpFood::collected(class Being *being){
    Prop::collected(being);
    being->useProp(this);
    static_cast<Snake*>(being->getPtr())->speedUp(5000);
    mGame->addObject(new DyingLabel(mGame, 800, xPos + 80, yPos - 20, 200, 30, 0, -100, "x2 speed", mColor));
    mGame->removeProp(this);
}


ScoreBonusFood::ScoreBonusFood(class Game* g, class FoodGenerator* ge)
    :BasicFood(g, ge){
    mColor = QColor(155, 50, 50);
    width = 30; height = 30;
    mType = "ScoreBonusFood";
    mInfo = "Special Food \n To Gain Doub \n le Speed for \n 5s";
}
ScoreBonusFood::ScoreBonusFood(class Game* g, float x, float y)
    :BasicFood(g, x, y){
    mColor = QColor(155, 50, 50);
    width = 30; height = 30;
    mType = "ScoreBonusFood";
    mInfo = "Special Food \n To Gain Doub \n le Speed for \n 5s";
}

ScoreBonusFood::~ScoreBonusFood(){

}

void ScoreBonusFood::update(int deltaTime){
    mTime += deltaTime;
    if (mTime > 400){
        int parX, parY;
        parX = xPos + (rand() % width + width) * (rand() % 3 - 1) / 2;
        parY = yPos + (rand() % height + height) * (rand() % 3 - 1) / 2;
        mGame->addObject(new Partical(mGame, 800, parX, parY, 8, 8, 0, 0, mColor));
        mTime = 0;
    }
}

void ScoreBonusFood::collected(class Being *being){
    Prop::collected(being);
    being->useProp(this);
    int s = rand() % 30 + 1;
    static_cast<Snake*>(being->getPtr())->addScore(s);
    mGame->addObject(new DyingLabel(mGame, 800, xPos + 80, yPos - 20, 200, 40, 0, -100, QString::number(s), mColor));
    mGame->removeProp(this);
}

