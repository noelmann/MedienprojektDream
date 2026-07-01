#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <QCoreApplication>

using namespace std;


struct responseScorePair
{
    string response;
    double score;
};

class KnowledgeBase
{
public:
    KnowledgeBase();
    //unordered_map<string, array<double,300>> loadWordEmbeddings(string path);
    unordered_map<string, array<double,300>> loadWordEmbeddings(const string &path);
    vector<responseScorePair> searchKnowledgeBase(string user_query, int responses);

private:
    vector<string> sentences;
    unordered_map<string,array<double,300>> sentences_embeddings;
    unordered_map<string, array<double,300>> embeddings;
    const string embeddingsPath = (QCoreApplication::applicationDirPath() + "/" +  R"(DE_wordEmbeddings200K_300d_fasttext)").toStdString();
    //const string embeddingsPath = ":/DE_wordEmbeddings200K_300d_fasttext";
    const string sentencesPath = ":/RAG-Knowledge.txt";
    vector<string> loadSentences(const string &path);
    array<double,300> generateSentenceEmbedding(string s);
    unordered_map<string, array<double,300>> generateSentenceEmbeddings(vector<string> s);
    array<double,300> addEmbeddings(array<double,300> a,array<double,300> b);
    array<double,300> normalizeSentenceEmbedding(array<double,300> a, int length);
    double calculateSemanticSimilarity(string s1, string s2);

    void printSentenceEmbeddings();
    void testResponseRanking();


};

#endif // KNOWLEDGEBASE_H
