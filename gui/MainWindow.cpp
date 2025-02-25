#include "MainWindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QHeaderView>
#include <exiv2/exiv2.hpp>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Interface
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QPushButton *openButton = new QPushButton("Ouvrir une image", this);
    metadataTable = new QTableWidget(this);
    QPushButton *clearButton = new QPushButton("Effacer les métadonnées", this);
    saveButton = new QPushButton("Enregistrer", this);
    saveButton->setEnabled(false);  // Désactiver jusqu'à ce qu'une image soit ouverte

    layout->addWidget(openButton);
    layout->addWidget(metadataTable);
    layout->addWidget(clearButton);
    layout->addWidget(saveButton);
    setCentralWidget(centralWidget);

    connect(openButton, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearMetadata);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveMetadata);
}

void MainWindow::openImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Choisir une image", "", "Images (*.jpg *.jpeg *.png *.bmp *.tiff)");
    if (filePath.isEmpty()) return;

    currentFilePath = filePath;
    saveButton->setEnabled(true);

    try {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filePath.toStdString());
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();

        metadataTable->setRowCount(static_cast<int>(exifData.count()));
        metadataTable->setColumnCount(2);
        metadataTable->setHorizontalHeaderLabels({"Clé", "Valeur"});

        int row = 0;
        for (const auto &exif : exifData) {
            metadataTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(exif.key())));
            metadataTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(exif.value().toString())));
            metadataTable->item(row, 1)->setFlags(metadataTable->item(row, 1)->flags() | Qt::ItemIsEditable);
            row++;
        }

    } catch (Exiv2::Error &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}


void MainWindow::saveMetadata() {
    if (currentFilePath.isEmpty()) return;

    try {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(currentFilePath.toStdString());
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();

        for (int row = 0; row < metadataTable->rowCount(); ++row) {
            QString key = metadataTable->item(row, 0)->text();
            QString value = metadataTable->item(row, 1)->text();

            auto it = exifData.findKey(Exiv2::ExifKey(key.toStdString()));
            if (it != exifData.end()) {
                it->setValue(value.toStdString());
            }
        }

        image->writeMetadata();
        std::cout << "Les métadonnées ont été modifiées et enregistrées.\n";

    } catch (Exiv2::Error &e) {
        std::cerr << "Erreur lors de l'enregistrement : " << e.what() << std::endl;
    }
}


void MainWindow::clearMetadata() {
    metadataTable->clear();
}
