//
// Created by michael on 11/27/18.
//

#ifndef SMAK_SMFILE_H
#define SMAK_SMFILE_H

#include <string>
#include <vector>
#include <map>
#include "User.h"

namespace smak{
    class SmFile {
    public:
        SmFile(const std::string& fileName, const std::string& fileContents);
        void changeIndex(std::shared_ptr<smak::User> author, int index);
        void addUser(std::shared_ptr<smak::User> author);
        void removeUser(std::shared_ptr<smak::User> authorToRemove);
        void insert(std::shared_ptr<smak::User> author, const std::string& toInsert);
        void delContent(std::shared_ptr<smak::User> author, int deleteContentEnd);

    private:
        std::string fileName;
        std::map<std::shared_ptr<smak::User>, int> userIndeces;
        std::string fileContents;
    };
}



#endif //SMAK_SMFILE_H
