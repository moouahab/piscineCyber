#ifndef DEPTH_OPTION_HPP
#define DEPTH_OPTION_HPP

#include "Option.hpp"
#include <iostream>


class DepthOption : public Option {
    private:
        int _depth;

    public:
        DepthOption();
        ~DepthOption() override;
        
        int     getDepth() const;
        void    apply() override;
        void    parse(int &i, int argc, char** argv, const std::string &groupedOptions);
};

#endif