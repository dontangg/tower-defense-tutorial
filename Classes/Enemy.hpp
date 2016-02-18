//
//  Enemy.hpp
//  TowerDefense
//
//  Created by Don Wilson on 2/16/16.
//
//

#ifndef Enemy_hpp
#define Enemy_hpp

USING_NS_CC;
using namespace std;

class Waypoint;
class Tower;
class HelloWorld;

class Enemy : Node {
    
private:
    Vec2 myPosition;
    int maxHp;
    int currentHp;
    float walkingSpeed;
    Waypoint *destinationWaypoint;
    bool active;
    HelloWorld *theGame;
    DrawNode *healthBarBg;
    DrawNode *healthBarFg;
    vector<Tower*> attackedBy;

public:
    Sprite *mySprite;

    Enemy(HelloWorld *theGame);

    void update(float delta);
    void scheduleActivate(float interval);
    void doActivate(float delta);
    void getRemoved();
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    void getDamaged(int damage);
};

#endif /* Enemy_hpp */
