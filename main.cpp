#include "WebPage.hpp"

int main(int ac, char **av) {

    if (ac < 2 || ac > 3)
        return (std::cerr << "Usage: ./spinder url [user-agent]" << std::endl, EXIT_FAILURE);
    try
    {
        /* code */
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
        WebPage page(av[1]);
    std::cout << "Title: " << page.getTitle() << std::endl;

    for (const auto &link : page.getLinks()) {
        std::cout << "Link: " << link << std::endl;
    }

    for (const auto &img : page.getImages()) {
        std::cout << "Image: " << img << std::endl;
    }


    return (EXIT_SUCCESS);
}
