#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <array>
#include <string>
#include <cmath>
#include <QCoreApplication>

using namespace std;

class HelperFunctions
{
public:
    HelperFunctions();
    static double calculateDotProduct(array<double,300> a,array<double,300> b);
    static double calculateMagnitude(array<double,300> a);
    static string removePunctuation(string &s);
    static string replaceSubString(string target ,string fillerMarker, string filler);
    static string getSentencesPath();
    static string getEmbeddingsPath();
    static string getPromptPath();
};

#endif // HELPERFUNCTIONS_H
