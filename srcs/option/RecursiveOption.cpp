#include "RecursiveOption.hpp"
#include <iostream>

void RecursiveOption::crawl(const std::string& url, int currentDepth, std::unordered_set<std::string>& visited) {
    if (currentDepth > depth || visited.count(url)) {
        return;
    }

    visited.insert(url);
    std::cout << "[Depth " << currentDepth << "] Crawling: " << url << std::endl;

    try {
        WebPage page(url);
        page.downloadAllImages(_downloadPath);
        // Puis on explore les liens
        for (const auto& link : page.getLinks()) {
            crawl(link, currentDepth + 1, visited);
        }

    } catch (const std::exception &e) {
        std::cerr << "Erreur sur " << url << " : " << e.what() << std::endl;
    }
}
