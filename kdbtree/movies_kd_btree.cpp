#include "movies_kd_btree.hpp"


cmp_vector<movie> movie_comp = {
        compare_budget, compare_revenue, compare_runtime,
        compare_popularity, compare_vote_avg, compare_vote_count
};


rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions) {
        movie minimum, median, maximum;
        //assigning the double fields
        //assigning the budget
        auto min_budget_it = min_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_budget(get<0>(*a), get<0>(*b));
                        return (res < 0);
        });
        auto max_budget_it = max_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_budget(get<2>(*a), get<2>(*b));
                        return (res < 0);
        });
        rectangle<movie> *min_budget_reg = *min_budget_it;
        rectangle<movie> *max_budget_reg = *max_budget_it;
        minimum.budget = get<0>(*min_budget_reg).budget;
        maximum.budget = get<2>(*max_budget_reg).budget;
        median.budget = (minimum.budget + maximum.budget) / 2;
        //assign the revenue
        auto min_revenue_it = min_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_revenue(get<0>(*a), get<0>(*b));
                        return (res < 0);
        });
        auto max_revenue_it = max_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_revenue(get<2>(*a), get<2>(*b));
                        return (res < 0);
        });
        rectangle<movie> *min_revenue_reg = *min_revenue_it;
        rectangle<movie> *max_revenue_reg = *max_revenue_it;
        minimum.revenue = get<0>(*min_revenue_reg).revenue;
        maximum.revenue = get<2>(*max_revenue_reg).revenue;
        median.revenue = (minimum.revenue + maximum.revenue) / 2;
        //assign popularity
        auto min_pop_it = min_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_popularity(get<0>(*a), get<0>(*b));
                        return (res < 0);
        });
        auto max_pop_it = max_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_popularity(get<2>(*a), get<2>(*b));
                        return (res < 0);
        });
        rectangle<movie> *min_pop_reg = *min_pop_it;
        rectangle<movie> *max_pop_reg = *max_pop_it;
        minimum.popularity = get<0>(*min_pop_reg).popularity;
        maximum.popularity = get<2>(*max_pop_reg).popularity;
        median.popularity = (minimum.popularity + maximum.popularity) / 2;
        //assign vote average
        auto min_vote_avg_it = min_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_vote_avg(get<0>(*a), get<0>(*b));
                        return (res < 0);
        });
        auto max_vote_avg_it = max_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_vote_avg(get<2>(*a), get<2>(*b));
                        return (res < 0);
        });
        rectangle<movie> *min_vote_avg_reg = *min_vote_avg_it;
        rectangle<movie> *max_vote_avg_reg = *max_vote_avg_it;
        minimum.vote_avg = get<0>(*min_vote_avg_reg).vote_avg;
        maximum.vote_avg = get<2>(*max_vote_avg_reg).vote_avg;
        median.vote_avg = (minimum.vote_avg + maximum.vote_avg) / 2;
        //done with double fields now integer fields
        //assign runtime
        auto min_runtime_it = min_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_runtime(get<0>(*a), get<0>(*b));
                        return (res < 0);
        });
        auto max_runtime_it = max_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_runtime(get<2>(*a), get<2>(*b));
                        return (res < 0);
        });
        rectangle<movie> *min_runtime_reg = *min_runtime_it;
        rectangle<movie> *max_runtime_reg = *max_runtime_it;
        minimum.runtime = get<0>(*min_runtime_reg).runtime;
        maximum.runtime = get<2>(*max_runtime_reg).runtime;
        median.runtime = (minimum.runtime + maximum.runtime) / 2;
        //assign vote_cnt
        auto min_vote_it = min_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_vote_count(get<0>(*a), get<0>(*b));
                        return (res < 0);
        });
        auto max_vote_it = max_element(movie_regions.begin(), movie_regions.end(),
                [](const auto &a, const auto &b) {
                        double res = compare_vote_count(get<2>(*a), get<2>(*b));
                        return (res < 0);
        });
        rectangle<movie> *min_vote_reg = *min_vote_it;
        rectangle<movie> *max_vote_reg = *max_vote_it;
        minimum.vote_count = get<0>(*min_vote_reg).vote_count;
        maximum.vote_count = get<2>(*max_vote_reg).vote_count;
        median.vote_count = (minimum.vote_count + maximum.vote_count) / 2;

        return make_tuple(minimum, median, maximum);
}

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points) {
        movie minimum, median, maximum;
        return make_tuple(minimum, median, maximum);
}
