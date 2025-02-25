#ifndef OPTIONREGISTRY_HPP
#define OPTIONREGISTRY_HPP

#include "Option.hpp"
#include <string>
#include <map>
#include <memory>
#include <iostream>

class OptionRegistry {
private:
    std::map<char, std::unique_ptr<Option>> options; // Stocke les options

public:
    void registerOption(char key, std::unique_ptr<Option> opt);

    void parse(int argc, char** argv);
    void applyOptions();


    Option* getOption(char key);
};

#endif
