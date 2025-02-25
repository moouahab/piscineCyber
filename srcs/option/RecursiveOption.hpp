#ifndef RECURSIVEOPTION_HPP
#define RECURSIVEOPTION_HPP

#include "Option.hpp"
#include "WebPage.hpp"
#include "DepthOption.hpp"
#include <unordered_set>

class RecursiveOption : public Option {
private:
    bool        recursive;
    int         depth;
    std::string _downloadPath;

public:
    RecursiveOption() : recursive(false), depth(5) {} // Désactivé par défaut

    void parse(int &i, int argc, char** argv, const std::string &groupedOptions) override {
        (void)i; (void)argc; (void)argv;
        if (groupedOptions.find('r') != std::string::npos) {
            recursive = true;
        }
    }

    
    void apply() override {
        std::cout << "Mode récursif " << (recursive ? "activé" : "désactivé") << std::endl;
    }
    
    void crawl(const std::string& url, int currentDepth, std::unordered_set<std::string>& visited);
    void setDownloadPath(const std::string& path) { _downloadPath = path; }
    bool isRecursive() const { return recursive; }
    
    
    void setDepth(int d) { depth = d; }

    int  getDepth() const     { return depth; }

    void setRecursive(bool r) { recursive = r; }
};

#endif
