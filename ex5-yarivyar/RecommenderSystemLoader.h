
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"
#include <sstream>
#include <fstream>
#include <iostream>

#define MIN_FEATURE_VALUE 1
#define MAX_FEATURE_VALUE 10
#define BAD_RATINGS "SOME OF THE MOVIE USERS RATINGS DON'T HAVE INVALID VALUES"

class RecommenderSystemLoader
{

public:
    RecommenderSystemLoader() = delete;

    /**
	 * loads movies by the given format for movies with their feature's score
	 * @param movies_file_path a path to the file of the movies
	 * @return unique pointer to a RecommenderSystem
     * which was created with those movies
	 */
    static std::unique_ptr<RecommenderSystem> create_rs_from_movies_file(
            const std::string &movies_file_path) noexcept(false);
};


#endif //RECOMMENDERSYSTEMLOADER_H