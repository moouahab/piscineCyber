#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <curl/curl.h>
#include <iostream>
#include <string>

class HttpRequest {
    private:
        CURL                *_curl;
        std::string         _response;
        struct curl_slist   *_headers;
        static size_t       writeCallback(void* contents, size_t size, size_t nmemb, void* userp);

    public:
        ~HttpRequest();
        HttpRequest(const std::string& url);
        
        std::string     getResponse();
        void            setUserAgent(const std::string& userAgent);
};  


#endif