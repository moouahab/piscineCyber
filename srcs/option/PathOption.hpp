#pragma once
#include "Option.hpp"
#include <string>

// La classe gère le chemin où seront sauvegardées les images.
class PathOption : public Option {
    public:
        PathOption();
        virtual ~PathOption();

        void parse(int &i, int argc, char** argv, const std::string &groupedOptions) override;
        void apply() override;

        std::string getPath() const;

    private:
        std::string _path;
};
