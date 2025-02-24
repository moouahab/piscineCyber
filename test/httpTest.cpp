#include <iostream>
#include <string>
#include <curl/curl.h>

// Fonction pour stocker la réponse dans une chaîne
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char const *argv[]) {
    // Vérification des arguments
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: ./spinder url [user-agent]" << std::endl;
        return 1;
    }

    std::string url(argv[1]);
    CURL *curl = curl_easy_init(); // Initialisation de cURL

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Stockage de la réponse
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Gestion de l'User-Agent si fourni
        struct curl_slist* headers = NULL;
        if (argc == 3) {
            std::string user_agent = "User-Agent: " + std::string(argv[2]);
            headers = curl_slist_append(headers, user_agent.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }

        // Exécution de la requête
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "---------------------------" << std::endl;
            std::cout << "URL: " << url << std::endl;
            std::cout << "---------------------------" << std::endl;
            std::cout << "HTTP Response:" << std::endl << response << std::endl;
        }

        // Libération de la mémoire
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return 0;
}
