#ifndef OPTION_HPP
#define OPTION_HPP

#include <string>

class Option {
    public:
        virtual ~Option() {};
        virtual void apply() = 0;
        virtual void    parse(int &i, int argc, char** argv, const std::string &groupedOptions) = 0;
};


#endif 