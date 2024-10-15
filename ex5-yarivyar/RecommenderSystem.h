#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H

#include <math.h>
#include "RSUser.h"
#include "Movie.h"  //maybe delete
#include <map>

#define NOT_RANKED 0
#define SIMILARITY_UPPER_BOUND 2.0
#define SIMILARITY_LOWER_BOUND 0

typedef std::map<sp_movie, std::vector<double>, equal_func> movie_map;

bool sp_movie_ptr_compare(const sp_movie &m1, const sp_movie &m2);

class RecommenderSystem
{
private:
    movie_map _movie_map;

    // returns the average of scores given to movies by user
    double get_avg_movies_ranks(const RSUser &user) const
    {
        double avg_movie_score = 0;
        int ranked_counter = 0;
        const rank_map temp_map = user.get_ranks();
        for (const auto &iter: temp_map)
        {
            if (iter.second != NOT_RANKED)
            {
                avg_movie_score += iter.second;
                ranked_counter += 1;
            }
        }
        return avg_movie_score / ranked_counter;
    }

    // creates a vector of preference of movie features for user
    std::vector<double> get_preference_vector(const RSUser &user) const
    {
        double avg_movies_score = get_avg_movies_ranks(user);
        int feature_vec_size = _movie_map.begin()->second.size();
        std::vector<double> pref_vec(feature_vec_size);
        for (const auto &ranks_iter: user.get_ranks())
        {
            if (ranks_iter.second != NOT_RANKED)
            {
                int counter = 0;
                double curr_rank = ranks_iter.second - avg_movies_score;
                for (const auto &feature_iter:
                        _movie_map.at(ranks_iter.first))
                {
                    pref_vec[counter] += curr_rank * feature_iter;
                    counter++;
                }
            }
        }
        return pref_vec;
    }

    // calculate dot product of two vectors
    double calculate_dot_product(const std::vector<double> &a_vec,
                                 const std::vector<double> &b_vec) const
    {
        if (a_vec.size() != b_vec.size())
        {
            throw std::invalid_argument(
                    "cant use dot product on vectors with different sizes.");
        }
        double product = 0;
        for (size_t i = 0; i < a_vec.size(); i++)
        {
            product += a_vec[i] * b_vec[i];
        }
        return product;
    }

    // calculates similarity of two movies a,b using the next equation
    // (a.features <dot_product> b.features) /
    // product of |a.features| with |b.features|
    double calculate_movies_similarity(const std::vector<double> &a_vec,
                                       const std::vector<double> &b_vec) const
    {
        double numerator = calculate_dot_product(a_vec, b_vec);
        double denominator = sqrt(calculate_dot_product(
                a_vec, a_vec)) * sqrt(calculate_dot_product
                                              (b_vec, b_vec));
        return numerator / denominator;
    }

    // finds the minimum similarity index inside k_similar movies
    int get_index_min_similarity(
            const std::vector<std::pair<sp_movie, double>> &k_similar_movies)
    const
    {
        int counter = 0;
        int min_sim_ind;
        double min_sim = SIMILARITY_UPPER_BOUND;
        for (const auto &iter: k_similar_movies)
        {
            if (iter.second < min_sim)
            {
                min_sim = iter.second;
                min_sim_ind = counter;
            }
            counter++;
        }
        return min_sim_ind;
    }

    // calculates the prediction of score to a movie
    double calculate_predict_movie_score(
            const std::vector<std::pair<sp_movie, double>> &k_similar_movies,
            const RSUser &user, int k) const
    {
        if ((size_t) k != k_similar_movies.size())
        {
            throw std::invalid_argument("movies vector size doesnt match k");
        }
        double prediction = 0;
        double similarity_sum = 0;
        rank_map temp_map(user.get_ranks());
        for (const auto &iter: k_similar_movies)
        {
            prediction += iter.second * temp_map[iter.first];
            similarity_sum += iter.second;
        }
        return prediction / similarity_sum;
    }

public:

    RecommenderSystem();
//	explicit RecommenderSystem()
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
    sp_movie add_movie(const std::string &name, int year,
                       const std::vector<double> &features);


    /**
     * a function that calculates the movie with highest
     * score based on movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
    sp_movie recommend_by_content(const RSUser &user) const;

    /**
     * a function that calculates the movie with highest
     * predicted score based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
    sp_movie recommend_by_cf(const RSUser &user, int k) const;


    /**
     * Predict a user rating for a movie given argument using item
     * cf procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
    double predict_movie_score(const RSUser &user, const sp_movie &movie,
                               int k) const;

    /**
     * gets a shared pointer to movie in system
     * @param name name of movie
     * @param year year movie was made
     * @return shared pointer to movie in system
     */
    sp_movie get_movie(const std::string &name, int year) const;


    friend std::ostream &operator<<(std::ostream &ostream,
                                    const RecommenderSystem &_rs);

};


#endif //RECOMMENDERSYSTEM_H