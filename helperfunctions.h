#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <array>
#include <string>
#include <cmath>

using namespace std;

class HelperFunctions
{
public:
    HelperFunctions();
    static double calculateDotProduct(array<double,300> a,array<double,300> b);
    static double calculateMagnitude(array<double,300> a);
    static string removePunctuation(string &s);
    static string replaceSubString(string target ,string fillerMarker, string filler);
};

#endif // HELPERFUNCTIONS_H
