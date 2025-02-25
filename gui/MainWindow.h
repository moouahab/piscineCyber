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
    void saveMetadata();

private:
    QTableWidget *metadataTable;
    QPushButton *saveButton; 
    QString currentFilePath;
};

#endif
