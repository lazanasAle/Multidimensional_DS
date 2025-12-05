#include "movie_utils.hpp"
#include <string>

movie::movie() {
        strcpy(this->title, "UN");
        strcpy(this->org_lang, "UN");
        strcpy(this->org_country, "UN");
        strcpy(this->genre_names, "UN");
        strcpy(this->prod_comp_names, "UN");

        this->release_date = year_month_day{year{2022}, month{10}, day{3}};
        this->adult = true;

        this->budget = this->revenue = this->popularity = this->vote_avg = -1;
        this->id = this->runtime = this->vote_count = 0;
}

void read_date(fstream &file, year_month_day &ymd) {
        int32_t y;
        uint8_t m, d;

        file.read((char *)&y, sizeof(int32_t));
        file.read((char *)&m, sizeof(uint8_t));
        file.read((char *)&d, sizeof(uint8_t));

        ymd = year_month_day{year{y}, month{m}, day{d}};
}

void write_date(fstream &file, year_month_day &ymd) {
        int32_t y = (int) ymd.year();
        uint8_t m = (unsigned) unsigned(ymd.month());
        uint8_t d = (unsigned) unsigned(ymd.day());

        file.write((char *)&y, sizeof(int32_t));
        file.write((char *)&m, sizeof(uint8_t));
        file.write((char *)&d, sizeof(uint8_t));
}

void movie::read(fstream &file) {
        //char * fields
        file.read(this->title, N);
        file.read(this->org_lang, N);
        file.read(this->org_country, N);
        file.read(this->genre_names, N);
        file.read(this->prod_comp_names, N);
        //date field
        read_date(file, this->release_date);
        //double fields
        file.read((char *)&this->budget, sizeof(double));
        file.read((char *)&this->revenue, sizeof(double));
        file.read((char *)&this->popularity, sizeof(double));
        file.read((char *)&this->vote_avg, sizeof(double));
        //size_t fields
        file.read((char *)&this->id, sizeof(size_t));
        file.read((char *)&this->runtime, sizeof(size_t));
        file.read((char *)&this->vote_count, sizeof(size_t));
        //bool field
        file.read((char *)&this->adult, sizeof(bool));
}

void movie::write(fstream &file) {
        //char * fields
        file.write(this->title, N);
        file.write(this->org_lang, N);
        file.write(this->org_country, N);
        file.write(this->genre_names, N);
        file.write(this->prod_comp_names, N);
        //date field
        write_date(file, this->release_date);
        //double fields
        file.write((char *)&this->budget, sizeof(double));
        file.write((char *)&this->revenue, sizeof(double));
        file.write((char *)&this->popularity, sizeof(double));
        file.write((char *)&this->vote_avg, sizeof(double));
        //size_t fields
        file.write((char *)&this->id, sizeof(size_t));
        file.write((char *)&this->runtime, sizeof(size_t));
        file.write((char *)&this->vote_count, sizeof(size_t));
        //bool field
        file.write((char *)&this->adult, sizeof(bool));
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
