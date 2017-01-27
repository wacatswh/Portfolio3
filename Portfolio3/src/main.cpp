//
//  main.cpp
//
//  Created by Hibberd, Richard on 03/10/2013.
//  Copyright (c) 2013 Hibberd, Richard. All rights reserved.
//



#include "../headers/classes/gpxp.h"
#include "../headers/utilities/utils.h"
#include "../headers/classes/parser.h"

#include <string>
#include <vector>
#include <iostream>

using namespace gpxp;

int main(int argc, const char * argv[])
{
    Route r1("src/ABCD.gpx", true);
    std::cout << (r1.builtOk()?"True":"False") << std::endl;
    std::cout << r1.toString() << std::endl;
    std::cout << r1.routeLength() << std::endl;

	system("pause");
}
