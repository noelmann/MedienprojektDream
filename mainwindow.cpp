#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_playVideo_clicked()
{
    QString Filename = QFileDialog::getOpenFileName(this,tr("Select video file"),"",tr("MP4 Files(*.MP4)"));
    QMediaPlayer *player = new QMediaPlayer(this);
    QVideoWidget *video = new QVideoWidget(this);
    QAudioOutput *audio = new QAudioOutput(this);

    video ->setGeometry(20,20,640,480);
    player -> setVideoOutput(video);
    player -> setAudioOutput(audio);

    player -> setSource(QUrl(Filename));

    video -> show();
    player -> play();
}


void MainWindow::on_btn_groqTest_clicked()
{
    QString apiKey = qEnvironmentVariable("GROQ_API_KEY");
    auto *manager = new QNetworkAccessManager(this);
    QMessageBox::information(
        nullptr,
        "Groq API Key",
        apiKey
        );

    QUrl url("https://api.groq.com/openai/v1/chat/completions");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json");

    request.setRawHeader(
        "Authorization",
        "Bearer "+apiKey.toUtf8()
        );

    QJsonObject message;
    message["role"] = "user";
    message["content"] = "Hallo, wie gehts wie stehts?!";

    QJsonArray messages;
    messages.append(message);

    QJsonObject payload;
    payload["model"] = "llama-3.3-70b-versatile";
    payload["messages"] = messages;

    QNetworkReply *reply =
        manager->post(
            request,
            QJsonDocument(payload).toJson()
            );

    connect(reply, &QNetworkReply::finished,
            this,
            [reply]()
            {
                if(reply->error())
                {
                    QMessageBox::critical(
                        nullptr,
                        "Fehler",
                        reply->errorString()
                        );

                    reply->deleteLater();
                    return;
                }

                QByteArray data = reply->readAll();

                QJsonDocument doc =
                    QJsonDocument::fromJson(data);

                QString answer =
                    doc.object()
                        ["choices"]
                            .toArray()
                            .first()
                            .toObject()
                                ["message"]
                            .toObject()
                                ["content"]
                            .toString();

                QMessageBox::information(
                    nullptr,
                    "Groq Antwort",
                    answer
                    );

                reply->deleteLater();
            });
}

