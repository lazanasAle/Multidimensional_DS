#include "movie_utils.hpp"
#include <string>

movie create_default_movie() {
        movie this_movie;
        strcpy(this_movie.title, "UN");
        strcpy(this_movie.org_lang, "UN");
        strcpy(this_movie.org_country, "UN");
        strcpy(this_movie.genre_names, "UN");
        strcpy(this_movie.prod_comp_names, "UN");

        this_movie.release_date = date_t{2022, 10, 3};
        this_movie.adult = true;

        this_movie.budget = this_movie.revenue = this_movie.popularity = this_movie.vote_avg = -1;
        this_movie.id = this_movie.runtime = this_movie.vote_count = 0;

        return this_movie;
}


string movie::print_interesting() {
        string ret = "<";
        ret += to_string(this->budget) + ", ";
        ret += to_string(this->revenue) + ", ";
        ret += to_string(this->runtime) + ", ";
        ret += to_string(this->popularity) + ", ";
        ret += to_string(this->vote_avg) + ", ";
        ret += to_string(this->vote_count) + ">";

        return ret;
}
