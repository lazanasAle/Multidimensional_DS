string random_string(size_t length) {
        const string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        random_device rand_dev;
        mt19937 generator(rand_dev());
        uniform_int_distribution<> distribution(0, characters.size() - 1);

        string rand_string;
        for (size_t j = 0; j < length; ++j) {
                size_t index = distribution(generator);
                rand_string += characters[index];
        }
        return rand_string;
}



template<typename T>

point<T>::point(T p) {
        this->name = random_string(7);
        this->location = p;
        this->parent_offset = INV_OFF;
}

template<typename T>

region<T>::region(pair<T, T> &reg) {
        this->region = reg;
        this->child_offset = this->parent_offset = INV_OFF;
}

template<typename T>

kd_bnode<T>::kd_bnode(function<pair<T, T> (vector<T> &)> make_rect) {
        this->maximum_fill = BLC_LEN / sizeof(T);
        this->minimum_fill = (size_t) trunc(MIN_PERC*this->maximum_fill);
        this->make_rectangle = make_rect;
}
