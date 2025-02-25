#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sys/stat.h> // Pour la date de création du fichier
#include <exiv2/exiv2.hpp>

// Liste d’extensions prises en compte
const std::vector<std::string> validExtensions = {".jpg", ".jpeg", ".png", ".bmp", ".tiff"};

// Clés EXIF importantes (pour l'affichage)
const std::set<std::string> importantExifKeys = {
    "Exif.Photo.DateTimeOriginal",
    "Exif.Image.Make",
    "Exif.Image.Model",
    "Exif.Photo.FNumber",
    "Exif.Photo.ShutterSpeedValue",
    "Exif.Photo.ISOSpeedRatings",
    "Exif.GPSInfo.GPSLatitude",
    "Exif.GPSInfo.GPSLongitude",
    "Exif.Photo.UserComment"
};

/**
 * Vérifie si l’extension du fichier est valide.
 */
bool isValidExtension(const std::string &filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return false;
    std::string extension = filename.substr(dotPos);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return std::find(validExtensions.begin(), validExtensions.end(), extension) != validExtensions.end();
}

/**
 * Récupère la date de création du fichier (selon le FS).
 */
std::string getFileCreationDate(const std::string &filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) == 0)
        return std::string(ctime(&fileInfo.st_ctime));
    return "Unknown\n";
}

/**
 * Supprime toutes les métadonnées EXIF du fichier
 * et réécrit le fichier sans celles-ci.
 */
bool deleteAllExifData(const std::string &filename) {
    try {
        // Ouvrir l'image avec Exiv2
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
        if (!image.get()) {
            std::cerr << "[deleteAllExifData] Impossible d'ouvrir: " << filename << std::endl;
            return false;
        }
        image->readMetadata();

        // Effacer toutes les données EXIF
        Exiv2::ExifData &exifData = image->exifData();
        exifData.clear();

        // Écriture du fichier modifié
        image->writeMetadata();
        std::cout << "Toutes les métadonnées EXIF ont été effacées pour " << filename << std::endl;
        return true;

    } catch (Exiv2::Error& e) {
        std::cerr << "[deleteAllExifData] Exiv2 exception: " << e.what() 
                  << " pour le fichier " << filename << std::endl;
        return false;
    }
}

/**
 * Modifie (ou ajoute) une métadonnée EXIF spécifique, puis réécrit le fichier.
 * Exemple de clé: "Exif.Photo.UserComment"
 */
bool modifyExifData(const std::string &filename, const std::string &key, const std::string &newValue) {
    try {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
        if (!image.get()) {
            std::cerr << "[modifyExifData] Impossible d'ouvrir: " << filename << std::endl;
            return false;
        }
        image->readMetadata();

        Exiv2::ExifData &exifData = image->exifData();

        // On assigne la nouvelle valeur au tag EXIF
        exifData[key].setValue(newValue);

        // Réécriture
        image->writeMetadata();
        std::cout << "La métadonnée " << key << " a été mise à jour -> " 
                  << newValue << " pour " << filename << std::endl;
        return true;

    } catch (Exiv2::Error &e) {
        std::cerr << "[modifyExifData] Exiv2 error: " << e.what() 
                  << " pour le fichier " << filename << std::endl;
        return false;
    }
}

/**
 * Affiche un message d'usage.
 */
void printUsage(const char* progName) {
    std::cerr 
        << "Usage: " << progName << " [options] <image_file1> [image_file2 ...]\n"
        << "Options:\n"
        << "  -d             : effacer toutes les métadonnées EXIF.\n"
        << "  -m key=value   : modifier ou ajouter la métadonnée 'key' avec 'value'.\n"
        << "\nExemples:\n"
        << "  " << progName << " image.jpg\n"
        << "  " << progName << " -d image.jpg\n"
        << "  " << progName << " -m Exif.Photo.UserComment=\"Hello\" image.jpg\n";
}

/**
 * Vérifie la validité des arguments passés en ligne de commande.
 * On ne retourne pas false si on rencontre -d ou -m, 
 * seulement si on voit un fichier invalidExtension.
 */
bool parseArguments(int argc, char **argv, bool &deleteAll, std::string &modifyKey, std::string &modifyValue, std::vector<std::string> &files) {
    if (argc < 2) {
        printUsage(argv[0]);
        return false;
    }

    deleteAll = false;
    modifyKey.clear();
    modifyValue.clear();

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-d") {
            deleteAll = true;
        }
        else if (arg == "-m") {
            if (i + 1 < argc) {
                std::string kv = argv[++i];
                size_t eqPos = kv.find('=');
                if (eqPos == std::string::npos) {
                    std::cerr << "Erreur: format attendu pour -m est key=value\n";
                    return false;
                }
                modifyKey   = kv.substr(0, eqPos);
                modifyValue = kv.substr(eqPos + 1);
            } else {
                std::cerr << "Erreur: -m nécessite key=value\n";
                return false;
            }
        }
        else {
            if (!isValidExtension(arg)) {
                std::cerr << "Extension invalide: " << arg << std::endl;
                return false;
            }
            files.push_back(arg);
        }
    }

    // S'il n'y a aucun fichier, on sort
    if (files.empty()) {
        std::cerr << "Aucun fichier spécifié.\n";
        printUsage(argv[0]);
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    bool deleteAll = false;
    std::string modifyKey, modifyValue;
    std::vector<std::string> files;

    if (!parseArguments(argc, argv, deleteAll, modifyKey, modifyValue, files)) {
        return 1;
    }

    for (const auto &filename : files) {
        try {
            Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
            if (!image.get()) {
                std::cerr << "Error: cannot open image file " << filename << std::endl;
                continue;
            }
            // Lire les métadonnées
            image->readMetadata();
            Exiv2::ExifData &exifData = image->exifData();

            // Affichage
            std::cout << "== Fichier: " << filename << " ==\n";
            std::cout << "Date de création (FS): " << getFileCreationDate(filename);
            if (exifData.empty())
                std::cout << "Pas de métadonnées EXIF.\n";
            else {
                bool shownAny = false;
                for (const auto &exif : exifData) {
                    if (importantExifKeys.count(exif.key())) {
                        std::cout << exif.key() << " = " << exif.value() << "\n";
                        shownAny = true;
                    }
                }
                if (!shownAny)
                    std::cout << "(Aucune métadonnée EXIF “importante”)\n";
            }
            std::cout << "-------------------------\n";

        } catch (Exiv2::Error &e) {
            std::cerr << "Exiv2 Exception for " << filename 
                      << ": " << e.what() << std::endl;
        }
    }

    for (const auto &filename : files) {
        if (deleteAll) {
            deleteAllExifData(filename);
        }
        if (!modifyKey.empty()) {
            modifyExifData(filename, modifyKey, modifyValue);
        }
    }

    return 0;
}
