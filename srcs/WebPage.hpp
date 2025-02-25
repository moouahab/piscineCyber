#ifndef WEB_PAGE_HPP
#define WEB_PAGE_HPP

# include <iostream>
# include <string>
# include <filesystem> 
# include "HttpRequest.hpp"
# include <vector>
# include <exception>

class WebPage {
    private:
        HttpRequest                 _request;
        std::string                 _title;
        std::vector<std::string>    _links;
        std::vector<std::string>    _images;

        void                        parseTitle(const std::string &html);
        void                        parseImages(const  std::string &html);
        void                        parseLinks(const std::string &html);
        void                        extractLinksAndImages(std::string &html);
        public:
        WebPage(const std::string &url);
        ~WebPage();
        
        std::string                 getTitle() const;
        std::vector<std::string>    getImages() const;
        std::vector<std::string>    getLinks() const;
        void                        downloadAllImages(const std::string &downloadPath);
    };

bool            hasValidExtension(const std::string &filename);
bool            downloadImage(const std::string &imageUrl, const std::string &savePath);
std::string     extractFileName(const std::string &url);

#endif