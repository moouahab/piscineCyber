#include <string>
#include <curl/curl.h>
#include <cstdio>
#include <iostream>

bool downloadImage(const std::string &imageUrl, const std::string &savePath) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Erreur: impossible d'initialiser cURL." << std::endl;
        return false;
    }

    // Ouvre le fichier en mode binaire
    FILE* fp = fopen(savePath.c_str(), "wb");
    if (!fp) {
        std::cerr << "Erreur: impossible de créer le fichier " << savePath << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, imageUrl.c_str());
    // Cette fonction anonyme écrit directement dans le fichier
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 
        +[](void* ptr, size_t size, size_t nmemb, void* stream) -> size_t {
            return fwrite(ptr, size, nmemb, (FILE*)stream);
        }
    );
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Gérer les redirections

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Erreur cURL: " << curl_easy_strerror(res) 
                  << " pour " << imageUrl << std::endl;
    }

    fclose(fp);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK);
}


std::string extractFileName(const std::string &url) {
    // On cherche la dernière occurence de '/' dans l'URL
    size_t pos = url.find_last_of("/\\");
    if (pos == std::string::npos) {
        return url; // Pas de slash, on renvoie l'URL telle quelle
    }
    return url.substr(pos + 1);
}

bool endsWith(const std::string &str, const std::string &suffix) {
    if (str.size() < suffix.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

bool hasValidExtension(const std::string &filename) {
    // Convertir en minuscule
    std::string lower;
    lower.reserve(filename.size());
    for (char c : filename) {
        lower.push_back(std::tolower(static_cast<unsigned char>(c)));
    }

    return endsWith(lower, ".jpg")  ||
           endsWith(lower, ".jpeg") ||
           endsWith(lower, ".png")  ||
           endsWith(lower, ".gif")  ||
           endsWith(lower, ".bmp");
}
