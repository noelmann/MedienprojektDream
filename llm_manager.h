#ifndef LLM_MANAGER_H
#define LLM_MANAGER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <iostream>
#include "helperfunctions.h"

using namespace std;

class llm_manager : public QObject
{
    Q_OBJECT

public:
    explicit llm_manager(QObject *parent = nullptr);

    string generatePrompt(string transcript,string knowledgeBaseEntries);
    void queryLLM(const QString &user_query);

signals:
    void answerReady(const QString &text);

private:
    QString apiKey = qEnvironmentVariable("GROQ_API_KEY_DREAM");
    string loadPrompt(const string &path);
    string basePrompt;
    QNetworkAccessManager *manager;
};

#endif