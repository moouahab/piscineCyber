#include "WebPage.hpp"
#include "OptionRegistry.hpp"
#include "DepthOption.hpp"
#include "RecursiveOption.hpp"
#include "PathOption.hpp"  // Ajouter ce #include

int main(int argc, char **argv) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [-rlp] [N] <url> [download_path]" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        OptionRegistry registry;
        registry.registerOption('l', std::make_unique<DepthOption>());
        registry.registerOption('r', std::make_unique<RecursiveOption>());
        registry.registerOption('p', std::make_unique<PathOption>());

        registry.parse(argc, argv);
        registry.applyOptions();

        DepthOption* depthOpt = dynamic_cast<DepthOption*>(registry.getOption('l'));
        RecursiveOption* recOpt = dynamic_cast<RecursiveOption*>(registry.getOption('r'));
        PathOption* pathOpt   = dynamic_cast<PathOption*>(registry.getOption('p'));

        int depth = depthOpt ? depthOpt->getDepth() : 5;
        bool recursive = recOpt ? recOpt->isRecursive() : false;
        std::string downloadPath = pathOpt ? pathOpt->getPath() : "./data/";
        std::string url = argv[argc - 1];

        if (recursive) {
            std::unordered_set<std::string> visited;
            recOpt->setDepth(depth);
            recOpt->setDownloadPath(downloadPath);
            recOpt->crawl(url, 0, visited);
        } else {
            WebPage page(url);
            page.downloadAllImages(downloadPath);
        }

    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
