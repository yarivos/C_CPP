#include "RecommenderSystemLoader.h"

std::vector<double> create_vec_from_stream(std::istringstream &ss);

std::pair<std::string, int>
create_movie_from_token2(std::string &token);

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file(
        const std::string &movies_file_path) noexcept(false)
{
    RecommenderSystem rs;
    std::ifstream is(movies_file_path);
    std::string line;
    while (std::getline(is, line))
    {
        if (line.length() == 0)
        {
            break;
        }
        std::istringstream ss(line);
        std::string token;
        ss >> token;
        std::pair<std::string, int> movie_data = create_movie_from_token2(
                token);
        std::vector<double> features = create_vec_from_stream(ss);
        rs.add_movie(movie_data.first, movie_data.second, features);
    }
    return std::make_unique<RecommenderSystem>(rs);
}

// parsing line using the delimiter '-'
std::pair<std::string, int>
create_movie_from_token2(std::string &token)
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

// creates a features vector from stream
std::vector<double> create_vec_from_stream(std::istringstream &ss)
{
    double feature;
    std::vector<double> features;
    while (ss >> feature)
    {
        if (feature < MIN_FEATURE_VALUE || feature > MAX_FEATURE_VALUE)
        {
            throw std::runtime_error(BAD_RATINGS);
        }
        features.push_back(feature);
    }
    return features;
}