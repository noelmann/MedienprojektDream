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