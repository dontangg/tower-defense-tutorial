//
//  TowersPosition.cpp
//  TowerDefense
//
//  Created by Don Wilson on 2/15/16.
//
//

#include "TowersPosition.hpp"
#include <iostream>

using namespace std;

typedef map<string, int> dictionary_type;
typedef vector<dictionary_type> array_type;

dictionary_type TowersPosition::parseDictionary(pugi::xml_node& node)
{

    dictionary_type dict;
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
        if (string("key") != it->name())
            std::cout << "plist: XML dictionary key expected but not found";

        string key(it->first_child().value());
        ++it;

        if (it == node.end())
            cout << "plist: XML dictionary value expected for key " + key + "but not found";
        else if (string("key") == it->name())
            cout << "plist: XML dictionary value expected for key " + key + "but found another key node";

        dict[key] = atoi(it->first_child().value());
    }
    
    return dict;
}

array_type TowersPosition::parseArray(pugi::xml_node& node)
{
    array_type array;
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        array.push_back(parseDictionary(*it));
    
    return array;
}

TowersPosition::TowersPosition() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("TowersPosition.plist");

    if (!result) {
        cout << "plist: XML parsed with error";
        cout << result.description();
    }

    pugi::xml_node rootNode = doc.child("plist").first_child();
    this->positions = parseArray(rootNode);
}
