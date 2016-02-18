#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

// Comment this line out to hide the debug drawing
#define DRAW_DEBUG_INFO

#include "cocos2d.h"

class Enemy;
class Tower;
class Waypoint;

class HelloWorld : public cocos2d::Layer
{
private:
    std::vector<cocos2d::Sprite*> _towerBases;
    std::vector<Tower*> _towers;
    int wave;
    cocos2d::Label *ui_wave_label;
    int playerHp;
    cocos2d::Label *ui_hp_lbl;
    bool gameEnded;
    int playerGold;
    cocos2d::Label *ui_gold_lbl;

    void loadTowerPositions();
    void addWaypoints();
    bool loadWave();

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    bool canBuyTower();

    void ccFillPoly(const cocos2d::Vec2& poli, int points, bool closePolygon);

public:
    std::vector<Waypoint*> waypoints;
    std::vector<Enemy*> enemies;

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    bool circlesCollide(const cocos2d::Vec2& circlePoint, float radius, const cocos2d::Vec2& circlePointTwo, float radiusTwo);
    void getHpDamage();
    void doGameOver();
    void enemyGotKilled();
    void awardGold(int gold);
};

#endif // __HELLOWORLD_SCENE_H__
