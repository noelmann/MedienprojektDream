#include "knowledgebase.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <array>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <set>
#include <cctype>
#include <QString>
#include <locale>
#include <QFile>
#include <QDebug>
#include <QByteArray>

using namespace std;




KnowledgeBase::KnowledgeBase()
{
    locale::global(std::locale(""));
    cout.imbue(std::locale());
    embeddings = loadWordEmbeddings(embeddingsPath);
    sentences = loadSentences(sentencesPath);
    sentences_embeddings = generateSentenceEmbeddings(sentences);
    //printSentenceEmbeddings();
    cout << "done loading" << endl;
    cout << embeddingsPath << endl;

    testResponseRanking();

}

string KnowledgeBase::removePunctuation(string &s)
{
    string temp = "";
    for(int i =0;i<s.length();i++)
    {
        if(!ispunct(s[i]))
        {
            temp+=s[i];
        }
    }

    return temp;
}

void KnowledgeBase::printSentenceEmbeddings()
{
    for(const auto &pair : sentences_embeddings)
    {
        cout << pair.first << endl;

        for(int i = 0;i<pair.second.size() ;i++)
        {
            cout << pair.second[i] << " ";
        }
        cout << endl;
    }
}

void KnowledgeBase::testResponseRanking()
{
    vector<responseScorePair> rps;
    string line = "Was ist das Projekt überhaupt und wie hilft es den kleinen und mittelständischen Unternehmen?";
    cout << "Please enter sentence for responseRetrieval:" << line << endl;

    QString q = QString::fromUtf8(line.c_str()).toLower();
    line = q.toUtf8().toStdString();

    rps = searchKnowledgeBase(line,25);
    for(const auto &rp : rps)
    {
        cout<< rp.response + "| Score:" + to_string(rp.score)<<endl;
    }

    ofstream MyFile("responses.txt");
    for(const auto &rp : rps)
    {
        string r = rp.response + "| Score:" + to_string(rp.score);
        MyFile << r <<endl;
    }

    MyFile.close();
}

double KnowledgeBase::calculateMagnitude(array<double,300> a)
{
    double magnitude = 0.0;
    for(int i = 0;i<a.size();i++)
    {
        magnitude+=pow(a[i],2);
    }

    return sqrt(magnitude);
}
double KnowledgeBase::calculateDotProduct(array<double,300> a,array<double,300> b)
{
    double dotProduct = 0.0;
    for(int i = 0;i<a.size();i++)
    {
        dotProduct+=(a[i]*b[i]);
    }

    return dotProduct;
}

array<double,300> KnowledgeBase::generateSentenceEmbedding(string s)
{
    array<double,300> vec = {0.0};
    string word;
    int wordCount = 0;
    stringstream ss(s);
    while(ss >> word)
    {
        if(embeddings.count((word)) >= 1)
        {
            vec = addEmbeddings(vec,embeddings[word]);
            wordCount++;
        }
    }

    if (wordCount == 0)
        return vec;

    return normalizeSentenceEmbedding(vec,wordCount);
}

double KnowledgeBase::calculateSemanticSimilarity(string s1, string s2)
{
    array<double,300> s1_embedding = {0.0};
    array<double,300> s2_embedding = {0.0};
    if(sentences_embeddings.count(s1) >= 1)
    {
        s1_embedding = sentences_embeddings[s1];
    }
    else
    {
        s1_embedding = generateSentenceEmbedding(s1);
    }

    if(sentences_embeddings.count(s2) >= 1)
    {
        s2_embedding = sentences_embeddings[s2];
    }
    else
    {
        s2_embedding = generateSentenceEmbedding(s2);
    }

    double m1 =calculateMagnitude(s1_embedding);
    double m2 =calculateMagnitude(s2_embedding);

    if (m1 == 0.0 || m2 == 0.0)
        return 0.0;

    return calculateDotProduct(s1_embedding,s2_embedding)/(m1*m2);

}

/*vector<responseScorePair> KnowledgeBase::searchKnowledgeBase(string user_query, int responses)
{
    map<double,string> responseScores;
    set<double,greater<double>> scores;
    double similarity;
    for(const string &s : sentences)
    {

        similarity = calculateSemanticSimilarity(user_query,s);
        responseScores[similarity]=s;
        scores.insert(similarity);
    }

    if(responses > scores.size())
    {
        responses = scores.size();
    }

    int c = 1;

    vector<responseScorePair> rps;
    for(const double &score : scores)
    {
        if(c<responses)
        {
            responseScorePair rp;
            rp.response = responseScores[score];
            rp.score = score;
            rps.emplace_back(rp);
            c++;
        }
        else
        {
            break;
        }
    }

    return rps;
}*/

vector<responseScorePair> KnowledgeBase::searchKnowledgeBase(string user_query, int responses)
{
    user_query = removePunctuation(user_query);
    transform(user_query.begin(), user_query.end(), user_query.begin(),::tolower);
    vector<responseScorePair> rps;
    rps.reserve(sentences.size());

    for (const string &s : sentences)
    {
        responseScorePair rp;
        rp.response = s;
        rp.score = calculateSemanticSimilarity(user_query, s);

        rps.push_back(rp);
    }

    sort(rps.begin(), rps.end(),[](const responseScorePair &a, const responseScorePair &b)
    {
        return a.score > b.score;
    });

    if (responses < rps.size())
        rps.resize(responses);

    return rps;
}


array<double,300> KnowledgeBase::addEmbeddings(array<double,300> a,array<double,300> b)
{
    array<double,300> c;
    for(int i =0;i<a.size();i++)
    {
        c[i]=a[i]+b[i];
    }
    return c;
}

array<double,300> KnowledgeBase::normalizeSentenceEmbedding(array<double,300> a, int length)
{
    for(int i =0;i<a.size();i++)
    {
        a[i]/=length;
    }
    return a;
}

/*unordered_map<string, array<double,300>> KnowledgeBase::loadWordEmbeddings(string path)
{
    ifstream embeddingFile(path);
    int loadcount = 20000;
    unordered_map<string,array<double,300>> embeddings;
    embeddings.reserve(loadcount);
    string word;
    array<double,300> vec;


    int c=0;

    while (c < loadcount && (embeddingFile >> word))
    {
        for (double &value : vec)
        {
            if (!(embeddingFile >> value))
            {
                cerr << "Error reading embedding for " << word << endl;
                return embeddings;
            }
        }

        embeddings.emplace(std::move(word), vec);
        ++c;
    }
     cout << "loading completed" << endl;

    return embeddings;
}*/


unordered_map<string, array<double,300>> KnowledgeBase::loadWordEmbeddings(const string& path)
{
    ifstream file(path);

    const int loadcount = 100000;

    unordered_map<string, array<double,300>> embeddings;
    embeddings.reserve(loadcount);

    string line;
    int c = 0;

    while (c < loadcount && getline(file, line))
    {
        const char* p = line.c_str();

        // Read word
        while (*p == ' ') ++p;
        const char* start = p;
        while (*p && *p != ' ') ++p;

        string word(start, p - start);

        array<double,300> vec;

        for (int i = 0; i < 300; ++i)
        {
            while (*p == ' ') ++p;
            vec[i] = strtod(p, (char**)&p);
        }

        embeddings.emplace(move(word), move(vec));
        ++c;
    }

    file.close();


    return embeddings;
}


vector<string> KnowledgeBase::loadSentences(const string &path)
{
    QFile file(QString::fromStdString(path));
    vector<string> sentences;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open resource:" << path;
        return sentences;
    }

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();

        QString q = QString::fromUtf8(line).trimmed().toLower();

        string sentence = q.toUtf8().toStdString();
        sentence = removePunctuation(sentence);

        sentences.emplace_back(move(sentence));
    }

    return sentences;
}

unordered_map<string, array<double,300>> KnowledgeBase::generateSentenceEmbeddings(vector<string> s)
{
    unordered_map<string,array<double,300>> sentenceEmbeddings;

    for(const auto &sentence : s)
    {
        sentenceEmbeddings.emplace(sentence,generateSentenceEmbedding(sentence));
    }
    return sentenceEmbeddings;
}