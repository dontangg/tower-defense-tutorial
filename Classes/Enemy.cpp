//
//  Enemy.cpp
//  TowerDefense
//
//  Created by Don Wilson on 2/16/16.
//
//

#include "Enemy.hpp"
#include "Tower.hpp"
#include "Waypoint.hpp"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define HEALTH_BAR_WIDTH 20
#define HEALTH_BAR_ORIGIN -10

Enemy::Enemy(HelloWorld *theGame) : theGame(theGame), maxHp(40), currentHp(maxHp), active(false), walkingSpeed(0.5)
{
    Waypoint *waypoint = (Waypoint *)theGame->waypoints[theGame->waypoints.size() - 1];
    destinationWaypoint = waypoint->nextWaypoint;

    Vec2 pos = waypoint->myPosition;
    myPosition = pos;

    mySprite = Sprite::create("enemy.png");
    addChild(this->mySprite);
    mySprite->setPosition(pos);

    healthBarBg = DrawNode::create();
    healthBarBg->drawSolidRect(Vec2(0, 2), Vec2(0 + HEALTH_BAR_WIDTH, 0), Color4F::RED);
    addChild(healthBarBg);

    healthBarFg = DrawNode::create();
    healthBarFg->drawSolidRect(Vec2(0, 2), Vec2(0 + HEALTH_BAR_WIDTH, 0), Color4F::GREEN);
    addChild(healthBarFg);

    theGame->addChild(this);

    this->scheduleUpdate();
}

void Enemy::update(float delta)
{
    if (!active)
        return;

    if (theGame->circlesCollide(this->myPosition, 1, destinationWaypoint->myPosition, 1)) {
        
        if (destinationWaypoint->nextWaypoint) {
            destinationWaypoint = destinationWaypoint->nextWaypoint;
        } else {
            //Reached the end of the road. Damage the player
            theGame->getHpDamage();
            getRemoved();
        }
    }

    Vec2 targetPoint = destinationWaypoint->myPosition;
    float movementSpeed = walkingSpeed;

    Vec2 normalized = (targetPoint - myPosition);
    normalized.normalize();

    mySprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x)));

    myPosition += normalized * movementSpeed;

    healthBarBg->setPosition(myPosition + Vec2(HEALTH_BAR_ORIGIN, 16));
    healthBarFg->setScaleX((float)currentHp / maxHp);
    healthBarFg->setPosition(myPosition + Vec2(HEALTH_BAR_ORIGIN, 16));

    mySprite->setPosition(myPosition);
}

void Enemy::scheduleActivate(float interval)
{
    this->schedule(CC_SCHEDULE_SELECTOR(Enemy::doActivate), interval);
}

void Enemy::doActivate(float delta) {
    this->active = true;
}

void Enemy::getRemoved()
{
    for (auto it = attackedBy.begin(); it != attackedBy.end(); ++it) {
        Tower *attacker = *it;
        attacker->targetKilled();
    }

    removeFromParent();

    theGame->enemies.erase(std::remove(theGame->enemies.begin(), theGame->enemies.end(), this), theGame->enemies.end());

    // Notify the game that we killed an enemy so we can check if we can send another wave
    this->theGame->enemyGotKilled();
}

void Enemy::getAttacked(Tower *attacker)
{
    attackedBy.push_back(attacker);
}

void Enemy::gotLostSight(Tower *attacker)
{
    attackedBy.erase(std::remove(attackedBy.begin(), attackedBy.end(), attacker), attackedBy.end());
}

void Enemy::getDamaged(int damage)
{
    SimpleAudioEngine::getInstance()->playEffect("laser_shoot.wav");

    currentHp -= damage;
    if (currentHp <= 0) {
        theGame->awardGold(200);
        getRemoved();
    }
}
