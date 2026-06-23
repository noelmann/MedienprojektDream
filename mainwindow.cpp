#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "knowledgebase.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QDir>
#include <QCoreApplication>
#include <QUrl>

QString apiKey = qEnvironmentVariable("GROQ_API_KEY_DREAM");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    videoWidget = new QVideoWidget(this);

    player->setAudioOutput(audioOutput);
    player->setVideoOutput(videoWidget);

    auto layout = new QVBoxLayout(ui->videoWidgetContainer);

    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(videoWidget);

    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);

    audioOutput->setVolume(0.5);

    connect(ui->playButton,
            &QPushButton::clicked,
            player,
            &QMediaPlayer::play);

    connect(ui->pauseButton,
            &QPushButton::clicked,
            player,
            &QMediaPlayer::pause);

    connect(ui->stopButton,
            &QPushButton::clicked,
            player,
            &QMediaPlayer::stop);

    connect(ui->volumeSlider,
            &QSlider::valueChanged,
            this,
            [this](int value)
            {
                audioOutput->setVolume(value / 100.0);
            });

    connect(player,
            &QMediaPlayer::positionChanged,
            this,
            [this](qint64 pos)
            {
                ui->timelineSlider->setValue(pos);
            });

    connect(player,
            &QMediaPlayer::durationChanged,
            this,
            [this](qint64 duration)
            {
                ui->timelineSlider->setMaximum(duration);
            });

    connect(ui->timelineSlider,
            &QSlider::sliderMoved,
            player,
            &QMediaPlayer::setPosition);


    connect(ui->videoListWidget,
            &QListWidget::itemClicked,
            this,
            &MainWindow::loadSelectedVideo);


    //Todo: auto load item names from video directory
    ui->videoListWidget->addItem("testA.mp4");
    ui->videoListWidget->addItem("testB.mp4");
    ui->videoListWidget->addItem("testC.mp4");

    connect(ui->sendButton,
            &QPushButton::clicked,
            this,
            &MainWindow::sendMessage);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage()
{
    QString prompt = ui->chatInput->toPlainText();

    if (prompt.isEmpty())
        return;


    ui->chatInput->clear();

    //Todo: replace test code with llm call
    ui->chatHistory->append("You: " + prompt);
    ui->chatHistory->append("Response:"+QString::fromStdString(kb.searchKnowledgeBase(prompt.toStdString(),1)[0].response));

}

void MainWindow::loadSelectedVideo(QListWidgetItem *item)
{
    QString filename = item->text();

    QString path =
        QCoreApplication::applicationDirPath()
        + "/videos/" + filename;

    player->setSource(QUrl::fromLocalFile(path));
    player->play();
}

