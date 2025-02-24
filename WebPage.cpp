#include "WebPage.hpp"
#include <regex>

WebPage::WebPage(const std::string &url) : _request(url){
    std::string html = _request.getResponse();

    if (html.empty())
        throw std::runtime_error("Failed to retrieve webpage content");

    // Parse HTML to extract title, images, and links
    extractLinksAndImages(html);
}

WebPage::~WebPage() {
    // Nothing to do here
}

// ------------------- EXTRACTION DES LIENS ET IMAGES -------------------
void WebPage::extractLinksAndImages(std::string &html)
{
    parseTitle(html);
    parseLinks(html);
    parseImages(html);
}

// ------------------- PARSING : TITRE -------------------
void WebPage::parseTitle(const std::string &html)
{
    // Exemples de regex simplifiée pour trouver le contenu d'une balise <title>
    std::regex reTitle("<title>([^<]+)</title>", std::regex::icase);
    std::smatch match;

    if (std::regex_search(html, match, reTitle)) {
        if (match.size() > 1) {
            _title = match[1].str();
        }
    }
}


// ------------------- PARSING : LIENS -------------------
void WebPage::parseLinks(const std::string &html)
{
    // Regex naïve pour extraire les href="..."
    // Attention : ça ne gère pas tous les cas (quotes simples, attributs désordonnés, etc.).
    std::regex reLink("<a[^>]*\\s+href=[\"']([^\"']+)[\"']", std::regex::icase);
    auto begin = std::sregex_iterator(html.begin(), html.end(), reLink);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;
        if (match.size() > 1) {
            _links.push_back(match[1].str());
        }
    }
}

// ------------------- PARSING : IMAGES -------------------
void WebPage::parseImages(const std::string &html)
{
    // Même logique que pour les liens, version <img ... src="...">
    std::regex reImg("<img[^>]*\\s+src=[\"']([^\"']+)[\"']", std::regex::icase);
    auto begin = std::sregex_iterator(html.begin(), html.end(), reImg);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it;
        if (match.size() > 1) {
            _images.push_back(match[1].str());
        }
    }
}

// ------------------- GETTERS -------------------
std::string WebPage::getTitle() const
{
    return _title;
}

std::vector<std::string> WebPage::getLinks() const
{
    return _links;
}

std::vector<std::string> WebPage::getImages() const
{
    return _images;
}