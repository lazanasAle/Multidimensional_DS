static long end_pos(fstream &file) {
        file.seekp(0, ios::end);
        long end_p = file.tellp();
        return end_p;
}

template<typename T>

void read_vector(fstream &file, vector<T> &vec, size_t it_in_blc) {
        size_t vec_len;
        file.read((char *)&vec_len, sizeof(size_t));
        vec.resize(vec_len);
        for (size_t j = 0; j < vec_len; ++j)
                file.read((char *)&vec[j], sizeof(T));
        size_t type_len = sizeof(T);
        char invalid[type_len] = "0";
        for (size_t j = vec_len; j < it_in_blc; ++j)
                file.read(invalid, type_len);
}

template<typename T>

void write_vector(fstream &file, vector<T> &vec, size_t it_in_blc) {
        size_t vec_len = vec.size();
        file.write((char *)&vec_len, sizeof(size_t));
        for (size_t j = 0; j < vec_len; ++j)
                file.write((char *)&vec[j], sizeof(T));
        size_t type_len = sizeof(T);
        char invalid[type_len] = "0";
        for (size_t j = vec_len; j < it_in_blc; ++j)
                file.write(invalid, type_len);
}

static string random_string(size_t length) {
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

double rect_area(rectangle<T> &r, cmp_vector<T> *cmp_vec) {
        T lower = get<0>(r);
        T upper = get<2>(r);
        double area = 1;
        for (auto &cmp : *cmp_vec) {
                double dim_area = abs(cmp(lower, upper));
                area *= dim_area;
        }
        return area;
}

template<typename T>

point<T>::point(T p) {
        this->name = random_string(7);
        this->location = p;
}

template<typename T>

region<T>::region(rectangle<T> reg) {
        this->region_rec = reg;
        this->child_offset = INV_OFF;
}

template<typename T>

kd_bnode<T>::kd_bnode(cmp_vector<T> *cmp_vec) {
        this->level = 0;
        this->comparators = cmp_vec;
        this->dim_len = cmp_vec->size();
        this->parent_offset = this->my_offset = INV_OFF;
}


// point node functions

template<typename T>

point_kd_bnode<T>::point_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<T *> &)> make_rectangle_fn):
kd_bnode<T>(cmp_vec) {
        this->maximum_fill = BLC_LEN / sizeof(point<T>);
        this->minimum_fill = (size_t) trunc(MIN_PERC * this->maximum_fill);
        this->make_rectangle = make_rectangle_fn;
}

template<typename T>

kd_bnode<T> *point_kd_bnode<T>::split_node() {
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
        return new_node;
}

template<typename T>

bool into_rectangle(pair<T, T> &rect, point<T> &point, cmp_vector<T> *cmp_vec) {
        size_t smaller = 0, bigger = 0;
        size_t dim_len = cmp_vec->size();
        for (auto &cmp : *cmp_vec) {
                if (cmp(rect.first, point.location) <= 0)
                        smaller++;
                if (cmp(rect.second, point.location) >= 0)
                        bigger++;
        }

        return (smaller >= dim_len) && (bigger >= dim_len);
}

// region node functions

template<typename T>

region_kd_bnode<T>::region_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> make_rectangle_fn):
kd_bnode<T>(cmp_vec) {
        this->maximum_fill = BLC_LEN / sizeof(region<T>);
        this->minimum_fill = (size_t) trunc(MIN_PERC * this->maximum_fill);
        this->make_rectangle = make_rectangle_fn;
}

template<typename T>

kd_bnode<T> *region_kd_bnode<T>::split_node() {
        //sort entries in the dimension of interest
        size_t cmp_idx = this->level % this->dim_len;
        stable_sort(this->regions.begin(), this->regions.end(),
                [this, cmp_idx] (const region<T> &r1, const region<T> &r2) {
                        int res = (*this->comparators)[cmp_idx](get<1>(r1.region_rec), get<1>(r2.region_rec));
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
        return new_node;
}


template<typename T>

bool into_rectangle(pair<T, T> &rect, region<T> &reg, cmp_vector<T> *cmp_vec) {
        rectangle<T> my_rect = reg.region_rec;
        size_t intersects = 0;
        size_t dim_len = cmp_vec->size();
        T my_low = get<0>(my_rect);
        T my_high = get<2>(my_rect);
        for (auto &cmp : *cmp_vec) {
                if ((cmp(my_low, rect.second) <= 0) && (cmp(my_high, rect.first) >= 0))
                        intersects++;
        }
        return (intersects >= dim_len);
}

//kd-btree functions

template<typename T>

kd_btree<T>::kd_btree(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
function<rectangle<T> (vector<T *> &)> point_rectangle_fn) {
        this->comparators = cmp_vec;
        this->make_region_rectangle = region_rectangle_fn;
        this->make_point_rectangle = point_rectangle_fn;
        this->file = fstream("records.bin", ios::binary | ios::in | ios::out | ios::trunc);
        this->coffset = 0;
        this->next_offset = 0;
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
                        read_vector<point<T>>(this->file, loaded_point->points, loaded_point->maximum_fill);
                        loaded = loaded_point;
                }
                else {
                        region_kd_bnode<T> *loaded_region = new region_kd_bnode<T>(this->comparators, this->make_region_rectangle);
                        //read characteristics of node
                        this->file.read((char *)&loaded_region->parent_offset, sizeof(long));
                        loaded_region->my_offset = node_offset;
                        this->file.read((char *)&loaded_region->level, sizeof(size_t));
                        read_vector<region<T>>(this->file, loaded_region->regions, loaded_region->maximum_fill);
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
                        write_vector(this->file, pnode->points, pnode->maximum_fill);
                }
                else {
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                        tag t = REGION;
                        this->file.write((char *)&t, sizeof(tag));
                        this->file.write((char *)&rnode->parent_offset, sizeof(long));
                        this->file.write((char *)&rnode->level, sizeof(size_t));
                        write_vector(this->file, rnode->regions, rnode->maximum_fill);
                }
                return true;
        }
        return false;
}

template<typename T>

void kd_btree<T>::update_node_level(kd_bnode<T> *node) {
        if (node->parent_offset > 0) {
                kd_bnode<T> *parent = load_node(node->parent_offset);
                node->level = parent->level + 1;
                delete(parent);
                return;
        }
        node->level = 0;
}

template<typename T>

void kd_btree<T>::range_query_rec(pair<T, T> &rect, vector<T> &vec, long subtree_root_off) {
        if (subtree_root_off >= 0) {
                kd_bnode<T> *node = load_node(subtree_root_off);
                if (typeid(*node) == typeid(point_kd_bnode<T>)) {
                        point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                        for (auto &point : pnode->points) {
                                if (into_rectangle<T>(rect, point, this->comparators))
                                        vec.push_back(point.location);
                        }
                }
                else {
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                        for (auto &region : rnode->regions) {
                                if (into_rectangle<T>(rect, region, this->comparators))
                                        range_query_rec(rect, vec, region.child_offset);
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

template<typename T>

point_kd_bnode<T> *kd_btree<T>::choose_leaf(T &data, long subtree_root_off) {
        if (subtree_root_off > 0) {
                kd_bnode<T> *curr_node = load_node(subtree_root_off);
                if (curr_node) {
                        if (typeid(*curr_node) == typeid(point_kd_bnode<T>)) {
                                //it's a leaf you found it
                                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) curr_node;
                                return pnode;
                        }
                        //unfortunately it's a region node dig deeper
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) curr_node;
                        vector<pair<double, bool>> enlargement(rnode->regions.size());
                        size_t j = 0;
                        for (region<T> &r : rnode->regions) {
                                pair<T, T> region_rect = make_pair(get<0>(r.region_rec), get<2>(r.region_rec));
                                point<T> pt(data);
                                if (into_rectangle<T>(region_rect, pt, this->comparators)) {
                                        //it's in here so go to the child
                                        return choose_leaf(data, r.child_offset);
                                }
                                //find the enlagement the area of this region needs to include the new point
                                T left_side = get<0>(r.region_rec);
                                T right_side = get<2>(r.region_rec);
                                //make the new rectangles
                                vector<T *> v1 = {&left_side, &data};
                                vector<T *> v2 = {&right_side, &data};
                                rectangle<T> r1 = this->make_point_rectangle(v1);
                                rectangle<T> r2 = this->make_point_rectangle(v2);
                                //find areas and enlargement
                                double left_area = rect_area<T>(r1, this->comparators);
                                double right_area = rect_area<T>(r2, this->comparators);
                                enlargement[j] = (left_area < right_area)? make_pair(left_area, true) : make_pair(right_area, false);
                                j++;
                        }
                        //find which region needs minimum enlargement
                        auto it = min_element(enlargement.begin(), enlargement.end(),
                                [] (const auto &a, const auto &b) {
                                        return a.first < b.first;
                        });
                        size_t min_idx = distance(enlargement.begin(), it);
                        rectangle<T> new_rect;
                        //enlarge that region
                        if (enlargement[min_idx].second) {
                                T right_side = get<2>(rnode->regions[min_idx].region_rec);
                                vector<T *> v = {&right_side, &data};
                                new_rect = this->make_point_rectangle(v);
                        }
                        else {
                                T left_side = get<0>(rnode->regions[min_idx].region_rec);
                                vector<T *> v = {&left_side, &data};
                                new_rect = this->make_point_rectangle(v);
                        }
                        rnode->regions[min_idx].region_rec = new_rect;
                        store_node(subtree_root_off, rnode);
                        delete(curr_node);
                        //now it's in here go to the child
                        return choose_leaf(data, rnode->regions[min_idx].child_offset);
                }
                return nullptr;
        }
        return nullptr;
}

template<typename T>

region<T> kd_btree<T>::make_parent_region(kd_bnode<T> *node) {
        rectangle<T> my_new_rect;
        if (typeid(*node) == typeid(point_kd_bnode<T>)) {
                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                //make the new vector of points
                vector<T *> point_locs;
                for (point<T> &p : pnode->points)
                        point_locs.push_back(&p.location);
                my_new_rect = this->make_point_rectangle(point_locs);
        }
        else {
                region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                //make the new vector of regions
                vector<rectangle<T> *> my_regs;
                for (region<T> &r : rnode->regions)
                        my_regs.push_back(&r.region_rec);
                my_new_rect = this->make_region_rectangle(my_regs);
        }
        region<T> splitted_parent(my_new_rect);
        return splitted_parent;
}

template<typename T>

void kd_btree<T>::update_chld_levels(region_kd_bnode<T> *node) {
        for (region<T> &reg : node->regions) {
                kd_bnode<T> *chld_node = load_node(reg.child_offset);
                update_node_level(chld_node);
                store_node(chld_node->my_offset, chld_node);
                delete(chld_node);
        }
}

template<typename T>

void kd_btree<T>::propagate_split(kd_bnode<T> *org_node, kd_bnode<T> *split_org_node) {
        size_t splitted_off = this->next_offset;
        split_org_node->my_offset = (split_org_node->my_offset < 0)? splitted_off : split_org_node->my_offset;
        region<T> splitted_parent = make_parent_region(split_org_node);
        splitted_parent.child_offset = splitted_off;
        if (org_node->parent_offset < 0) {
                region<T> org_parent = make_parent_region(org_node);
                org_parent.child_offset = org_node->my_offset;
                region_kd_bnode<T> *parent_node = new region_kd_bnode<T>(this->comparators, this->make_region_rectangle);
                parent_node->regions.assign({org_parent, splitted_parent});
                store_node(split_org_node->my_offset, split_org_node);
                //find father offset
                long end_offset = end_pos(this->file);
                this->next_offset = (this->coffset != this->next_offset)? this->coffset : end_offset;
                this->coffset = this->next_offset;
                //put the father were you should
                store_node(this->coffset, parent_node);
                //inform the children where the parent is
                org_node->parent_offset = this->coffset;
                split_org_node->parent_offset = this->coffset;
                update_node_level(org_node);
                update_node_level(split_org_node);
                store_node(org_node->my_offset, org_node);
                store_node(split_org_node->my_offset, split_org_node);
                // make the parent new root
                this->root_offset = this->coffset;
                //move the offset to the next position
                this->coffset = this->next_offset = end_pos(this->file);
                delete(parent_node);
        }
        else {
                region_kd_bnode<T> *par_node = (region_kd_bnode<T> *) load_node(org_node->parent_offset);
                par_node->regions.push_back(splitted_parent);
                store_node(split_org_node->my_offset, split_org_node);
                //now deal with the father and maybe propagate the split
                size_t vlen = par_node->regions.size();
                size_t dlen = this->comparators->size();
                if (vlen > par_node->maximum_fill) {
                        region_kd_bnode<T> *neighbour = (region_kd_bnode<T> *) par_node->split_node();
                        store_node(par_node->my_offset, par_node);
                        //store the neighbour
                        this->coffset = end_pos(this->file);
                        neighbour->my_offset = this->coffset;
                        store_node(this->coffset, neighbour);
                        //determine who is the father, then store it acordingly
                        bool in_me = binary_search(par_node->regions.begin(), par_node->regions.end(), splitted_parent,
                                [this, dlen, par_node] (const region<T> &a, const region<T> &b) {
                                        int res = (*this->comparators)[par_node->level % dlen](get<1>(a.region_rec), get<1>(b.region_rec));
                                        return (res < 0);
                        });
                        split_org_node->parent_offset = (in_me)? par_node->my_offset : neighbour->my_offset;
                        propagate_split(par_node, neighbour);
                        //update the level in all children
                        update_chld_levels(par_node);
                        //do the same for the neighbour
                        update_chld_levels(neighbour);
                        delete(neighbour);
                }
                else {
                        store_node(par_node->my_offset, par_node);
                        split_org_node->parent_offset = par_node->my_offset;
                        update_node_level(split_org_node);
                        store_node(split_org_node->my_offset, split_org_node);
                }
                delete(par_node);
        }
        this->coffset = end_pos(this->file);
}


template<typename T>

void kd_btree<T>::insert_rec(T &data, long subtree_root_off) {
        point_kd_bnode<T> *leaf = choose_leaf(data, subtree_root_off);
        leaf->points.push_back(data);
        store_node(leaf->my_offset, leaf);
        size_t vlen = leaf->points.size();
        if (vlen > leaf->maximum_fill) {
                point_kd_bnode<T> *neighbour =  (point_kd_bnode<T> *) leaf->split_node();
                store_node(leaf->my_offset, leaf);
                propagate_split(leaf, neighbour);
        }
        delete(leaf);
}


template<typename T>

void kd_btree<T>::insert(T &data) {
        if (this->root_offset >= 0)
                insert_rec(data, this->root_offset);
        else {
                point_kd_bnode<T> new_root(this->comparators, this->make_point_rectangle);
                new_root.points.push_back(data);
                update_node_level(&new_root);
                new_root.my_offset = this->next_offset;
                store_node(new_root.my_offset, &new_root);
                this->root_offset = new_root.my_offset;
        }
        this->coffset = this->next_offset = end_pos(this->file);
}
