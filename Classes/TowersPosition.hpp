//
//  TowersPosition.hpp
//  TowerDefense
//
//  Created by Don Wilson on 2/15/16.
//
//

#ifndef TowersPosition_hpp
#define TowersPosition_hpp

#include "pugixml.hpp"

using namespace std;

class TowersPosition {

private:
    vector<map<string, int>> parseArray(pugi::xml_node& node);
    map<string, int> parseDictionary(pugi::xml_node& node);

public:
    TowersPosition();

    vector<map<string, int>> positions;
    
};

#endif /* TowersPosition_hpp */
