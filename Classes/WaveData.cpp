//
//  Waves.cpp
//  TowerDefense
//
//  Created by Don Wilson on 2/16/16.
//
//

#include "WaveData.hpp"
#include <iostream>

typedef std::map<std::string, float> dictionary_type;
typedef std::vector<dictionary_type> inner_array_type;
typedef std::vector<inner_array_type> outer_array_type;

dictionary_type WaveData::parseDictionary(pugi::xml_node& node)
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

        dict[key] = atof(it->first_child().value());
    }

    return dict;
}

inner_array_type WaveData::parseInnerArray(pugi::xml_node& node)
{
    inner_array_type array;
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        array.push_back(parseDictionary(*it));

    return array;
}

outer_array_type WaveData::parseOuterArray(pugi::xml_node& node)
{
    outer_array_type array;
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
        array.push_back(parseInnerArray(*it));

    return array;
}

WaveData::WaveData() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("Waves.plist");

    if (!result) {
        std::cout << "plist: XML parsed with error";
        std::cout << result.description();
    }

    pugi::xml_node rootNode = doc.child("plist").first_child();
    this->waves = parseOuterArray(rootNode);
}
