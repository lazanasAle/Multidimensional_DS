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
        this->parent_offset = this->my_offset = INV_OFF;
}

template<typename T>

region<T>::region(rectangle<T> &reg) {
        this->region = reg;
        this->child_offset = this->parent_offset = this->my_offset = INV_OFF;
}

template<typename T>

kd_bnode<T>::kd_bnode(cmp_vector<T> *cmp_vec) {
        this->maximum_fill = BLC_LEN / sizeof(T);
        this->minimum_fill = (size_t) trunc(MIN_PERC * this->maximum_fill);
        this->level = 0;
        this->comparators = cmp_vec;
        this->dim_len = cmp_vec->size();
}


// point node functions

template<typename T>

point_kd_bnode<T>::point_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<T *>)> make_rectangle_fn):
kd_bnode<T>(cmp_vec) {
        this->make_rectangle = make_rectangle_fn;
}

template<typename T>

pair<kd_bnode<T> *, kd_bnode<T> *> point_kd_bnode<T>::split_node() {
        //sort entries in the dimension of interest
        size_t cmp_idx = this->level % this->dim_len;
        stable_sort(this->points.begin(), this->points.end(),
                [this, cmp_idx] (const point<T> &a, const point<T> &b) {
                        int res = (*this->comparators)[cmp_idx](a.location, b.location);
                        return (res < 0);
        });
        //find the median
        size_t median_idx = (this->points.size() - 1) / 2;
        auto median_it = this->points.begin() + median_idx;
        //now split from the median
        vector<point<T>> new_left(this->points.begin(), median_it + 1);
        vector<point<T>> new_right(median_it + 1, this->points.end());
        this->points = move(new_left);
        point_kd_bnode<T> *new_node = new point_kd_bnode<T>(this->comparators, this->make_rectangle);
        new_node->points = move(new_right);
        new_node->level = this->level;

        //return the splitted nodes
        return make_pair(this, new_node);
}

// region node functions

template<typename T>

region_kd_bnode<T>::region_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *>)> make_rectangle_fn):
kd_bnode<T>(cmp_vec) {
        this->make_rectangle = make_rectangle_fn;
}

template<typename T>

pair<kd_bnode<T> *, kd_bnode<T> *> region_kd_bnode<T>::split_node() {
        //sort entries in the dimension of interest
        size_t cmp_idx = this->level % this->dim_len;
        stable_sort(this->regions.begin(), this->regions.end(),
                [this, cmp_idx] (const region<T> &r1, const region<T> &r2) {
                        int res = (*this->comparators)[cmp_idx](get<1>(r1.region, get<1>(r2.region)));
                        return (res < 0);
        });
        //find the median
        size_t median_idx = (this->regions.size() - 1) / 2;
        auto median_it = this->regions.begin() + median_idx;
        //split from the median
        vector<region<T>> left_regions(this->regions.begin(), median_it + 1);
        vector<region<T>> right_regions(median_it + 1, this->regions.end());
        this->regions = move(left_regions);
        region_kd_bnode<T> *new_node = new region_kd_bnode<T>(this->comparators, this->make_rectangle);
        new_node->regions = move(right_regions);
        new_node->level = this->level;

        //return the splitted nodes
        return make_pair(this, new_node);
}
