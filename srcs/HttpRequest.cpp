#include "HttpRequest.hpp"

size_t HttpRequest::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

HttpRequest::HttpRequest(const std::string &url) : _headers(nullptr) // Initialisation de _headers
{
    _curl = curl_easy_init();
    if (_curl) {
        curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, HttpRequest::writeCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_response);
        curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    }
}

HttpRequest::~HttpRequest()
{
    if (_curl) {
        if (_headers)
            curl_slist_free_all(_headers); // Libérer les headers avant de fermer cURL
        curl_easy_cleanup(_curl);
    }
}

void HttpRequest::setUserAgent(const std::string& userAgent)
{
    if (_headers) {
        curl_slist_free_all(_headers); // Libérer l'ancien User-Agent
    }
    _headers = curl_slist_append(nullptr, ("User-Agent: " + userAgent).c_str());
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers);
}

std::string HttpRequest::getResponse()
{
    if (_curl) {
        CURLcode res = curl_easy_perform(_curl);
        if (res != CURLE_OK) {
            std::cerr << "Erreur cURL: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return _response;
}
