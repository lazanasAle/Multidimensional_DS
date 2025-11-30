#ifndef _MOVIE_UTILS_HPP
#define _MOVIE_UTILS_HPP
#pragma once

#include <string>
#include <chrono>
#include <algorithm>

using   std::string, std::chrono::year_month_day, std::chrono::sys_days,
        std::chrono::days, std::min;

struct movie {
        string title, org_lang, org_country,
                genre_names, prod_comp_names;
        year_month_day release_date;
        double budget, revenue, popularity, vote_avg;
        size_t id, runtime, vote_count;
        bool adult;
};

double compare_str(const string &a, const string &b) {
        size_t len1 = a.size();
        size_t len2 = b.size();
        size_t min_len = min(len1, len2);

        for (size_t j = 0; j < min_len; ++j) {
                if(a[j] != b[j])
                        return (a[j] - b[j]);
        }
        return (len1 - len2);
}

static inline double compare_title(const movie &a, const movie &b) {return compare_str(a.title, b.title);}
static inline double compare_org_lang(const movie &a, const movie &b) {return compare_str(a.org_lang, b.org_lang);}
static inline double compare_org_country(const movie &a, const movie &b) {return compare_str(a.org_country, b.org_country);}
static inline double compare_genre_names(const movie &a, const movie &b) {return compare_str(a.genre_names, b.genre_names);}
static inline double compare_prod_comp_names(const movie &a, const movie &b) {return compare_str(a.prod_comp_names, b.prod_comp_names);}
static inline double compare_budget(const movie &a, const movie &b) {return (a.budget - b.budget);}
static inline double compare_revenue(const movie &a, const movie &b) {return (a.revenue - b.revenue);}
static inline double compare_popularity(const movie &a, const movie &b) {return (a.popularity - b.popularity);}
static inline double compare_vote_avg(const movie &a, const movie &b) {return (a.vote_avg - b.vote_avg);}
static inline double compare_id(const movie &a, const movie &b) {return (a.id - b.id);}
static inline double compare_runtime(const movie &a, const movie &b) {return (a.runtime - b.runtime);}
static inline double compare_vote_count(const movie &a, const movie &b) {return (a.vote_count - b.vote_count);}

static inline double compare_release_dates(const movie &a, const movie &b) {
        sys_days sd1 = a.release_date;
        sys_days sd2 = b.release_date;
        days diff = sd1 - sd2;
        return diff.count();
}

#endif /* _MOVIE_UTILS_HPP */
