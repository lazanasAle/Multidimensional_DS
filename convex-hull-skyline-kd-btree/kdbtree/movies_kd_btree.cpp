#include "movies_kd_btree.hpp"
#include "kd_btree.hpp"
#include <cstdint>



cmp_vector<movie> movie_comp = {
        compare_budget, compare_revenue, compare_runtime,
        compare_popularity, compare_vote_avg, compare_vote_count
};



template<size_t corner, typename field>

field get_field(rectangle<movie> *r, field movie:: *member) {
        return (get<corner>(*r).*member);
}

template<typename field>

pair<field, field> compute_min_max(vector<rectangle<movie> *> &movie_regions, field movie:: *member) {
        size_t length = movie_regions.size();
        size_t min_idx = 0, max_idx = 0;

        //computing min
        for (size_t j = 1; j < length; ++j) {
                field current = get<0>(*movie_regions[j]).*member;
                field min_movie = get<0>(*movie_regions[min_idx]).*member;
                if (current < min_movie)
                        min_idx = j;
        }
        //computing max
        for (size_t j = 1; j < length; ++j) {
                field current = get<2>(*movie_regions[j]).*member;
                field max_movie = get<2>(*movie_regions[max_idx]).*member;
                if (current > max_movie)
                        max_idx = j;
        }

        field minv = get<0>(*movie_regions[min_idx]).*member;
        field maxv = get<2>(*movie_regions[max_idx]).*member;

        return make_pair(minv, maxv);
}


template<typename field>

pair<field, field> compute_point_min_max(vector<movie *> &movie_pt, field movie:: *member) {
        size_t length = movie_pt.size();
        size_t min_idx = 0, max_idx = 0;

        //computing min
        for (size_t j = 1; j < length; ++j) {
                field current = (*movie_pt[j]).*member;
                field min_movie = (*movie_pt[min_idx]).*member;
                if (current < min_movie)
                        min_idx = j;
        }
        //computing max
        for (size_t j = 1; j < length; ++j) {
                field current = (*movie_pt[j]).*member;
                field max_movie = (*movie_pt[max_idx]).*member;
                if (current > max_movie)
                        max_idx = j;
        }

        field minv = (*movie_pt[min_idx]).*member;
        field maxv = (*movie_pt[max_idx]).*member;

        return make_pair(minv, maxv);
}

rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions) {
        movie minimum, median, maximum;
        //assigning the double fields
        //assigning the budget
        pair<double, double> budget_values = compute_min_max(movie_regions, &movie::budget);
        minimum.budget = budget_values.first;
        maximum.budget = budget_values.second;
        median.budget = (minimum.budget + maximum.budget) / 2;
        //assign the revenue
        pair<double, double> revenue_values = compute_min_max(movie_regions, &movie::revenue);
        minimum.revenue = revenue_values.first;
        maximum.revenue = revenue_values.second;
        median.revenue = (minimum.revenue + maximum.revenue) / 2;
        //assign popularity
        pair<double, double> popularity_values = compute_min_max(movie_regions, &movie::popularity);
        minimum.popularity = popularity_values.first;
        maximum.popularity = popularity_values.second;
        median.popularity = (minimum.popularity + maximum.popularity) / 2;
        //assign vote average
        pair<double, double> vote_avg_values = compute_min_max(movie_regions, &movie::vote_avg);
        minimum.vote_avg = vote_avg_values.first;
        maximum.vote_avg = vote_avg_values.second;
        median.vote_avg = (minimum.vote_avg + maximum.vote_avg) / 2;
        //done with double fields now integer fields
        //assign runtime
        pair<size_t, size_t> runtime_values = compute_min_max(movie_regions, &movie::runtime);
        minimum.runtime = runtime_values.first;
        maximum.runtime = runtime_values.second;
        median.runtime = (minimum.runtime + maximum.runtime) / 2;
        //assign vote_cnt
        pair<size_t, size_t> vote_cnt_values = compute_min_max(movie_regions, &movie::vote_count);
        minimum.vote_count = vote_cnt_values.first;
        maximum.vote_count = vote_cnt_values.second;
        median.vote_count = (minimum.vote_count + maximum.vote_count) / 2;

        return make_tuple(minimum, median, maximum);
}

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points) {
        movie minimum, median, maximum;
        //assigning the double fields
        //assigning the budget
        pair<double, double> budget_values = compute_point_min_max(movie_points, &movie::budget);
        minimum.budget = budget_values.first;
        maximum.budget = budget_values.second;
        median.budget = (minimum.budget + maximum.budget) / 2;
        //assign the revenue
        pair<double, double> revenue_values = compute_point_min_max(movie_points, &movie::revenue);
        minimum.revenue = revenue_values.first;
        maximum.revenue = revenue_values.second;
        median.revenue = (minimum.revenue + maximum.revenue) / 2;
        //assign popularity
        pair<double, double> popularity_values = compute_point_min_max(movie_points, &movie::popularity);
        minimum.popularity = popularity_values.first;
        maximum.popularity = popularity_values.second;
        median.popularity = (minimum.popularity + maximum.popularity) / 2;
        //assign vote average
        pair<double, double> vote_avg_values = compute_point_min_max(movie_points, &movie::vote_avg);
        minimum.vote_avg = vote_avg_values.first;
        maximum.vote_avg = vote_avg_values.second;
        median.vote_avg = (minimum.vote_avg + maximum.vote_avg) / 2;
        //done with double fields now integer fields
        //assign runtime
        pair<size_t, size_t> runtime_values = compute_point_min_max(movie_points, &movie::runtime);
        minimum.runtime = runtime_values.first;
        maximum.runtime = runtime_values.second;
        median.runtime = (minimum.runtime + maximum.runtime) / 2;
        //assign vote_cnt
        pair<size_t, size_t> vote_cnt_values = compute_point_min_max(movie_points, &movie::vote_count);
        minimum.vote_count = vote_cnt_values.first;
        maximum.vote_count = vote_cnt_values.second;
        median.vote_count = (minimum.vote_count + maximum.vote_count) / 2;

        return make_tuple(minimum, median, maximum);
}


void read_csv(kd_btree<movie> &movies_kdb) {
        rapidcsv::Document movies_csv("../data_movies_clean.csv", rapidcsv::LabelParams(0, -1));
        size_t row_len = movies_csv.GetRowCount();

        vector<string> int_columns = {"id", "runtime", "vote_count"};
        vector<size_t movie:: *> int_fields = {&movie::id, &movie::runtime, &movie::vote_count};

        vector<string> double_columns = {"budget", "popularity", "revenue", "vote_average"};
        vector<double movie:: *> double_fields = {&movie::budget, &movie::popularity, &movie::revenue, &movie::vote_avg};

        vector<string> str_columns = {"title", "original_language", "origin_country", "genre_names", "production_company_names"};
        vector<name movie:: *> str_fields = {&movie::title, &movie::org_lang, &movie::org_country, &movie::genre_names, &movie::prod_comp_names};

        for (size_t j = 0; j < 72; ++j) {
                movie m;
                //integer fields
                size_t int_f_c = int_fields.size();
                for (size_t i = 0; i < int_f_c; ++i)
                        m.*int_fields[i] = movies_csv.GetCell<uint64_t>(int_columns[i], j);
                //double fields
                size_t double_f_c = double_fields.size();
                for (size_t i = 0; i < double_f_c; ++i)
                        m.*double_fields[i] = movies_csv.GetCell<double>(double_columns[i], j);
                //string fields
                size_t str_f_c = str_fields.size();
                for (size_t i = 0; i < str_f_c; ++i)
                        copy_to_char_array(m.*str_fields[i], movies_csv.GetCell<string>(str_columns[i], j));
                m.adult = (movies_csv.GetCell<string>("adult", j).compare("TRUE") == 0);
                m.release_date = parse_date(movies_csv.GetCell<string>("release_date", j));

                movies_kdb.insert(m);
        }
}
