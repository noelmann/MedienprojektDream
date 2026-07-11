#include "helperfunctions.h"

HelperFunctions::HelperFunctions()
{

}

//calculates the length of a vector
double HelperFunctions::calculateMagnitude(array<double,300> a)
{
    double magnitude = 0.0;
    for(int i = 0;i<a.size();i++)
    {
        magnitude+=pow(a[i],2);
    }

    return sqrt(magnitude);
}

//calculates the dot product of two vectors
double HelperFunctions::calculateDotProduct(array<double,300> a,array<double,300> b)
{
    double dotProduct = 0.0;
    for(int i = 0;i<a.size();i++)
    {
        dotProduct+=(a[i]*b[i]);
    }

    return dotProduct;
}
//removes the punctation of a string
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

//replaces a section in a string(used for passing the conversation history in the prompt to the llm)
string HelperFunctions::replaceSubString(string target ,string fillerMarker, string filler)
{
    int pos = target.find(fillerMarker);

    if (pos != std::string::npos)
    {
        target.replace(pos, fillerMarker.length(), filler);
    }

    return target;
}

//returns the path to the knowledge file(currently embedded in the executable) used for the llm
string HelperFunctions::getSentencesPath()
{
    return ":/RAG-Knowledge.txt";
}

//returns the path to the file containing the word embeddings that are necessary for calculating the semantic similarity
string HelperFunctions::getEmbeddingsPath()
{
    return (QCoreApplication::applicationDirPath() + "/" +  R"(DE_wordEmbeddings100K_300d_fasttext)").toStdString();
}

//returns the prompt template
string  HelperFunctions::getPromptPath()
{
    return ":/llm_prompt.txt";
}

//returns the path to stored videos that are displayed in the video player and creates the empty folder if it does not already exist
string HelperFunctions::getVideosPath()
{
    const QString vPath = QString::fromStdString(QCoreApplication::applicationDirPath().toStdString() + "/" + "videos");

    QDir dir(vPath);
    if(!dir.exists())
    {
        dir.mkdir(".");
    }

    return vPath.toStdString();
}

//returns the video file names
QStringList HelperFunctions::getVideonames()
{
    QDir directory(QString::fromStdString(getVideosPath()));
    QStringList videoNames = directory.entryList(QStringList() << "*.mp4" << "*.MP4",QDir::Files);

    return videoNames;
}