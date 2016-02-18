//
//  Waypoint.cpp
//  TowerDefense
//
//  Created by Don Wilson on 2/15/16.
//
//

#include "Waypoint.hpp"
#include "HelloWorldScene.h"

Waypoint::Waypoint(HelloWorld *theGame, const Vec2& location, Waypoint *nextWaypoint) : theGame(theGame), nextWaypoint(nextWaypoint)
{
    this->myPosition = location;
    this->setPosition(cocos2d::Vec2::ZERO);

#ifdef DRAW_DEBUG_INFO
    DrawNode *circle = DrawNode::create();
    circle->drawCircle(location, 6, 360, 30, false, Color4F::GREEN);
    this->addChild(circle);

    DrawNode *circle2 = DrawNode::create();
    circle2->drawCircle(location, 2, 360, 30, false, Color4F::GREEN);
    this->addChild(circle2);


    if (nextWaypoint) {
        DrawNode *line = DrawNode::create();
        line->drawLine(this->myPosition, nextWaypoint->myPosition, Color4F::GREEN);
        this->addChild(line);
    }
#endif

    theGame->addChild(this);
}

