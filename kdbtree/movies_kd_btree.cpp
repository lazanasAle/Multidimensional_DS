#include "movies_kd_btree.hpp"


cmp_vector<movie> movie_comp = {
        compare_budget, compare_revenue, compare_runtime,
        compare_popularity, compare_vote_avg, compare_vote_count
};



template<size_t corner, typename field>

field get_field(rectangle<movie> *r, field movie:: *member) {
        return (get<corner>(*r).*member);
}

template<typename field>

pair<field, field> compute_min_max(vector<rectangle<movie> *> &movie_regions, field movie:: *member,
        function<double (const movie &, const movie &)> compare) {
                auto min_it = min_element(movie_regions.begin(), movie_regions.end(),
                        [compare](const auto &a, const auto &b) {
                                double res = compare(get<0>(*a), get<0>(*b));
                                return (res < 0);
                });
                auto max_it = max_element(movie_regions.begin(), movie_regions.end(),
                        [compare](const auto &a, const auto &b) {
                                double res = compare(get<2>(*a), get<2>(*b));
                                return (res < 0);
                });
                field minv = get_field<0>(*min_it, member);
                field maxv = get_field<0>(*max_it, member);

                return make_pair(minv, maxv);
}


template<typename field>

pair<field, field> compute_point_min_max(vector<movie *> &movie_pt, field movie:: *member,
        function<double (const movie &, const movie &)> compare) {
                auto min_it = min_element(movie_pt.begin(), movie_pt.end(),
                        [compare](const auto &a, const auto &b) {
                                double res = compare(*a, *b);
                                return (res < 0);
                });
                auto max_it = max_element(movie_pt.begin(), movie_pt.end(),
                        [compare](const auto &a, const auto &b) {
                                double res = compare(*a, *b);
                                return (res < 0);
                });

                field minv = (*min_it)->*member;
                field maxv = (*max_it)->*member;

                return make_pair(minv, maxv);
}

rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions) {
        movie minimum, median, maximum;
        //assigning the double fields
        //assigning the budget
        pair<double, double> budget_values = compute_min_max(movie_regions, &movie::budget, compare_budget);
        minimum.budget = budget_values.first;
        maximum.budget = budget_values.second;
        median.budget = (minimum.budget + maximum.budget) / 2;
        //assign the revenue
        pair<double, double> revenue_values = compute_min_max(movie_regions, &movie::revenue, compare_revenue);
        minimum.revenue = revenue_values.first;
        maximum.revenue = revenue_values.second;
        median.revenue = (minimum.revenue + maximum.revenue) / 2;
        //assign popularity
        pair<double, double> popularity_values = compute_min_max(movie_regions, &movie::popularity, compare_popularity);
        minimum.popularity = popularity_values.first;
        maximum.popularity = popularity_values.second;
        median.popularity = (minimum.popularity + maximum.popularity) / 2;
        //assign vote average
        pair<double, double> vote_avg_values = compute_min_max(movie_regions, &movie::vote_avg, compare_vote_avg);
        minimum.vote_avg = vote_avg_values.first;
        maximum.vote_avg = vote_avg_values.second;
        median.vote_avg = (minimum.vote_avg + maximum.vote_avg) / 2;
        //done with double fields now integer fields
        //assign runtime
        pair<size_t, size_t> runtime_values = compute_min_max(movie_regions, &movie::runtime, compare_runtime);
        minimum.runtime = runtime_values.first;
        maximum.runtime = runtime_values.second;
        median.runtime = (minimum.runtime + maximum.runtime) / 2;
        //assign vote_cnt
        pair<size_t, size_t> vote_cnt_values = compute_min_max(movie_regions, &movie::vote_count, compare_vote_count);
        minimum.vote_count = vote_cnt_values.first;
        maximum.vote_count = vote_cnt_values.second;
        median.vote_count = (minimum.vote_count + maximum.vote_count) / 2;

        return make_tuple(minimum, median, maximum);
}

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points) {
        movie minimum, median, maximum;
        //assigning the double fields
        //assigning the budget
        pair<double, double> budget_values = compute_point_min_max(movie_points, &movie::budget, compare_budget);
        minimum.budget = budget_values.first;
        maximum.budget = budget_values.second;
        median.budget = (minimum.budget + maximum.budget) / 2;
        //assign the revenue
        pair<double, double> revenue_values = compute_point_min_max(movie_points, &movie::revenue, compare_revenue);
        minimum.revenue = revenue_values.first;
        maximum.revenue = revenue_values.second;
        median.revenue = (minimum.revenue + maximum.revenue) / 2;
        //assign popularity
        pair<double, double> popularity_values = compute_point_min_max(movie_points, &movie::popularity, compare_popularity);
        minimum.popularity = popularity_values.first;
        maximum.popularity = popularity_values.second;
        median.popularity = (minimum.popularity + maximum.popularity) / 2;
        //assign vote average
        pair<double, double> vote_avg_values = compute_point_min_max(movie_points, &movie::vote_avg, compare_vote_avg);
        minimum.vote_avg = vote_avg_values.first;
        maximum.vote_avg = vote_avg_values.second;
        median.vote_avg = (minimum.vote_avg + maximum.vote_avg) / 2;
        //done with double fields now integer fields
        //assign runtime
        pair<size_t, size_t> runtime_values = compute_point_min_max(movie_points, &movie::runtime, compare_runtime);
        minimum.runtime = runtime_values.first;
        maximum.runtime = runtime_values.second;
        median.runtime = (minimum.runtime + maximum.runtime) / 2;
        //assign vote_cnt
        pair<size_t, size_t> vote_cnt_values = compute_point_min_max(movie_points, &movie::vote_count, compare_vote_count);
        minimum.vote_count = vote_cnt_values.first;
        maximum.vote_count = vote_cnt_values.second;
        median.vote_count = (minimum.vote_count + maximum.vote_count) / 2;

        return make_tuple(minimum, median, maximum);
}
