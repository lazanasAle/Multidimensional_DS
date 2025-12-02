#ifndef _MOVIE_UTILS_HPP
#define _MOVIE_UTILS_HPP
#pragma once

#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <chrono>
#include <charconv>
#include <algorithm>

#define N 50

using   std::string, std::chrono::year_month_day, std::chrono::sys_days,
        std::chrono::days, std::min, std::strncpy, std::chrono::year,
        std::chrono::month, std::chrono::day, std::fstream, std::format;

typedef char name[N];

struct movie {
        name title, org_lang, org_country,
                genre_names, prod_comp_names;
        year_month_day release_date;
        double budget, revenue, popularity, vote_avg;
        size_t id, runtime, vote_count;
        bool adult;

        //serialization for file io
        void read(fstream &file);
        void write(fstream &file);
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


static inline void copy_to_char_array(char *dest, const string &src) {
        strncpy(dest, src.c_str(), N);
}

static inline year_month_day parse_date(const string &s) {
        int y, m, d;
        if (sscanf(s.c_str(), "%d-%d-%d", &y, &m, &d)) {
                return year_month_day{year{y}, month{(unsigned)m}, day{(unsigned)d}};
        }
        return year_month_day{};
}

#endif /* _MOVIE_UTILS_HPP */
