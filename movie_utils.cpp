#include "movie_utils.hpp"

void movie::read(fstream &file) {
        //char * fields
        file.read(this->title, N);
        file.read(this->org_lang, N);
        file.read(this->org_country, N);
        file.read(this->genre_names, N);
        file.read(this->prod_comp_names, N);
        //date field
        char rel_date[12];
        file.read(rel_date, 12);
        string str_reldate(rel_date);
        this->release_date = parse_date(str_reldate);
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
        char rel_date[12];
        string s = format("{}", this->release_date);
        strcpy(rel_date, s.c_str());
        file.write(rel_date, 12);
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
