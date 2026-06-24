#include "llm_manager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

llm_manager::llm_manager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    basePrompt = loadPrompt(promptPath);
}

string llm_manager::loadPrompt(const string &path)
{
    string temp = "";
    QFile file(QString::fromStdString(path));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open resource:" << path;
        return temp;
    }

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();

        temp += QString::fromUtf8(line).toStdString();
    }

    return temp;

}

string llm_manager::replaceSubString(string target ,string fillerMarker, string filler)
{
    size_t pos = target.find(fillerMarker);

    if (pos != std::string::npos)
    {
        target.replace(pos, fillerMarker.length(), filler);
    }

    return target;
}

string llm_manager::generatePrompt(string transcript,string knowledgeBaseEntries)
{
    string finalPrompt = basePrompt;
    finalPrompt = replaceSubString(finalPrompt,"[!!!Transkript!!!]",transcript);
    finalPrompt = replaceSubString(finalPrompt,"[!!!Datenbankausgaben!!!]",knowledgeBaseEntries);
    cout << finalPrompt << endl;
    return finalPrompt;
}

void llm_manager::queryLLM(const QString &user_query)
{

    QUrl url("https://api.groq.com/openai/v1/chat/completions");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");

    QByteArray authHeader = "Bearer " + apiKey.toUtf8();
    request.setRawHeader("Authorization", authHeader);

    QJsonObject message;
    message["role"] = "user";
    message["content"] = user_query;

    QJsonArray messages;
    messages.append(message);

    QJsonObject payload;
    payload["model"] = "llama-3.3-70b-versatile";
    payload["messages"] = messages;

    QNetworkReply *reply = manager->post(
        request,
        QJsonDocument(payload).toJson()
        );

    connect(reply, &QNetworkReply::finished, this, [this, reply]()
            {
                QByteArray data = reply->readAll();

                if (reply->error() != QNetworkReply::NoError)
                {
                    emit answerReady("Network error: " + reply->errorString());
                    reply->deleteLater();
                    return;
                }

                QJsonDocument doc = QJsonDocument::fromJson(data);
                QJsonObject root = doc.object();
                QJsonArray choices = root["choices"].toArray();

                if (choices.isEmpty())
                {
                    emit answerReady("No response from API");
                    reply->deleteLater();
                    return;
                }

                QString answer =
                    choices.first().toObject()
                        ["message"].toObject()
                                ["content"].toString();

                emit answerReady(answer);

                reply->deleteLater();
            });
}