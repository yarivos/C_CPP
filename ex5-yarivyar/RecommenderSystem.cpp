#include "RecommenderSystem.h"
#include "RSUser.h"

bool sp_movie_ptr_compare(const sp_movie &m1, const sp_movie &m2)
{
    return *m1 < *m2;
}

RecommenderSystem::RecommenderSystem() : _movie_map(sp_movie_ptr_compare)
{}

sp_movie RecommenderSystem::recommend_by_content(const RSUser &user) const
{
    std::vector<double> pref_vec = get_preference_vector(user);
    rank_map user_ranks = user.get_ranks();
    sp_movie recommended_movie;
    double recommendation_score = 0;
    for (const auto &movie_iter: _movie_map)
        // runs on all movies added to the database
    {
        if (user_ranks.find(movie_iter.first) == user_ranks.end() ||
            //check if movie is inside rank_map or if it wasn't ranked
            user_ranks[movie_iter.first] == NOT_RANKED)
        {
            double movie_similarity = calculate_movies_similarity(
                    movie_iter.second, pref_vec);
            if (recommendation_score < movie_similarity)
            {
                recommendation_score = movie_similarity;
                recommended_movie = movie_iter.first;
            }
        }
    }
    return recommended_movie;
}

double RecommenderSystem::predict_movie_score(const RSUser &user,
                                              const sp_movie &movie,
                                              int k) const
{
    std::vector<std::pair<sp_movie, double>> k_similar_movies;
    double movie_similarity;
    for (const auto &ranks_iter: user.get_ranks())
    {
        if (ranks_iter.second != NOT_RANKED)
        {  // using only ranked movies by the user
            movie_similarity = calculate_movies_similarity(
                    _movie_map.at(ranks_iter.first), _movie_map.at(movie));
            if ((size_t) k != k_similar_movies.size())
            {
                k_similar_movies.push_back(
                        std::make_pair(ranks_iter.first, movie_similarity));
            } else
            {
                int min_sim_ind = get_index_min_similarity(k_similar_movies);
                if (k_similar_movies[min_sim_ind].second < movie_similarity)
                {
                    k_similar_movies[min_sim_ind] = std::make_pair(
                            ranks_iter.first, movie_similarity);
                }
            }
        }
    }
    return calculate_predict_movie_score(k_similar_movies, user, k);
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser &user, int k) const
{
    rank_map user_ranks = user.get_ranks();
    sp_movie recommended_movie;
    double recommendation_value = SIMILARITY_LOWER_BOUND;  // equals 0
    for (const auto &movie_iter: _movie_map)
    {   // runs on all movies added to the database
        if (user_ranks.find(movie_iter.first) == user_ranks.end() ||
            //check if movie is inside rank_map or if it wasn't ranked
            user_ranks[movie_iter.first] == NOT_RANKED)
        {

            double curr_score = predict_movie_score(user, movie_iter.first, k);
            if (recommendation_value < curr_score)
            {
                recommendation_value = curr_score;
                recommended_movie = movie_iter.first;
            }
        }
    }
    return recommended_movie;
}

sp_movie RecommenderSystem::add_movie(const std::string &name, int year,
                                      const std::vector<double> &features)
{
    sp_movie new_movie = std::make_shared<Movie>(Movie(name, year));
    _movie_map[new_movie] = features;
    return new_movie;
}

sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
    sp_movie find_movie = std::make_shared<Movie>(name, year);
    auto map_iter = _movie_map.find(find_movie);
    if (map_iter == _movie_map.end())
    {
        return nullptr;
    }
    return map_iter->first;
}

std::ostream &operator<<(std::ostream &ostream, const RecommenderSystem &_rs)
{
    for (const auto &movie_iter: _rs._movie_map)
    {
        ostream << *(movie_iter.first);
    }
    return ostream;
}


