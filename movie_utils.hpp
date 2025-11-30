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

static inline double compare_budget(const movie &a, const movie &b) {return (a.budget - b.budget);}
static inline double compare_revenue(const movie &a, const movie &b) {return (a.revenue - b.revenue);}
static inline double compare_popularity(const movie &a, const movie &b) {return (a.popularity - b.popularity);}
static inline double compare_vote_avg(const movie &a, const movie &b) {return (a.vote_avg - b.vote_avg);}
static inline double compare_runtime(const movie &a, const movie &b) {return (a.runtime - b.runtime);}
static inline double compare_vote_count(const movie &a, const movie &b) {return (a.vote_count - b.vote_count);}

static inline double compare_release_dates(const movie &a, const movie &b) {
        sys_days sd1 = a.release_date;
        sys_days sd2 = b.release_date;
        days diff = sd1 - sd2;
        return diff.count();
}

#endif /* _MOVIE_UTILS_HPP */
