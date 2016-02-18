//
//  Waves.hpp
//  TowerDefense
//
//  Created by Don Wilson on 2/16/16.
//
//

#ifndef Waves_hpp
#define Waves_hpp

#include "pugixml.hpp"

using namespace std;

class WaveData {

private:
    vector<vector<map<string, float>>> parseOuterArray(pugi::xml_node& node);
    vector<map<string, float>> parseInnerArray(pugi::xml_node& node);
    map<string, float> parseDictionary(pugi::xml_node& node);

public:
    vector<vector<map<string, float>>> waves;

    WaveData();
    
};

#endif /* Waves_hpp */
