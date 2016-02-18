//
//  Tower.hpp
//  TowerDefense
//
//  Created by Don Wilson on 2/15/16.
//
//

#ifndef Tower_hpp
#define Tower_hpp

#define kTOWER_COST 300

class HelloWorld;
class Enemy;

USING_NS_CC;

class Tower : Node
{
private:
    int attackRange;
    int damage;
    float fireRate;
    HelloWorld *theGame;
    bool attacking;
    Enemy *chosenEnemy;

    void attackEnemy();
    void lostSightOfEnemy();
    void shootWeapon(float delta);
    void damageEnemy();
    void chosenEnemyForAttack(Enemy *enemy);

public:
    Sprite *mySprite;
    DrawNode *circle;

    Tower(HelloWorld *theGame, const Vec2& location);

    void targetKilled();
    void update(float delta);
};

#endif /* Tower_hpp */
