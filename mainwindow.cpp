#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "knowledgebase.h"

QString apiKey = qEnvironmentVariable("GROQ_API_KEY_DREAM");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    KnowledgeBase kb = KnowledgeBase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_playVideo_clicked()
{
   /* QString Filename = QFileDialog::getOpenFileName(this,tr("Select video file"),"",tr("MP4 Files(*.MP4)"));
    QMediaPlayer *player = new QMediaPlayer(this);
    QVideoWidget *video = new QVideoWidget(this);
    QAudioOutput *audio = new QAudioOutput(this);

    video ->setGeometry(20,20,640,480);
    player -> setVideoOutput(video);
    player -> setAudioOutput(audio);

    player -> setSource(QUrl(Filename));

    video -> show();
    player -> play();*/

        QMediaPlayer *player = new QMediaPlayer(this);
        QVideoWidget *video = new QVideoWidget(this);
        QAudioOutput *audio = new QAudioOutput(this);

        player->setVideoOutput(video);
        player->setAudioOutput(audio);

        video->setGeometry(20, 20, 640, 480);
        video->show();

    //QUrl url("qrc:/resources/videos/20250304_155424.mp4");
        QUrl url("https://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4");

        player->setSource(QUrl("https://interactive-examples.mdn.mozilla.net/media/cc0-videos/flower.mp4"));
        player->play();
    //player->setSource(url);
    //player->play();

        //kb.loadWordEmbeddings(R"(D:\germanWordEmbeddings\chunk_aa)");


}


void MainWindow::on_btn_groqTest_clicked()
{
    auto *manager = new QNetworkAccessManager(this);

    QUrl url("https://api.groq.com/openai/v1/chat/completions");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");

    QByteArray authHeader = "Bearer " + apiKey.toUtf8();
    request.setRawHeader("Authorization", authHeader);

    QJsonObject message;
    message["role"] = "user";
    message["content"] = "Hallo, wie gehts wie stehts?!";

    QJsonArray messages;
    messages.append(message);

    QJsonObject payload;
    payload["model"] = "llama-3.3-70b-versatile";
    payload["messages"] = messages;

    QNetworkReply *reply = manager->post(
        request,
        QJsonDocument(payload).toJson()
        );

    connect(reply, &QNetworkReply::finished, this, [reply, this]()
            {
                QByteArray data = reply->readAll();

                qDebug() << "RAW RESPONSE:" << data;

                if (reply->error() != QNetworkReply::NoError)
                {
                    auto *msg = new QMessageBox(this);
                    msg->setAttribute(Qt::WA_DeleteOnClose);
                    msg->setWindowTitle("Network Error");
                    msg->setText(reply->errorString() + "\n\n" + QString(data));
                    msg->show();

                    reply->deleteLater();
                    return;
                }

                QJsonParseError jsonError;
                QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);

                if (jsonError.error != QJsonParseError::NoError)
                {
                    auto *msg = new QMessageBox(this);
                    msg->setAttribute(Qt::WA_DeleteOnClose);
                    msg->setWindowTitle("JSON Parse Error");
                    msg->setText(jsonError.errorString() + "\n\nRaw:\n" + QString(data));
                    msg->show();

                    reply->deleteLater();
                    return;
                }

                QJsonObject root = doc.object();
                QJsonArray choices = root["choices"].toArray();

                if (choices.isEmpty())
                {
                    auto *msg = new QMessageBox(this);
                    msg->setAttribute(Qt::WA_DeleteOnClose);
                    msg->setWindowTitle("API Error");
                    msg->setText("No response choices found:\n" + QString(data));
                    msg->show();

                    reply->deleteLater();
                    return;
                }

                QString answer =
                    choices.first().toObject()
                        ["message"].toObject()
                                ["content"].toString();

                auto *msg = new QMessageBox(this);
                msg->setAttribute(Qt::WA_DeleteOnClose);
                msg->setWindowTitle("Groq Antwort");
                msg->setText(answer);
                msg->show();

                reply->deleteLater();
            });
}

