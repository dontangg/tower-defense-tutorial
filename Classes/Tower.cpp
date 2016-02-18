//
//  Tower.cpp
//  TowerDefense
//
//  Created by Don Wilson on 2/15/16.
//
//

#include "Tower.hpp"
#include "Enemy.hpp"
#include "HelloWorldScene.h"

Tower::Tower(HelloWorld *theGame, const Vec2& location) : attackRange(70), damage(10), fireRate(1), chosenEnemy(nullptr), theGame(theGame)
{
    this->mySprite = Sprite::create("tower.png");
    this->mySprite->setPosition(location);
    this->addChild(this->mySprite);

#ifdef DRAW_DEBUG_INFO
    this->circle = DrawNode::create();
    this->circle->drawCircle(location, this->attackRange, 360, 30, false, Color4F::WHITE);
    this->addChild(this->circle);
#endif

    theGame->addChild(this);

    scheduleUpdate();
}

void Tower::attackEnemy()
{
    schedule(CC_SCHEDULE_SELECTOR(Tower::shootWeapon), fireRate);
}

void Tower::shootWeapon(float delta)
{
    Sprite *bullet = Sprite::create("bullet.png");
    theGame->addChild(bullet);
    bullet->setPosition(mySprite->getPosition());
    bullet->runAction(Sequence::create(MoveTo::create(0.1, chosenEnemy->mySprite->getPosition()),
                                       CallFunc::create(CC_CALLBACK_0(Tower::damageEnemy, this)),
                                       RemoveSelf::create(),
                                       nullptr));
}

void Tower::damageEnemy()
{
    if (chosenEnemy)
        chosenEnemy->getDamaged(damage);
}

void Tower::targetKilled()
{
    chosenEnemy = nullptr;

    unschedule(CC_SCHEDULE_SELECTOR(Tower::shootWeapon));
}

void Tower::lostSightOfEnemy()
{
    if (chosenEnemy) {
        chosenEnemy->gotLostSight(this);
        chosenEnemy = nullptr;
    }

    unschedule(CC_SCHEDULE_SELECTOR(Tower::shootWeapon));
}

void Tower::chosenEnemyForAttack(Enemy *enemy)
{
    chosenEnemy = enemy;
    attackEnemy();
    enemy->getAttacked(this);
}

void Tower::update(float delta)
{
    if (chosenEnemy) {
        // We make it turn to target the enemy chosen
        Vec2 normalized = chosenEnemy->mySprite->getPosition() - mySprite->getPosition();
        normalized.normalize();

        mySprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x)) + 90);

        if (!theGame->circlesCollide(mySprite->getPosition(), attackRange, chosenEnemy->mySprite->getPosition(), 1))
        {
            lostSightOfEnemy();
        }
    } else {
        auto allEnemies = theGame->enemies;
        for (auto it = allEnemies.begin(); it != allEnemies.end(); ++it)
        {
            Enemy *enemy = *it;
            if (theGame->circlesCollide(mySprite->getPosition(), attackRange, enemy->mySprite->getPosition(), 1))
            {
                chosenEnemyForAttack(enemy);
                break;
            }
        }
    }
}
