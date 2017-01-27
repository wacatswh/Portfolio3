/*
 * parser.cpp
 *
 *  Created on: Jan 30, 2011
 *      Author: rbh
 */
/*
 \
 */

#ifndef PARSER_C_
#define PARSER_C_

#include "../headers/classes/parser.h"
#include <iostream>

// namespace gpxParser

namespace gpxParser
{
    
    bool tagExists(const std::string & sauce, const std::string & tagName)
    {
        size_t start =sauce.find("<"+tagName);
        return((start !=std::string::npos) && (std::string(" >").find(sauce[start+1+tagName.length()])!=std::string::npos)); //2nd test is whether char after the tag is ' ' or '>'
    }
    
    std::string getWholeElement(const std::string & sauce,  const std::string & tagName)
    {
        if(!tagExists(sauce, tagName))
            return std::string("");
        size_t startOfOpeningTag, endOfOpeningTag,startOfClosingTag, endOfClosingTag;
        startOfOpeningTag = sauce.find("<"+tagName);
        endOfOpeningTag = sauce.find(">", startOfOpeningTag);
        if (sauce[endOfOpeningTag-2]=='/')   //is of form <tagName /> so no value
            return sauce.substr(startOfOpeningTag, endOfOpeningTag-startOfOpeningTag);
        startOfClosingTag =  sauce.find("</"+tagName+">", endOfOpeningTag);
        endOfClosingTag=  sauce.find(">", startOfClosingTag)+1;
        return sauce.substr(startOfOpeningTag, endOfClosingTag-startOfOpeningTag);
    }
    
    std::string getElementValue(std::string & sauce, const std::string & tagName)
    
    {
        if(!tagExists(sauce, tagName))
            return std::string("");
        size_t startOfOpeningTag, endOfOpeningTag,startOfClosingTag, endOfClosingTag;
        startOfOpeningTag = sauce.find("<"+tagName);
        endOfOpeningTag = sauce.find(">", startOfOpeningTag)+1;
        if (sauce[endOfOpeningTag-1]=='/')   //is of form <tagName /> so no value
            return std::string("");
        startOfClosingTag =  sauce.find("</"+tagName +">", endOfOpeningTag);
        endOfClosingTag=  sauce.find(">", startOfClosingTag)+1;
        return sauce.substr(endOfOpeningTag, startOfClosingTag-endOfOpeningTag);
    }
    
    std::string getAttributeValue(const std::string & element, const std::string & tagName, const std::string & attributeName)
    {
        size_t endOfTagString = element.find("<"+tagName+" ")+2+tagName.length();
        size_t startOfAttributeValue = element.find(std::string(attributeName)+std::string("=\""), endOfTagString);
        if (startOfAttributeValue == std::string::npos)
            return (std::string(""));
        startOfAttributeValue += (2+attributeName.length());
        size_t endOfAttributeValue = element.find("\"",startOfAttributeValue);
        
        return element.substr(startOfAttributeValue, endOfAttributeValue - startOfAttributeValue) ;
    }
    
    
} // namespace gpxParser


#endif /* PARSER_C_ */
