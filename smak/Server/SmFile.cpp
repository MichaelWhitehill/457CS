//
// Created by michael on 11/27/18.
//

#include "SmFile.h"

smak::SmFile::SmFile(const std::string &fileName, const std::string &fileContents) {
    this->fileName = fileName;
    this->fileContents = fileContents;
}
