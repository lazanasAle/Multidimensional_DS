template<typename T>

void read_vector(fstream file, vector<T> &vec) {
        size_t vec_len;
        file.read((char *)&vec_len, sizeof(size_t));
        vec.set_size(vec_len);
        for (size_t j = 0; j < vec_len; ++j)
                file.read((char *)&vec[j], sizeof(T));
}

template<typename T>

void write_vector(fstream file, vector<T> &vec) {
        size_t vec_len = vec.size();
        file.write((char *)&vec_len, sizeof(size_t));
        for (size_t j = 0; j < vec_len; ++j)
                file.write((char *)&vec[j], sizeof(T));
}

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
}

template<typename T>

region<T>::region(rectangle<T> &reg) {
        this->region = reg;
        this->child_offset = INV_OFF;
}

template<typename T>

kd_bnode<T>::kd_bnode(cmp_vector<T> *cmp_vec) {
        this->maximum_fill = BLC_LEN / sizeof(T);
        this->minimum_fill = (size_t) trunc(MIN_PERC * this->maximum_fill);
        this->level = 0;
        this->comparators = cmp_vec;
        this->dim_len = cmp_vec->size();
        this->parent_offset = this->my_offset = INV_OFF;
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

template<typename T>

bool point_kd_bnode<T>::into_rectangle(pair<T, T> &rect, point<T> &point) {
        size_t smaller = 0, bigger = 0;
        for (auto &cmp : *(this->comparators)) {
                if (cmp(rect.first, point) <= 0)
                        smaller++;
                if (cmp(rect.second, point) >= 0)
                        bigger++;
        }
        return (smaller >= this->dim_len) && (bigger >= this->dim_len);
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


template<typename T>

bool region_kd_bnode<T>::into_rectangle(pair<T, T> &rect, region<T> &reg) {
        rectangle<T> my_rect = reg.region;
        size_t intersects = 0;
        T my_low = get<0>(my_rect);
        T my_high = get<2>(my_rect);
        for (auto &cmp : *(this->comparators)) {
                if ((cmp(my_low, rect.second) <= 0) && (cmp(my_high, rect.first) >= 0))
                        intersects++;
        }
        return (intersects >= this->dim_len);
}

//kd-btree functions

template<typename T>

kd_btree<T>::kd_btree(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *>)> region_rectangle_fn,
function<rectangle<T> (vector<T *>)> point_rectangle_fn) {
        this->comparators = cmp_vec;
        this->make_region_rectangle = region_rectangle_fn;
        this->make_point_rectangle = point_rectangle_fn;
        this->file = fstream("records.bin", ios::binary | ios::in | ios::out | ios::trunc);
        this->coffset = 0;
        this->next_offset = this->coffset;
        this->root_offset = INV_OFF;
}

template<typename T>

bool kd_btree<T>::empty() {
        return (this->root_offset <= INV_OFF);
}

template<typename T>

kd_bnode<T> *kd_btree<T>::load_node(size_t node_offset) {
        if (this->file.is_open()) {
                this->file.seekp(node_offset, ios::beg);
                tag t;
                this->file.read((char *)&t, sizeof(tag));
                kd_bnode<T> *loaded;
                if (t == POINT) {
                        point_kd_bnode<T> *loaded_point = new point_kd_bnode<T>(this->comparators, this->make_point_rectangle);
                        // read characteristics of node
                        this->file.read((char *)&loaded_point->parent_offset, sizeof(long));
                        loaded_point->my_offset = node_offset;
                        this->file.read((char *)&loaded_point->level, sizeof(size_t));
                        read_vector<region<T>>(this->file, loaded_point->points);
                        loaded = loaded_point;
                }
                else {
                        region_kd_bnode<T> *loaded_region = new region_kd_bnode<T>(this->comparators, this->make_region_rectangle);
                        //read characteristics of node
                        this->file.read((char *)&loaded_region->parent_offset, sizeof(long));
                        loaded_region->my_offset = node_offset;
                        this->file.read((char *)&loaded_region->level, sizeof(size_t));
                        read_vector<region<T>>(this->file, loaded_region->regions);
                        loaded = loaded_region;
                }
                return loaded;
        }
        return nullptr;
}

template<typename T>

bool kd_btree<T>::store_node(size_t node_offset, kd_bnode<T> *node) {
        if (this->file.is_open()) {
                node->my_offset = node_offset;
                this->file.seekp(node_offset, ios::beg);
                if (typeid(*node) == typeid(point_kd_bnode<T>)) {
                        point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                        tag t = POINT;
                        this->file.write((char *)&t, sizeof(tag));
                        this->file.write((char *)&pnode->parent_offset, sizeof(long));
                        this->file.write((char *)&pnode->level, sizeof(size_t));
                        write_vector(this->file, pnode->points);
                }
                else {
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                        tag t = REGION;
                        this->file.write((char *)&t, sizeof(tag));
                        this->file.write((char *)&rnode->parent_offset, sizeof(long));
                        this->file.write((char *)&rnode->level, sizeof(size_t));
                        write_vector(this->file, rnode->regions);
                }
                return true;
        }
        return false;
}

template<typename T>

void kd_btree<T>::update_node_level(kd_bnode<T> &node) {
        if (node->parent_offset > 0) {
                kd_bnode<T> *parent = load_node(node->parent_offset);
                node->level = parent->level + 1;
        }
        node->level = 0;
}

template<typename T>

void kd_btree<T>::range_query_rec(pair<T, T> &rect, vector<T> &vec, long subtree_root_off) {
        if (subtree_root_off > 0) {
                kd_bnode<T> *node = load_node(subtree_root_off);
                if (typeid(*node) == typeid(point_kd_bnode<T>)) {
                        point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                        for (auto &point : pnode->points) {
                                if (pnode->into_rectangle(rect, point))
                                        vec.push_back(point.location);
                        }
                }
                else {
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                        for (auto &region : rnode->regions) {
                                if (rnode->into_rectangle(rect, region))
                                        range_query_rec(rect, vec, region->child_offset);
                        }
                }
        }
}

template<typename T>

vector<T> kd_btree<T>::range_query(pair<T, T> &rect) {
        vector<T> vec;
        range_query_rec(rect, vec, root_offset);
        return vec;
}
