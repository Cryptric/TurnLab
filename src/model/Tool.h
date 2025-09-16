//
// Created by gawain on 9/16/25.
//

#ifndef TURNLAB_TOOLDATA_H
#define TURNLAB_TOOLDATA_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

struct Tool {
    int number;
    std::string description;
    std::string isoCode;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Tool, number, description, isoCode);
};

struct ToolTable {
    std::vector<Tool> tools;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ToolTable, tools);
};

#endif //TURNLAB_TOOLDATA_H