#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QAudioOutput>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QListWidgetItem>
#include "knowledgebase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadSelectedVideo(QListWidgetItem *item);
    void sendMessage();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    QVideoWidget* videoWidget;
    KnowledgeBase kb;

};
#endif // MAINWINDOW_H
