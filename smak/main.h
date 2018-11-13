//
// Created by michael on 10/16/18.
//

#ifndef SMAK_MAIN_H
#define SMAK_MAIN_H
#include <string>
#include <vector>

/**
 * Prints the error message to standard error and terminates with exit code 1
 * @param msg Error message to be printed
 */
void error(const char *msg);

/**
 * Splits a string based on whitespace
 * @param input String to be split
 * @return Vector of strings, being the parts of the input separated by spaces
 */
std::vector<std::string> split(std::string const &input);


#endif //SMAK_MAIN_H
