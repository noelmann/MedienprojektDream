#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "knowledgebase.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QDir>
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkAccessManager>



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
    //ui->videoListWidget->addItem("VerteiltOderDezentral.mp4");
    //ui->videoListWidget->addItem("testB.mp4");

    QStringList l = HelperFunctions::getVideonames();
    for(const QString& s : l)
    {
        ui->videoListWidget->addItem(s);
    }

    connect(ui->sendButton,
            &QPushButton::clicked,
            this,
            &MainWindow::sendMessage);

    llm = new llm_manager(this);

    connect(llm, &llm_manager::answerReady,
            this, [this](const QString &text)
            {
                ui->chatHistory->append("[LLM]: " + text + "\n");
            });


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage()
{
    QString input = ui->chatInput->toPlainText();

    if (input.isEmpty())
        return;


    ui->chatInput->clear();

    ui->chatHistory->append("[User]: " + input+"\n");
    //ui->chatHistory->append("Response:"+QString::fromStdString(kb.searchKnowledgeBase(prompt.toStdString(),1)[0].response));


    string kbEntries = "";
    vector<responseScorePair> rps = kb.searchKnowledgeBase(input.toStdString(),50);
    for(const auto & rp : rps)
    {
        kbEntries += rp.response + "\n";
    }

    string prompt = llm -> generatePrompt(ui->chatHistory->toPlainText().toStdString(),kbEntries);
    llm ->queryLLM(QString::fromStdString(prompt));

}

void MainWindow::loadSelectedVideo(QListWidgetItem *item)
{
    string filename = item->text().toStdString();

    QString path = QString::fromStdString(HelperFunctions::getVideosPath() + "/" + filename);

    player->setSource(QUrl::fromLocalFile(path));
    player->play();
}

