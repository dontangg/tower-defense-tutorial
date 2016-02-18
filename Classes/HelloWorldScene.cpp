#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "TowersPosition.hpp"
#include "Waypoint.hpp"
#include "Tower.hpp"
#include "WaveData.hpp"
#include "Enemy.hpp"
#include <iostream>

USING_NS_CC;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("bg.png");
    background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(background);

    loadTowerPositions();

    addWaypoints();

    // Listen for touches/clicks
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, background);

    ui_wave_label = Label::createWithBMFont("font_red_14.fnt", "WAVE: ");
    this->addChild(ui_wave_label);
    ui_wave_label->setPosition(400, visibleSize.height - 12);
    ui_wave_label->setAnchorPoint(Vec2(0, 0.5));

    loadWave();

    playerHp = 5;
    gameEnded = false;

    std::ostringstream hpStringStream;
    hpStringStream << "HP: " << playerHp;
    ui_hp_lbl = Label::createWithBMFont("font_red_14.fnt", hpStringStream.str());
    this->addChild(ui_hp_lbl, 10);
    ui_hp_lbl->setPosition(35, visibleSize.height - 12);

    playerGold = 700;

    std::ostringstream goldStringStream;
    goldStringStream << "GOLD: " << playerGold;
    ui_gold_lbl = Label::createWithBMFont("font_red_14.fnt", goldStringStream.str());
    this->addChild(ui_gold_lbl, 10);
    ui_gold_lbl->setPosition(135, visibleSize.height - 12);
    ui_gold_lbl->setAnchorPoint(Vec2(0, 0.5));

    SimpleAudioEngine::getInstance()->playBackgroundMusic("8bitDungeonLevel.mp3", true);

    return true;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    Vec2 touchLocation = touch->getLocation();

    for (std::vector<Sprite *>::iterator it = this->_towerBases.begin(); it != this->_towerBases.end(); ++it) {
        Sprite *base = *it;
        if (base->getBoundingBox().containsPoint(touchLocation) && this->canBuyTower() && !base->getUserData()) {

            playerGold -= kTOWER_COST;
            std::ostringstream stringStream;
            stringStream << "GOLD: " << playerGold;
            ui_gold_lbl->setString(stringStream.str());

            SimpleAudioEngine::getInstance()->playEffect("tower_place.wav");

            Tower *tower = new Tower(this, base->getPosition());

            base->setUserData(tower);

            this->_towers.push_back(tower);
        }
    }

    return true;
}

bool HelloWorld::canBuyTower() {
    return playerGold - kTOWER_COST >=0;
}

void HelloWorld::loadTowerPositions()
{
    TowersPosition towersPosition;
    std::vector<std::map<std::string, int>> positions = towersPosition.positions;

    this->_towerBases.reserve(positions.size());

    for(std::vector<std::map<std::string, int>>::iterator it = positions.begin(); it != positions.end(); ++it) {
        std::map<std::string, int> location = *it;

        auto towerBase = Sprite::create("open_spot.png");
        towerBase->setPosition(location["x"], location["y"]);
        this->addChild(towerBase);

        this->_towerBases.push_back(towerBase);
    }
}

void HelloWorld::addWaypoints()
{
    this->waypoints.reserve(6);

    Waypoint *waypoint1 = new Waypoint(this, Vec2(420, 35), nullptr);
    this->waypoints.push_back(waypoint1);

    Waypoint *waypoint2 = new Waypoint(this, Vec2(35, 35), waypoint1);
    this->waypoints.push_back(waypoint2);

    Waypoint *waypoint3 = new Waypoint(this, Vec2(35, 130), waypoint2);
    this->waypoints.push_back(waypoint3);

    Waypoint *waypoint4 = new Waypoint(this, Vec2(445, 130), waypoint3);
    this->waypoints.push_back(waypoint4);

    Waypoint *waypoint5 = new Waypoint(this, Vec2(445, 220), waypoint4);
    this->waypoints.push_back(waypoint5);

    Waypoint *waypoint6 = new Waypoint(this, Vec2(-40, 220), waypoint5);
    this->waypoints.push_back(waypoint6);
}

bool HelloWorld::circlesCollide(const cocos2d::Vec2 &circlePoint, float radius, const cocos2d::Vec2 &circlePointTwo, float radiusTwo)
{
    float xdif = circlePoint.x - circlePointTwo.x;
    float ydif = circlePoint.y - circlePointTwo.y;

    float distance = sqrt(xdif*xdif+ydif*ydif);

    if(distance <= radius+radiusTwo)
        return true;

    return false;
}

bool HelloWorld::loadWave()
{
    WaveData waveInfo;
    auto allWaveData = waveInfo.waves;

    if (wave >= allWaveData.size())
        return false;

    auto currentWaveData = allWaveData[wave];

    for (vector<map<string, float>>::iterator it = currentWaveData.begin(); it != currentWaveData.end(); ++it) {
        auto enemyData = *it;

        Enemy *enemy = new Enemy(this);
        enemies.push_back(enemy);
        enemy->scheduleActivate(enemyData["spawnTime"]);
    }

    wave++;

    std::ostringstream stringStream;
    stringStream << "WAVE: " << wave;
    ui_wave_label->setString(stringStream.str());

    return true;
}

void HelloWorld::enemyGotKilled()
{
    if (enemies.empty()) //If there are no more enemies.
    {
        if(!loadWave())
        {
            std::cout << "You win!";
            Director::getInstance()->replaceScene(TransitionSplitCols::create(1, HelloWorld::createScene()));
        }
    }
}

void HelloWorld::getHpDamage()
{
    SimpleAudioEngine::getInstance()->playEffect("life_lose.wav");
    
    playerHp--;

    std::ostringstream stringStream;
    stringStream << "HP: " << playerHp;
    ui_hp_lbl->setString(stringStream.str());

    if (playerHp <=0) {
        doGameOver();
    }
}

void HelloWorld::doGameOver() {
    if (!gameEnded) {
        gameEnded = true;


        Director::getInstance()->replaceScene(TransitionSplitCols::create(1, HelloWorld::createScene()));
    }
}

void HelloWorld::awardGold(int gold) {
    playerGold += gold;

    std::ostringstream stringStream;
    stringStream << "GOLD: " << playerGold;
    ui_gold_lbl->setString(stringStream.str());
}
