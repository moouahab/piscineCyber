#include "OptionRegistry.hpp"

void OptionRegistry::registerOption(char key, std::unique_ptr<Option> opt) {
    options[key] = std::move(opt);
}

void OptionRegistry::parse(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg[0] == '-' && arg.length() > 1) { 
            for (char c : arg.substr(1)) {
                if (options.count(c)) {
                    options[c]->parse(i, argc, argv, arg);
                } else {
                    throw std::runtime_error(std::string("Unknown option: -") + c);
                }
            }
        }
    }
}

void  OptionRegistry::applyOptions() {
    for (auto &opt : options) {
        opt.second->apply();
    }
}

Option  *OptionRegistry::getOption(char key) {
    return options.count(key) ? options[key].get() : nullptr;
}