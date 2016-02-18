//
//  Waypoint.hpp
//  TowerDefense
//
//  Created by Don Wilson on 2/15/16.
//
//

#ifndef Waypoint_hpp
#define Waypoint_hpp

USING_NS_CC;

class HelloWorld;

class Waypoint : Node
{
private:
    HelloWorld *theGame;

public:
    Vec2 myPosition;
    Waypoint *nextWaypoint;

    Waypoint(HelloWorld *theGame, const Vec2& location, Waypoint *nextWaypoint);
};

#endif /* Waypoint_hpp */
