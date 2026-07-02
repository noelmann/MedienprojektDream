#include "helperfunctions.h"

HelperFunctions::HelperFunctions()
{

}

double HelperFunctions::calculateMagnitude(array<double,300> a)
{
    double magnitude = 0.0;
    for(int i = 0;i<a.size();i++)
    {
        magnitude+=pow(a[i],2);
    }

    return sqrt(magnitude);
}
double HelperFunctions::calculateDotProduct(array<double,300> a,array<double,300> b)
{
    double dotProduct = 0.0;
    for(int i = 0;i<a.size();i++)
    {
        dotProduct+=(a[i]*b[i]);
    }

    return dotProduct;
}

string HelperFunctions::removePunctuation(string &s)
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

string HelperFunctions::replaceSubString(string target ,string fillerMarker, string filler)
{
    int pos = target.find(fillerMarker);

    if (pos != std::string::npos)
    {
        target.replace(pos, fillerMarker.length(), filler);
    }

    return target;
}

string HelperFunctions::getSentencesPath()
{
    return ":/RAG-Knowledge.txt";
}
string HelperFunctions::getEmbeddingsPath()
{
    return (QCoreApplication::applicationDirPath() + "/" +  R"(DE_wordEmbeddings100K_300d_fasttext)").toStdString();
}
string  HelperFunctions::getPromptPath()
{
    return ":/llm_prompt.txt";
}

string HelperFunctions::getVideosPath()
{
    return (QCoreApplication::applicationDirPath() + "/" +  "videos").toStdString();
}

QStringList HelperFunctions::getVideonames()
{
    QDir directory(QString::fromStdString(getVideosPath()));
    QStringList videoNames = directory.entryList(QStringList() << "*.mp4" << "*.MP4",QDir::Files);

    return videoNames;
}