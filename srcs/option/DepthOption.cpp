#include "DepthOption.hpp"

void    DepthOption::parse(int &i, int argc, char** argv, const std::string &groupedOptions) {
    if (groupedOptions.find('l') != std::string::npos) {
        if (i + 1 < argc) {
            try {
                _depth = std::stoi(argv[++i]);
                if (_depth < 1) {
                    throw std::invalid_argument("Depth must be positive.");
                }
            } catch (const std::exception&) {
                throw std::runtime_error("Invalid depth value after -l.");
            }
        } else {
            throw std::runtime_error("Missing depth value after -l.");
        }
    }
}

void    DepthOption::apply() {}

int     DepthOption::getDepth() const { return _depth; }

DepthOption::DepthOption() : _depth(5) {}



DepthOption::~DepthOption() {}