
#ifndef USERFACTORY_H
#define USERFACTORY_H

#include <stdlib.h>
#include <vector>
#include <string>
#include "RSUser.h"   //maybe delete
#include "RecommenderSystem.h"
#include <sstream>
#include <fstream>
#include <iostream>

#define UNRANKED "NA"
#define BUCKET_SIZE 16
#define MIN_FEATURE_VALUE 1
#define MAX_FEATURE_VALUE 10
#define BAD_RATINGS "SOME OF THE MOVIE USERS RATINGS DON'T HAVE INVALID VALUES"


class RSUsersLoader
{
public:
    RSUsersLoader() = delete;

    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users
     * and their movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser>
    create_users_from_file(const std::string &users_file_path,
                           std::unique_ptr<RecommenderSystem> rs)
                           noexcept(false);

};


#endif //USERFACTORY_H