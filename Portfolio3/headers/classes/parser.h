/*
 * parser.h
 *
 *  Created on: Jan 30, 2011
 *      Author: rbh
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>


namespace gpxParser
{
	bool tagExists(const std::string & sauce, const std::string & tagName);
	std::string getWholeElement(const std::string & sauce,  const std::string & tagName);
  	std::string getElementValue(std::string & sauce, const std::string & tagName);
  	std::string getAttributeValue(const std::string & element, const std::string & tagName, const std::string & attributeName);
} // namespace gpxParser

#endif /* PARSER_H_ */
