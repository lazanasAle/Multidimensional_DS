#ifndef _MOVIE_UTILS_HPP
#define _MOVIE_UTILS_HPP
#pragma once

#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>

#define N 50

using   std::string, std::fstream,
        std::int32_t, std::to_string;

typedef char name[N];

struct date_t {
        size_t year;
        uint8_t month;
        uint8_t day;
};

struct movie {
        name title, org_lang, org_country,
                genre_names, prod_comp_names;
        date_t release_date;
        double budget, revenue, popularity, vote_avg;
        size_t id, runtime, vote_count;
        bool adult;
        string print_interesting();
};

movie create_default_movie();

static inline double compare_budget(const movie &a, const movie &b) {return (a.budget - b.budget);}
static inline double compare_revenue(const movie &a, const movie &b) {return (a.revenue - b.revenue);}
static inline double compare_popularity(const movie &a, const movie &b) {return (a.popularity - b.popularity);}
static inline double compare_vote_avg(const movie &a, const movie &b) {return (a.vote_avg - b.vote_avg);}
static inline double compare_runtime(const movie &a, const movie &b) {return ((double)a.runtime - (double)b.runtime);}
static inline double compare_vote_count(const movie &a, const movie &b) {return ((double)a.vote_count - (double)b.vote_count);}

static inline void copy_to_char_array(char *dest, const string &src) {
        strncpy(dest, src.c_str(), N);
}

static inline date_t parse_date(const string &s) {
        size_t y;
        uint8_t m, d;
        date_t ret_date{2022, 10, 3};
        if (sscanf(s.c_str(), "%ld-%hhd-%hhd", &y, &m, &d) == 3) {
                return date_t{y, m, d};
        }
        return ret_date;
}

#endif /* _MOVIE_UTILS_HPP */
