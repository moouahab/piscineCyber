#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QString>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void openImage();
    void clearMetadata();
    void saveMetadata();  // Nouvelle fonction pour enregistrer les changements

private:
    QTableWidget *metadataTable;
    QPushButton *saveButton;  // Bouton pour sauvegarder
    QString currentFilePath;  // Stocker le fichier en cours d'édition
};

#endif
