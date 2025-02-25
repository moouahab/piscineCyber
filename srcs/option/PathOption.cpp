#include "PathOption.hpp"
#include <stdexcept>

PathOption::PathOption() : _path("./data/") {
    // Par défaut, on pointe vers ./data/
}

PathOption::~PathOption() {}

void PathOption::parse(int &i, int argc, char** argv, const std::string &groupedOptions) {
    // Si on détecte le caractère 'p' dans "-p"
    if (groupedOptions.find('p') != std::string::npos) {
        if (i + 1 < argc) {
            // On lit la chaîne suivante comme chemin de téléchargement
            _path = argv[++i];
        } else {
            throw std::runtime_error("Missing path value after -p.");
        }
    }
}

void PathOption::apply() {}

std::string PathOption::getPath() const {
    return _path;
}
