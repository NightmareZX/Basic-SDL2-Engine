#pragma once

//For basic console interactions as well as other things
#include <iostream>

//Easier and more secure handling of strings
#include <string>

//Good dynamic container
#include <vector>

//Might be useful in curtain situations to use smart pointers instead
#include <memory>

//Iterators can be useful to treverse containers like vectors
#include <iterator>

//For File reading and writing
#include <fstream>

//For storing things
#include <map>

//String streams
#include <sstream>

#include <cmath>

//standard library c++ string
using String = std::string;

//
using WriteFileStream = std::ofstream;
using ReadFileStream = std::ifstream;

using std::ios;

//Vector container
using std::vector;

using std::map;

using std::pair;

using std::getline;
using std::to_string;

using InputStringStream = std::istringstream;
using OutputStringStream = std::ostringstream;

using StringStream = std::stringstream;