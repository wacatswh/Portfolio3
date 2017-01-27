/*
 * utils.h
 *
 *  Created on: Jan 21, 2011
 *      Author: rbh
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <stack>

//#include "classes.h"


using std::string;

double getVal(std::string, double, double);

namespace xmlutils
{
    
    
    bool validChars(const std::string & toCheck, const std::string & allowed);
    
    bool noDups(const std::string &);
    
    string removeDigitsFrom(string source);
    
    string removeRepeatingCharsFrom(string source);
    
    string basicHeader();
    
    void openTag(string & dest, string tagName, string tagParameters, string tagContent, std::stack<string> & closingTags);
    
    void openCloseTagAndValue(string & dest, const string & tagName, const string & tagParameters, const string & tagContent);
    
    std::string timeAsString(unsigned currTime);
    
    unsigned timeFromString (const std::string & timeStr);
    
} // namespace xmlutils


#endif /* UTILS_H_ */
