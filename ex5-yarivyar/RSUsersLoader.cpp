#include "RSUsersLoader.h"

void fill_rank_map(std::istringstream &ss, rank_map &new_map,
                   const std::vector<std::pair<std::string, int>> &movie_data,
                   std::shared_ptr<RecommenderSystem> rs_shared);

std::pair<std::string, int>
create_movie_from_token(std::string &token);

std::vector<std::pair<std::string, int>>
create_movies_vec(std::string &line);

std::vector<RSUser>
RSUsersLoader::create_users_from_file(const std::string &users_file_path,
                                      std::unique_ptr<RecommenderSystem> rs)
noexcept(false)
{
    std::vector<RSUser> users_vec;
    std::shared_ptr<RecommenderSystem> rs_shared = std::move(rs);
    std::ifstream is(users_file_path);
    std::string line;
    std::getline(is, line);
    std::vector<std::pair<std::string, int>> movie_data = create_movies_vec(
            line);
    std::string token;
    while (std::getline(is, line)) // lines of rating of movies
    {
        if (line.length() == 0)
        {
            break;
        }
        rank_map new_map(BUCKET_SIZE, sp_movie_hash, sp_movie_equal);
        std::istringstream ss(line);
        std::string user_name;
        ss >> user_name;    // first string inline is the username
        fill_rank_map(ss, new_map, movie_data, rs_shared);
        users_vec.push_back(RSUser(user_name, new_map, rs_shared));
    }
    return users_vec;
}
// parses string token in the form of  "<movie_name>-<year>"' returns a pair - movie_name, year
std::pair<std::string, int>
create_movie_from_token(std::string &token)
{
    int year;
    char delim = '-';
    int delim_pos = (int) token.find(delim);
    std::string movie_name = token.substr(0, delim_pos);
    std::stringstream str_year(
            token.substr(delim_pos + 1, token.length()));
    str_year >> year;
    return std::make_pair(movie_name, year);
}

// creates a movies vector of all movies in the title of the input file
std::vector<std::pair<std::string, int>>
create_movies_vec(std::string &line)
{
    std::istringstream ss(line);
    std::vector<std::pair<std::string, int>> movies_vec;
    int movie_year;
    std::string movie_name;
    char delim = '-';
    int delim_pos;
    std::string token;
    while (ss >> token)
    {
        // parsing token to name of the movie and year movie was made.
        delim_pos = (int) token.find(delim);
        movie_name = token.substr(0, delim_pos);
        std::stringstream str_year(
                token.substr(delim_pos + 1, token.length()));
        str_year >> movie_year;
        movies_vec.push_back(std::make_pair(movie_name, movie_year));
    }
    return movies_vec;
}
/// function inserts movies and user rankings to the user rank_map
/// \param ss stream that holds the current user ranks for movies
/// \param new_map current user rank_map
/// \param movie_data vector which holds all the movies to load to new_map
/// \param rs_shared shared pointer to the RecommenderSystem
void fill_rank_map(std::istringstream &ss, rank_map &new_map,
                   const std::vector<std::pair<std::string, int>> &movie_data,
                   std::shared_ptr<RecommenderSystem> rs_shared)
{
    double user_rating;
    int counter = 0;
    std::string token;
    while (ss >> token)
    {
        if (token == UNRANKED)
        {
            user_rating = 0;
        } else
        {
            std::istringstream str_rating(token);
            str_rating >> user_rating;
        }
        if (user_rating < MIN_FEATURE_VALUE || user_rating > MAX_FEATURE_VALUE)
        {
            if (user_rating != 0)
            {
                throw std::runtime_error(BAD_RATINGS);
            }
        }
        new_map[rs_shared->get_movie(movie_data[counter].first,
                                     movie_data[counter].second)]
                                     = user_rating;
        counter++;
    }
}