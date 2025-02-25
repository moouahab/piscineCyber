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

// Clés EXIF importantes
const std::set<std::string> importantExifKeys = {
    "Exif.Photo.DateTimeOriginal",
    "Exif.Image.Make",
    "Exif.Image.Model",
    "Exif.Photo.FNumber",
    "Exif.Photo.ShutterSpeedValue",
    "Exif.Photo.ISOSpeedRatings",
    "Exif.GPSInfo.GPSLatitude",
    "Exif.GPSInfo.GPSLongitude"
};

/**
 * Vérifie si l’extension du fichier est valide
 */
bool isValidExtension(const std::string &filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return false;

    std::string extension = filename.substr(dotPos);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    return std::find(validExtensions.begin(), validExtensions.end(), extension) != validExtensions.end();
}

/**
 * Récupère la date de création du fichier
 */
std::string getFileCreationDate(const std::string &filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) == 0) {
        return ctime(&fileInfo.st_ctime); // Convertir le timestamp en string
    }
    return "Unknown";
}

/**
 * Vérifie la validité des arguments passés en ligne de commande.
 */
bool parseArguments(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_file1> [image_file2 ...]" << std::endl;
        return false;
    }
    for (int i = 1; i < argc; ++i) {
        std::string filename = argv[i];
        if (!isValidExtension(filename)) {
            std::cerr << "Invalid file extension: " << filename << std::endl;
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (!parseArguments(argc, argv))
        return 1;

    std::map<std::string, Exiv2::ExifData> images;
    std::map<std::string, std::string> fileCreationDates; // Stocke les dates de création

    for (int i = 1; i < argc; ++i) {
        std::string filename = argv[i];

        try {
            Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
            if (!image.get()) {
                std::cerr << "Error: cannot open image file " << filename << std::endl;
                continue;
            }

            
            // Lire les métadonnées
            image->readMetadata();
            Exiv2::ExifData &exifData = image->exifData();

            // Vérifier si des métadonnées sont présentes
            if (exifData.empty()) {
                std::cerr << "Warning: no EXIF data in " << filename << std::endl;
                continue;
            }

            // Stocker les métadonnées et la date de création du fichier
            images[filename] = exifData;
            fileCreationDates[filename] = getFileCreationDate(filename);

        } catch (Exiv2::Error &e) {
            std::cerr << "Exiv2 Exception for " << filename << ": " << e.what() << std::endl;
        }
    }

    // Vérifier si on a au moins une image traitée
    if (images.empty()) {
        std::cerr << "No valid images processed." << std::endl;
        return 1;
    }

    // Affichage des métadonnées pour chaque image
    for (const auto &entry : images) {
        const std::string &fileName = entry.first;
        const Exiv2::ExifData &exifData = entry.second;

        std::cout << "Metadata for: " << fileName << std::endl;
        std::cout << "Creation Date: " << fileCreationDates[fileName];
        std::cout << "--------------------------------------" << std::endl;

        bool hasMetadata = false; // Vérifie si au moins une métadonnée est affichée

        for (const auto &exif : exifData) {
            if (importantExifKeys.count(exif.key())) {
                std::cout << exif.key() << " = " << exif.value() << std::endl;
                hasMetadata = true;
            }
        }

        if (!hasMetadata) {
            std::cout << "No important metadata found." << std::endl;
        }

        std::cout << "--------------------------------------" << std::endl;
    }

    return 0;
}
