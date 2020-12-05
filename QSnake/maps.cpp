#include "Maps.h"
#include "Prop.h"

void Map::paintEvent(QPaintEvent* ){
    QPainter p(this);
    for(auto i : *mGameWidget->getObjects())
        i->paint(&p);
}

Map::Map(QWidget* parent, GameWidget* gameWidget)
    :QWidget(parent){
    mGameWidget = gameWidget;
    mGame = mGameWidget->getGame();

    setMouseTracking(true);
}

Map::~Map(){

}



Map_Hi::Map_Hi(QWidget* parent, GameWidget* gameWidget)
    :Map(parent, gameWidget){
    mPlayer = new Player(mGame);
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);

    resize(1200, 1200);

    mGame->addBlock(new Block(mGame, 100, 350, 50, 350));   //添加了一些基本方块，组成"HI"形
    mGame->addBlock(new Block(mGame, 175, 350, 200, 50));
    mGame->addBlock(new Block(mGame, 250, 350, 50, 350));

    mGame->addBlock(new Block(mGame, 400, 200, 200, 50));
    mGame->addBlock(new Block(mGame, 400, 500, 200, 50));
    mGame->addBlock(new Block(mGame, 400, 350, 50, 350));

    mPlayer->setXY(400, 0);
}

Map_First::Map_First(QWidget* parent, GameWidget* gameWidget)
    :Map(parent, gameWidget){
    mPlayer = new Player(mGame);
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);

    resize(1600, 1600);

    mGame->addBlock(new Block(mGame, 700, 300, 400, 20));
    mGame->addBlock(new Block(mGame, 1000, 550, 200, 20));
    mGame->addBlock(new Block(mGame, 600, 700, 400, 20));
    mGame->addBlock(new Block(mGame, 1200, 700, 300, 20));
    mGame->addBlock(new Block(mGame, 700, 900, 500, 20));
    mGame->addBlock(new Block(mGame, 1100, 1000, 400, 20));
    mGame->addBlock(new Block(mGame, 400, 1000, 600, 20));
    mGame->addBlock(new Block(mGame, 1000, 1200, 300, 20));
    mGame->addBlock(new Block(mGame, 800, 1300, 400, 20));

    mGame->addBlock(new Block(mGame, 800, 1500, 1600, 200));
    mGame->addBlock(new Block(mGame, 800, 100, 1600, 200));
    mGame->addBlock(new Block(mGame, 100, 800, 200, 1600));
    mGame->addBlock(new Block(mGame, 1500, 800, 200, 1600));

    mPlayer->setXY(400, 1300);
}

Map_Snake_Plain::Map_Snake_Plain(QWidget* parent, GameWidget* gameWidget)
    :Map(parent, gameWidget){

    mPlayer = new Snake(mGame, 500, 500);
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);
    static_cast<Snake*>(mPlayer)->setKeys("Up", "Down", "Left", "Right");

    resize(1000, 1000);
}

Map_Snake_1::Map_Snake_1(QWidget* parent, GameWidget* gameWidget)
    :Map(parent, gameWidget){

    mPlayer = new Snake(mGame, 500, 500);
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);
    static_cast<Snake*>(mPlayer)->setKeys("Up", "Down", "Left", "Right");

    resize(1600, 1600);

    mGame->addBlock(new Block(mGame, 800, 1525, 1600, 200, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 800, 75, 1600, 200, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 75, 800, 200, 1600, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 1525, 800, 200, 1600, QColor(60, 40, 40)));

    mGame->addBlock(new Block(mGame, 800, 800, 350, 350, QColor(60, 40, 40)));

    mGame->addBlock(new FoodGenerator(mGame, 800, 800, 800));
}

Map_Snake_2::Map_Snake_2(QWidget* parent, GameWidget* gameWidget)
    :Map(parent, gameWidget){

    mPlayer = new Snake(mGame, 300, 300, 50, 50, QColor(130, 180, 255));
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);

    mPlayer = new Snake(mGame, 700, 700, 50, 50, QColor(255, 180, 145));
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);
    mPlayer->setVel(-250, 0);

    static_cast<Snake*>(mPlayer)->setKeys("Up", "Down", "Left", "Right");

    resize(1000, 1000);

    mGame->addBlock(new Block(mGame, 500, 0, 1000, 50, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 500, 1000, 1000, 50, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 0, 500, 50, 1000, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 1000, 500, 50, 1000, QColor(60, 40, 40)));

    mGame->addBlock(new FoodGenerator(mGame, 500, 500, 500, 150, 150));
}

Map_Snake_3::Map_Snake_3(QWidget* parent, GameWidget* gameWidget)
    :Map(parent, gameWidget){

    mPlayer = new Snake(mGame, 300, 300, 50, 50, QColor(130, 180, 255));
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);

    mPlayer = new AISnake(mGame, 700, 700, 50, 50, QColor(255, 180, 145));
    mGame->addBeing(mPlayer);
    mGame->addPlayer(mPlayer);

    resize(1000, 1000);

    mGame->addBlock(new Block(mGame, 500, 0, 1000, 50, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 500, 1000, 1000, 50, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 0, 500, 50, 1000, QColor(60, 40, 40)));
    mGame->addBlock(new Block(mGame, 1000, 500, 50, 1000, QColor(60, 40, 40)));

    mGame->addBlock(new FoodGenerator(mGame, 500, 500, 500, 150, 150));
}

