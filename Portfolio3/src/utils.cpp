/*
 * utils.cpp
 *
 *  Created on: Jan 21, 2011
 *      Author: rbh
 */
// general purpose routines moved from classes(.h & .cpp)

// v1.0
// declarations moved from classes.cpp
// 21/1/2011
//

#ifndef UTILS_C_
#define UTILS_C_

#include <stack>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <cctype>

#include "../headers/utilities/utils.h"

using std::string;
namespace xmlutils
{
    
    
    bool validChars(const std::string & toCheck, const std::string & allowed)
    {
        return (toCheck.find_first_not_of(allowed)==std::string::npos);
    }
    
    bool noDups(const std::string & st)
    {
        return (adjacent_find(st.begin(), st.end()) == st.end());
    }
    
    string removeDigitsFrom(string copySource)
    {
        copySource.erase(remove_if(copySource.begin(), copySource.end(), isdigit), copySource.end());
        return copySource;
    }
    
    string removeRepeatingCharsFrom(string copySource)
    {
        copySource.erase(unique(copySource.begin(), copySource.end()), copySource.end());
        return copySource;
    }
    
    string basicHeader()
    {
        string  st;
        st = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        st += "<gpx version=\"1.0\"	creator=\"GPXP 2011 (http://warren.ntu.ac.uk/projects/soft20111/GPXP)\" ";
        st += "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
        st += "xmlns=\"http://www.topografix.com/GPX/1/0\" ";
        st += "xmlns:topografix=\"http://www.topografix.com/GPX/Private/TopoGrafix/0/2\" ";
        st += "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd ";
        st += "http://www.topografix.com/GPX/Private/TopoGrafix/0/2 http://www.topografix.com/GPX/Private/TopoGrafix/0/2/topografix.xsd\">";
        return st;
    }
    
    void openTag(string & dest, string tagName, string tagParameters, string tagContent, std::stack<string> & closingTags)
    {
        string closingTag;
        closingTag.reserve(3 + tagName.length());
        closingTag = "</";
        closingTag += tagName;
        closingTag += ">";
        closingTags.push(closingTag);
        dest += "<";
        dest += tagName;
        dest += " ";
        dest += tagParameters;
        dest += ">";
        dest += tagContent;
    }
    
    void openCloseTagAndValue(string & dest, const string & tagName, const string & tagParameters, const string & tagContent)
    {
        string closingTag;
        closingTag.reserve(3 + tagName.length());
        closingTag = "</";
        closingTag += tagName;
        closingTag += ">";
        
        dest += "<";
        dest += tagName;
        dest +=  tagParameters.empty()?"":(string(" ")+tagParameters);
        dest += tagParameters;
        dest += ">";
        dest += tagContent;
        dest += closingTag;
    }
    
    std::string timeAsString(unsigned currTime)
    {
        std::string st;
        std::ostringstream oss(st);
        oss << currTime;
        return (oss.str());
    }
    
    unsigned timeFromString (const std::string & timeStr)
    {
        unsigned currTime;
        std::istringstream iss(timeStr);
        iss >> currTime;
        return (currTime);
        
    }
    
} //namespace xmlutils
#endif /* UTILS_C_ */

