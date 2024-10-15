
// don't change these include
#include "RSUser.h"
#include "RecommenderSystem.h"


void RSUser::add_movie_to_rs(const std::string &name, int year,
                             const std::vector<double> &features,
                             double rate)
{
    _rank_map[_rs->add_movie(name, year, features)] = rate;
    // _rs.add_movie() return shared pointer to sp_movie
}

sp_movie RSUser::get_recommendation_by_content() const
{
    return _rs->recommend_by_content(*this);
}

double
RSUser::get_prediction_score_for_movie(const std::string &name, int year,
                                       int k) const
{
    return _rs->predict_movie_score(*this,
                                    std::make_shared<Movie>
                                            (Movie(name, year)),
                                    k);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
    return _rs->recommend_by_cf(*this, k);
}

std::ostream &operator<<(std::ostream &ostream, const RSUser &rs_user)
{
    ostream << "name: " << rs_user._username << "\n";
    ostream << *(rs_user._rs);
    ostream << std::endl;
    return ostream;
}