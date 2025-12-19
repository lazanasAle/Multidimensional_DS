
static long end_pos(fstream &file) {
        file.seekp(0, ios::end);
        long end_p = file.tellp();
        return end_p;
}

template <typename T, typename U>

vector<T> get_map_keys(map<T, U> &mapper) {
        vector<T> keys;

        for (auto it = mapper.begin(); it != mapper.end(); ++it)
                keys.push_back((*it).first);
        return keys;
}

template <typename T>

void read_vector(fstream &file, vector<T> &vec, size_t it_in_blc) {
        long off = file.tellg();
        if (off >= 0) {
                size_t vec_len;
                if (file.read((char *)&vec_len, sizeof(size_t))) {
                        vec.resize(vec_len);
                        file.read(reinterpret_cast<char *>(vec.data()), vec_len * sizeof(T));
                        char inv[sizeof(T)] = {0};
                        for (size_t j = vec_len; j < it_in_blc + 1; ++j)
                                file.read(inv, sizeof(T));
                }
        }
}

template <typename T>

void write_vector(fstream &file, vector<T> &vec, size_t it_in_blc) {
        long off = file.tellp();
        if (off >= 0) {
                size_t vec_len = vec.size();
                file.write((char *)&vec_len, sizeof(size_t));
                file.write(reinterpret_cast<char *>(vec.data()), vec_len * sizeof(T));
                char inv[sizeof(T)] = {0};
                for (size_t j = vec_len; j < it_in_blc + 1; ++j)
                        file.write(inv, sizeof(T));
        }
}

template <typename T>

void read_rectangle(rectangle<T> &rect, fstream &file) {
        get<0>(rect).read(file);
        get<1>(rect).read(file);
        get<2>(rect).read(file);
}

template <typename T>

void write_rectangle(rectangle<T> &rect, fstream &file) {
        get<0>(rect).write(file);
        get<1>(rect).write(file);
        get<2>(rect).write(file);
}


template <typename T>

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

template <typename T>

point<T>::point(T p) {
        this->location = p;
}

template <typename T>

void point<T>::read(fstream &file) {
        location.read(file);
}

template <typename T>

void point<T>::write(fstream &file) {
        location.write(file);
}

template <typename T>

region<T>::region(rectangle<T> reg) {
        this->region_rec = reg;
        this->child_offset = INV_OFF;
}

template <typename T>

void region<T>::read(fstream &file) {
        file.read((char *)&this->child_offset, sizeof(long));
        read_rectangle(this->region_rec, file);
}

template <typename T>

void region<T>::write(fstream &file) {
        file.write((char *)&this->child_offset, sizeof(long));
        write_rectangle(this->region_rec, file);
}

template <typename T>

kd_bnode<T>::kd_bnode(cmp_vector<T> *cmp_vec) {
        this->level = 0;
        this->comparators = cmp_vec;
        this->dim_len = cmp_vec->size();
        this->parent_offset = this->my_offset = INV_OFF;
}

template <typename T>

void kd_bnode<T>::read_common(fstream &file, long node_offset) {
        file.read((char *)&this->parent_offset, sizeof(long));
        this->my_offset = node_offset;
        file.read((char *)&this->level, sizeof(size_t));
}
// point node functions

template <typename T>

point_kd_bnode<T>::point_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<T *> &)> make_rectangle_fn):
kd_bnode<T>(cmp_vec) {
        this->tag = false;
        this->maximum_fill = BLC_LEN / sizeof(point<T>);
        this->minimum_fill = (size_t) trunc(MIN_PERC * this->maximum_fill);
        this->make_rectangle = make_rectangle_fn;
}

template <typename T>

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

template <typename T>

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

template <typename T>

region_kd_bnode<T>::region_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> make_rectangle_fn):
kd_bnode<T>(cmp_vec) {
        this->tag = true;
        this->maximum_fill = BLC_LEN / sizeof(region<T>);
        this->minimum_fill = (size_t) trunc(MIN_PERC * this->maximum_fill);
        this->make_rectangle = make_rectangle_fn;
}

template <typename T>

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


template <typename T>

bool into_rectangle(pair<T, T> &rect, region<T> &reg, cmp_vector<T> *cmp_vec) {
        rectangle<T> my_rect = reg.region_rec;
        size_t intersects = 0;
        size_t dim_len = cmp_vec->size();
        T my_low = get<0>(my_rect);
        T my_high = get<2>(my_rect);
        for (size_t j = 0; j < dim_len; ++j) {
                if (((*cmp_vec)[j](my_low, rect.second) <= 0) && ((*cmp_vec)[j](my_high, rect.first) >= 0)) {
                        intersects++;
                }
        }
        return (intersects >= dim_len);
}

//kd-btree functions

template <typename T, typename C>

kd_btree<T, C>::kd_btree(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
function<rectangle<T> (vector<T *> &)> point_rectangle_fn) {
        this->nitems = 0;
        this->comparators = cmp_vec;
        this->make_region_rectangle = region_rectangle_fn;
        this->make_point_rectangle = point_rectangle_fn;
        this->file = fstream("records.bin", ios::binary | ios::in | ios::out | ios::trunc);
        this->coffset = 0;
        this->recent_cnt = 0;
        this->max_cached_rnodes = MAX_CACHED;
        this->max_cached_pnodes = MAX_CACHED / 2;
        this->root_offset = INV_OFF;
}

template <typename T, typename C>

bool kd_btree<T, C>::empty() {
        return (this->root_offset <= INV_OFF);
}

template <typename T, typename C>

kd_bnode<T> *kd_btree<T, C>::load_file_node(long node_offset) {
        if (this->file.is_open() && node_offset >= 0) {
                this->file.seekg(node_offset, ios::beg);
                bool t;
                this->file.read((char *)&t, sizeof(bool));
                // here i cannot do it (read the common and then differentiate) it is unsafe
                kd_bnode<T> *loaded;
                if (!t) {
                        point_kd_bnode<T> *loaded_point = new point_kd_bnode<T>(this->comparators, this->make_point_rectangle);
                        // read characteristics of node
                        loaded_point->read_common(this->file, node_offset);
                        read_vector<point<T>>(this->file, loaded_point->points, loaded_point->maximum_fill);
                        loaded = loaded_point;
                }
                else {
                        region_kd_bnode<T> *loaded_region = new region_kd_bnode<T>(this->comparators, this->make_region_rectangle);
                        //read characteristics of node
                        loaded_region->read_common(this->file, node_offset);
                        read_vector<region<T>>(this->file, loaded_region->regions, loaded_region->maximum_fill);
                        loaded = loaded_region;
                }
                return loaded;
        }
        return nullptr;
}

template <typename T, typename C>

bool kd_btree<T, C>::store_file_node(long node_offset, kd_bnode<T> *node) {
        if (this->file.is_open() && node_offset >= 0) {
                node->my_offset = node_offset;
                //write the common fields
                this->file.seekp(node_offset, ios::beg);
                this->file.write((char *)&node->tag, sizeof(bool));
                this->file.write((char *)&node->parent_offset, sizeof(long));
                this->file.write((char *)&node->level, sizeof(size_t));
                //diferentiate
                if (!node->tag) {
                        point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                        write_vector<point<T>>(this->file, pnode->points, pnode->maximum_fill);
                }
                else {
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                        write_vector<region<T>>(this->file, rnode->regions, rnode->maximum_fill);
                }
                return true;
        }
        return false;
}

template <typename T, typename C>

void kd_btree<T, C>::evict_point_cache() {
        size_t cache_len = this->pnodes_cached.size();
        if (cache_len >= this->max_cached_pnodes) {
                auto min_it = min_element(this->off_cnt_points.begin(), this->off_cnt_points.end(),
                        [](const pair<long, size_t> &a, const pair<long, size_t> &b) {
                                return (a.second < b.second);
                }); //the minimum second value is the minimum timestamp (least recently used) this shall be removed
                long rem_key = min_it->first;
                auto rem_it = this->pnodes_cached.find(rem_key);
                store_file_node(rem_key, &rem_it->second);
                this->pnodes_cached.erase(rem_key);
                this->off_cnt_points.erase(rem_key); //erase it from the lru map as well
        }
}

template <typename T, typename C>

void kd_btree<T, C>::evict_region_cache() {
        size_t cache_len = this->rnodes_cached.size();
        if (cache_len >= this->max_cached_rnodes) {
                auto min_it = min_element(this->off_cnt_regions.begin(), this->off_cnt_regions.end(),
                        [](const pair<long, size_t> &a, const pair<long, size_t> &b) {
                                return (a.second < b.second);
                }); //the minimum second value is the minimum timestamp (least recently used) this shall be removed
                long rem_key = min_it->first;
                auto rem_it = this->rnodes_cached.find(rem_key);
                store_file_node(rem_key, &rem_it->second);
                this->rnodes_cached.erase(rem_key);
                this->off_cnt_regions.erase(rem_key); //erase it from the lru map as well
        }
}

template <typename T, typename C>

kd_bnode<T> *kd_btree<T, C>::load_node(long node_offset) {
        if (node_offset >= 0) {
                //look in first cache (it could be a leaf)
                auto it1 = this->pnodes_cached.find(node_offset);
                if (it1 != this->pnodes_cached.end()) {
                        point_kd_bnode<T> *pnode = new point_kd_bnode<T>(this->comparators, this->make_point_rectangle);
                        *pnode = it1->second;
                        this->off_cnt_points.insert_or_assign(node_offset, this->recent_cnt); //update the lru map that we found it in this timestamp
                        this->recent_cnt++; //increase current timestamp
                        return pnode;
                }
                //look in second cache (could be a region node)
                auto it2 = this->rnodes_cached.find(node_offset);
                if (it2 !=  this->rnodes_cached.end()) {
                        region_kd_bnode<T> *rnode = new region_kd_bnode<T>(this->comparators, this->make_region_rectangle);
                        *rnode = it2->second;
                        this->off_cnt_regions.insert_or_assign(node_offset, this->recent_cnt); //update the lru map that we found it in this timestamp
                        this->recent_cnt++; //increase current timestamp
                        return rnode;
                }
                // it is not cached so bring it from the file
                kd_bnode<T> *nnode = load_file_node(node_offset);
                //check what it is and put it in the right cache
                if (nnode) {
                        if (nnode->tag) {
                                region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) nnode;
                                evict_region_cache();
                                this->rnodes_cached.insert_or_assign(node_offset, *rnode);
                                this->off_cnt_regions.insert_or_assign(node_offset, this->recent_cnt); //update the lru map that we found it in this timestamp
                        }
                        else {
                                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) nnode;
                                evict_point_cache();
                                this->pnodes_cached.insert_or_assign(node_offset, *pnode);
                                this->off_cnt_points.insert_or_assign(node_offset, this->recent_cnt); //update the lru map that we found it in this timestamp
                        }
                        this->recent_cnt++; //increase current timestamp
                        return nnode;
                }
                return nullptr;
        }
        return nullptr;
}

template <typename T, typename C>

bool kd_btree<T, C>::store_node(long node_offset, kd_bnode<T> *node) {
        if (node_offset >= 0 && node) {
                if (node->tag) {
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                        auto it = this->rnodes_cached.find(node_offset);
                        if (it != this->rnodes_cached.end())
                                it->second = *rnode;
                        else {
                                //put it in cache
                                evict_region_cache();
                                this->rnodes_cached.insert_or_assign(node_offset, *rnode);

                                long end_off = end_pos(this->file);
                                if (node_offset >= end_off) // it is not in the file we put it
                                        store_file_node(node_offset, node);
                        }
                        this->off_cnt_regions.insert_or_assign(node_offset, this->recent_cnt); //update the lru map that we found it in this timestamp
                }
                else {
                        point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                        auto it = this->pnodes_cached.find(node_offset);
                        if (it != this->pnodes_cached.end())
                                it->second = *pnode;
                        else {
                                //put it in cache
                                evict_point_cache();
                                this->pnodes_cached.insert_or_assign(node_offset, *pnode);

                                long end_off = end_pos(this->file);
                                if (node_offset >= end_off) // it is not in the file we put it
                                        store_file_node(node_offset, node);
                        }
                        this->off_cnt_points.insert_or_assign(node_offset, this->recent_cnt); //update the lru map that we found it in this timestamp
                }
                this->recent_cnt++; //increase current timestamp
                return true;
        }
        return false;
}

template <typename T, typename C>

void kd_btree<T, C>::update_node_level(kd_bnode<T> *node) {
        if (node->parent_offset >= 0) {
                kd_bnode<T> *parent = load_node(node->parent_offset);
                if (parent) {
                        node->level = parent->level + 1;
                        delete(parent);
                }
                return;
        }
        node->level = 0;
}

template <typename T, typename C>

void kd_btree<T, C>::range_query_rec(pair<T, T> &rect, vector<T> &vec, long subtree_root_off) {
        if (subtree_root_off >= 0) {
                kd_bnode<T> *node = load_node(subtree_root_off);
                if (node) {
                        if (!node->tag) {
                                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                                for (auto &point : pnode->points) {
                                        if (into_rectangle<T>(rect, point, this->comparators))
                                                vec.push_back(point.location);
                                }
                        }
                        else {
                                region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                                for (auto &region : rnode->regions) {
                                        if (into_rectangle<T>(rect, region, this->comparators)) {
                                                range_query_rec(rect, vec, region.child_offset);
                                        }
                                }
                        }
                        delete(node);
                }
        }
}

template <typename T, typename C>

vector<T> kd_btree<T, C>::range_query(pair<T, T> &rect) {
        vector<T> vec;
        range_query_rec(rect, vec, root_offset);
        return vec;
}

template <typename T, typename C>

point_kd_bnode<T> *kd_btree<T, C>::choose_leaf(T &data, long subtree_root_off) {
        if (subtree_root_off >= 0) {
                kd_bnode<T> *curr_node = load_node(subtree_root_off);
                if (curr_node) {
                        if (!curr_node->tag) {
                                //it's a leaf you found it
                                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) curr_node;
                                return pnode;
                        }
                        //unfortunately it's a region node dig deeper
                        region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) curr_node;
                        vector<pair<double, rectangle<T>>> enlargement(rnode->regions.size());
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
                                vector<T *> v = {&left_side, &data, &right_side};
                                rectangle<T> r_new = this->make_point_rectangle(v);
                                //find areas and enlargement
                                double curr_area = rect_area<T>(r.region_rec, this->comparators);
                                double new_area = rect_area<T>(r_new, this->comparators);
                                enlargement[j] = make_pair(new_area - curr_area, r_new);
                                j++;
                        }
                        //find which region needs minimum enlargement
                        auto it = min_element(enlargement.begin(), enlargement.end(),
                                [] (const auto &a, const auto &b) {
                                        return a.first < b.first;
                        });
                        size_t min_idx = distance(enlargement.begin(), it);
                        //enlarge it
                        rnode->regions[min_idx].region_rec = enlargement[min_idx].second;
                        store_node(subtree_root_off, rnode);
                        //now it's in here go to the child
                        point_kd_bnode<T> *leaf = choose_leaf(data, rnode->regions[min_idx].child_offset);
                        delete(curr_node);
                        return leaf;
                }
                return nullptr;
        }
        return nullptr;
}

template <typename T, typename C>

region<T> kd_btree<T, C>::make_parent_region(kd_bnode<T> *node) {
        rectangle<T> my_new_rect;
        if (!node->tag) {
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

template <typename T, typename C>

void kd_btree<T, C>::update_chld_levels(region_kd_bnode<T> *node) {
        for (region<T> &reg : node->regions) {
                kd_bnode<T> *chld_node = load_node(reg.child_offset);
                if (chld_node) {
                        update_node_level(chld_node);
                        store_node(chld_node->my_offset, chld_node);
                        delete(chld_node);
                }
        }
}

template <typename T, typename C>

void kd_btree<T, C>::make_and_store_parent(region<T> &org_parent, region<T> &splitted_par,
        kd_bnode<T> *org_node, kd_bnode<T> *splitted_node) {
                region_kd_bnode<T> *parent_node = new region_kd_bnode<T>(this->comparators, this->make_region_rectangle);
                parent_node->regions.push_back(org_parent);
                parent_node->regions.push_back(splitted_par);
                store_node(splitted_node->my_offset, splitted_node);
                //find father offset
                long end_offset = next_pos(true);
                this->coffset = end_offset;
                //put the father were you should
                parent_node->my_offset = this->coffset;
                store_node(this->coffset, parent_node);
                //inform the children where the parent is
                org_node->parent_offset = this->coffset;
                splitted_node->parent_offset = this->coffset;
                //update the changes to the file
                update_node_level(org_node);
                update_node_level(splitted_node);
                store_node(org_node->my_offset, org_node);
                store_node(splitted_node->my_offset, splitted_node);
                // make the parent new root
                this->root_offset = this->coffset;
                //move the offset to the next position
                this->coffset = end_pos(this->file);
                delete(parent_node);
        }

template <typename T, typename C>

void kd_btree<T, C>::assign_new_region(region_kd_bnode<T> *par_node, rectangle<T> &new_region_rect, long searching_off) {
        for (region<T> &r : par_node->regions) {
                if (r.child_offset == searching_off)
                        r.region_rec = new_region_rect;
        }
        //store potential changes to the parent node
        store_node(par_node->my_offset, par_node);
}

template <typename T, typename C>

void kd_btree<T, C>::store_neighbour_after_split(region_kd_bnode<T> *neighbour, region_kd_bnode<T> *par_node,
        size_t dlen, region<T> &splitted_parent, kd_bnode<T> *splitted_node) {
                this->coffset = next_pos(true);
                neighbour->my_offset = this->coffset;
                store_node(this->coffset, neighbour);
                //determine who is the father, then store it acordingly
                bool in_me = binary_search(par_node->regions.begin(), par_node->regions.end(), splitted_parent,
                        [this, dlen, par_node] (const region<T> &a, const region<T> &b) {
                                int res = (*this->comparators)[par_node->level % dlen](get<1>(a.region_rec), get<1>(b.region_rec));
                                return (res < 0);
                        });
                splitted_node->parent_offset = (in_me)? par_node->my_offset : neighbour->my_offset;
                //in its children change the father offset (to inform them for the change of the father)
                for (region<T> &reg : par_node->regions) {
                        kd_bnode<T> *current_chld = load_node(reg.child_offset);
                        current_chld->parent_offset = neighbour->my_offset;
                        store_node(current_chld->my_offset, current_chld);
                        delete(current_chld);
                }
}

template <typename T, typename C>

void kd_btree<T, C>::propagate_split(kd_bnode<T> *org_node, kd_bnode<T> *split_org_node) {
        size_t splitted_off = next_pos(org_node->tag);
        split_org_node->my_offset = (split_org_node->my_offset < 0)? splitted_off : split_org_node->my_offset;
        //find the new parent region after the splitting (for the new node)
        region<T> splitted_parent = make_parent_region(split_org_node);
        splitted_parent.child_offset = split_org_node->my_offset;
        //find the new parent region after the splitting (for the orignal node)
        region<T> org_parent = make_parent_region(org_node);
        //if there is no parent node we make one
        if (org_node->parent_offset < 0) {
                org_parent.child_offset = org_node->my_offset;
                make_and_store_parent(org_parent, splitted_parent,
                        org_node, split_org_node);
        }
        else {
                region_kd_bnode<T> *par_node = (region_kd_bnode<T> *) load_node(org_node->parent_offset);
                if (par_node) {
                        //since there is a parent node we must find the original's parent and update it
                        assign_new_region(par_node, org_parent.region_rec, org_node->my_offset);
                        //insert the new parent region found
                        par_node->regions.push_back(splitted_parent);
                        store_node(split_org_node->my_offset, split_org_node);
                        //now deal with the father and maybe propagate the split
                        size_t vlen = par_node->regions.size();
                        size_t dlen = this->comparators->size();
                        if (vlen > par_node->maximum_fill) {
                                region_kd_bnode<T> *neighbour = (region_kd_bnode<T> *) split_node(par_node);
                                store_node(par_node->my_offset, par_node);
                                //store the neighbour
                                store_neighbour_after_split(neighbour, par_node, dlen,
                                        splitted_parent, split_org_node);
                                //upward the split
                                propagate_split(par_node, neighbour);
                                //update the level in all children
                                update_chld_levels(par_node);
                                //do the same for the neighbour
                                update_chld_levels(neighbour);
                                store_node(split_org_node->my_offset, split_org_node);
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
        }
        this->coffset = end_pos(this->file);
}


template <typename T, typename C>

void kd_btree<T, C>::insert_rec(T &data, long subtree_root_off) {
        point_kd_bnode<T> *leaf = choose_leaf(data, subtree_root_off);
        leaf->points.push_back(data);
        store_node(leaf->my_offset, leaf);
        size_t vlen = leaf->points.size();
        if (vlen > leaf->maximum_fill) {
                point_kd_bnode<T> *neighbour =  (point_kd_bnode<T> *) split_node(leaf);
                store_node(leaf->my_offset, leaf);
                propagate_split(leaf, neighbour);
        }
        delete(leaf);
}

template <typename T, typename C>

void kd_btree<T, C>::insert(T &data) {
        if (this->root_offset >= 0)
                insert_rec(data, this->root_offset);
        else {
                point_kd_bnode<T> new_root(this->comparators, this->make_point_rectangle);
                new_root.points.push_back(data);
                update_node_level(&new_root);
                new_root.my_offset = this->coffset;
                store_node(new_root.my_offset, &new_root);
                this->root_offset = new_root.my_offset;
        }
        this->nitems++;
        this->coffset = end_pos(this->file);
}

template <typename T>

static inline bool point_dimention_domination(point<T> &p1, point<T> &p2, function<double (const T &, const T &)> cmp, best_t best_dim) {
        double res = cmp(p1.location, p2.location);
        bool value = (best_dim == MAXIMIZE)? (res > 0) : (res < 0);
        return value;
}

template <typename T>

bool dominates(point<T> &p1, point<T> &p2, cmp_vector<T> *cmp_vec, vector<best_t> &best) {
        size_t dom_count = 0, worse_count = 0;
        size_t cmp_len = cmp_vec->size();
        for (size_t j = 0; j < cmp_len; ++j) {
                bool dominated = point_dimention_domination<T>(p1, p2, (*cmp_vec)[j], best[j]);
                if (dominated)
                        dom_count++;
                bool reverse_dominated = point_dimention_domination<T>(p2, p1, (*cmp_vec)[j], best[j]);
                if (reverse_dominated)
                        worse_count++;
        }
        return (dom_count >= 1 ) && (worse_count == 0);
}

template <typename T>

static inline bool region_dimention_domination(region<T> &r1, region<T> &r2, function<double (const T &, const T &)> cmp, best_t best_dim) {
        T r1_low = get<0>(r1.region_rec);
        T r1_high = get<2>(r1.region_rec);

        T r2_low = get<0>(r2.region_rec);
        T r2_high = get<2>(r2.region_rec);

        double res1 = cmp(r1_low, r2_low);
        double res2 = cmp(r1_high, r2_high);

        bool value = (best_dim == MAXIMIZE)? (res2 > 0) : (res1 < 0);
        return value;
}

template <typename T>

bool dominates(region<T> &r1, region<T> &r2, cmp_vector<T> *cmp_vec, vector<best_t> &best) {
        size_t dom_count = 0, worse_count = 0;
        size_t cmp_len = cmp_vec->size();
        for (size_t j = 0; j < cmp_len; ++j) {
                bool dominated = region_dimention_domination<T>(r1, r2, (*cmp_vec)[j], best[j]);
                if (dominated)
                        dom_count++;
                bool reverse_dominated = region_dimention_domination<T>(r2, r1, (*cmp_vec)[j], best[j]);
                if (reverse_dominated)
                        worse_count++;
        }
        return (dom_count >= 1) && (worse_count == 0);
}

template <typename T>

static inline bool region_point_dimention_domination(region<T> &r, point<T> &p, function<double (const T &, const T &)> cmp, best_t best_dim) {
        T r_low = get<0>(r.region_rec);
        T r_high = get<2>(r.region_rec);

        double res1 = cmp(r_low, p.location);
        double res2 = cmp(r_high, p.location);

        bool value = (best_dim == MAXIMIZE)? (res2 > 0) : (res1 < 0);
        return value;
}

template <typename T>

static inline bool point_region_dimention_domination(point<T> &p, region<T> &r, function<double (const T &, const T &)> cmp, best_t best_dim) {
        T r_low = get<0>(r.region_rec);
        T r_high = get<2>(r.region_rec);

        double res1 = cmp(p.location, r_low);
        double res2 = cmp(p.location, r_high);

        bool value = (best_dim == MAXIMIZE)? (res2 > 0) : (res1 < 0);
        return value;
}


template <typename T>

bool dominates(point<T> &p, region<T> &r, cmp_vector<T> *cmp_vec, vector<best_t> &best) {
        size_t dom_count = 0, worse_count = 0;
        size_t cmp_len = cmp_vec->size();
        for (size_t j = 0; j < cmp_len; ++j) {
                bool dominated = point_region_dimention_domination<T>(p, r, (*cmp_vec)[j], best[j]);
                if (dominated)
                        dom_count++;
                bool reverse_dominated = region_point_dimention_domination<T>(r, p, (*cmp_vec)[j], best[j]);
                if (reverse_dominated)
                        worse_count++;
        }
        return (dom_count >= 1) && (worse_count == 0);
}

template <typename T, typename C>

void kd_btree<T, C>::skyline_update(vector<best_t> &best, set<T, C> &skyline_set, point<T> &p) {
        auto it = skyline_set.begin();
        set<T, C> im_set;
        while (it != skyline_set.end()) {
                point<T> q(*it);
                if (dominates<T>(p, q, this->comparators, best)) {
                        // p is out of the current skyline but dominates q which is inside it fix that
                        it = skyline_set.erase(it);
                        im_set.insert(p.location);
                }
                else if (!dominates<T>(q, p, this->comparators, best)) {
                        im_set.insert(p.location);
                        ++it;
                }
                else
                        ++it;
        }
        // insert the dominant ones you found
        for (auto it = im_set.begin(); it != im_set.end(); ++it)
                skyline_set.insert(*it);
}

template <typename T, typename C>

void kd_btree<T, C>::skyline_region_update(vector<best_t> &best, set<region<T>, region_comp<T>> &skyline_regs, region<T> &r) {
        auto it = skyline_regs.begin();
        set<region<T>, region_comp<T>> im_set;
        while (it != skyline_regs.end()) {
                region<T> q = *it;
                if (dominates<T>(r, q, this->comparators, best)) {
                        it = skyline_regs.erase(it);
                        im_set.insert(r);
                }
                else if (!dominates<T>(q, r, this->comparators, best)) {
                        im_set.insert(r);
                        ++it;
                }
                else
                        ++it;
        }
        //insert the dominant ones you found
        for (auto it = im_set.begin(); it != im_set.end(); ++it)
                skyline_regs.insert(*it);
}

template <typename T, typename C>

void kd_btree<T, C>::skyline_region_prune(vector<best_t> &best, set<region<T>, region_comp<T>> &skyline_regs, set<T, C> &skyline_set) {
        auto it = skyline_regs.begin();
        while (it != skyline_regs.end()) {
                region<T> cr = *it;
                bool dom = false;
                for (auto p = skyline_set.begin(); p != skyline_set.end(); ++p) {
                        point<T> cp(*p);
                        if (dominates(cp, cr, this->comparators, best)) {
                                dom = true;
                                it = skyline_regs.erase(it);
                                break;
                        }
                }
                if (!dom)
                        ++it;
        }
}

template <typename T, typename C>

void kd_btree<T, C>::skyline_rec(vector<best_t> &best, set<T, C> &skyline_set, long subtree_root_off) {
        kd_bnode<T> *node = load_node(subtree_root_off);

        if (!node->tag) {
                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                if (skyline_set.empty())
                        skyline_set.insert(pnode->points[0].location);
                for (point<T> &p : pnode->points)
                        skyline_update(best, skyline_set, p);
        }
        else {
                region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                set<region<T>, region_comp<T>> skyline_regs;
                skyline_regs.insert(rnode->regions[0]);
                for (region<T> &r : rnode->regions)
                        skyline_region_update(best, skyline_regs, r);
                //prune the regions dominated by the current skyline points
                skyline_region_prune(best, skyline_regs, skyline_set);
                //recursively descend to the children
                for (auto it = skyline_regs.begin(); it != skyline_regs.end(); ++it) {
                        region<T> f = *it;
                        skyline_rec(best, skyline_set, f.child_offset);
                }
        }
        delete(node);
}

template <typename T, typename C>

set<T, C> kd_btree<T, C>::skyline(vector<best_t> &best) {
        set<T, C> skyline_set;
        skyline_rec(best, skyline_set, this->root_offset);
        return skyline_set;
}


template <typename T>

long compare_all(vector<point<T>> &points, T &element, cmp_vector<T> *comparators) {
        long ret = -1;
        size_t pt_len = points.size();
        size_t cmp_len = comparators->size();
        // use all points
        for (size_t j = 0; j < pt_len; ++j) {
                size_t all_give0 = 0;
                // use each comparator to compare all points with this one to see which is really equal
                for (size_t i = 0; i < cmp_len; ++i) {
                        if ((*comparators)[i](points[j].location, element) == 0)
                                all_give0++;
                }
                // if you found the point mark its place and stop
                if (all_give0 >= cmp_len) {
                        ret = j;
                        break;
                }
        }
        return ret;
}

template <typename T>

bool underfull(kd_bnode<T> *node) {
        if (node->tag) {
                region_kd_bnode<T> *rnode = (region_kd_bnode<T> *) node;
                size_t my_size = rnode->regions.size();
                return (my_size < rnode->minimum_fill);
        }
        else {
                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                size_t my_size = pnode->points.size();
                return (my_size < pnode->minimum_fill);
        }
}

template <typename T, typename C>

void kd_btree<T, C>::eliminate_node(kd_bnode<T> *node, region_kd_bnode<T> *parent, vector<T> &eliminated_data) {
        this->eliminated_stack.push(make_pair(node->my_offset, node->tag));
        if (!node->tag) {
                point_kd_bnode<T> *pnode = (point_kd_bnode<T> *) node;
                for (point<T> &p : pnode->points)
                        eliminated_data.push_back(p.location);
        }
        if (parent) {
                long chld_off = -1;
                size_t reg_len = parent->regions.size();
                for (size_t j = 0; j < reg_len; ++j) {
                        if (parent->regions[j].child_offset == node->my_offset) {
                                chld_off = j;
                                break;
                        }
                }
                auto pregion_it = parent->regions.begin() + chld_off;
                (chld_off >= 0)? parent->regions.erase(pregion_it) : null_func();
                //reflect the changes of the parent to the file
                store_node(parent->my_offset, parent);
        }
}

template <typename T, typename C>

void kd_btree<T, C>::eliminate_root(kd_bnode<T> *root_node) {
        if (root_node->tag) {
                region_kd_bnode<T> *root_regnode = (region_kd_bnode<T> *) root_node;
                size_t root_len = root_regnode->regions.size();
                if (root_len && root_len <= 1) {
                        //if there is only one child this will be the new root
                        this->eliminated_stack.push(make_pair(root_regnode->my_offset, root_regnode->tag));
                        region<T> reg = root_regnode->regions[0];
                        this->root_offset = reg.child_offset;
                }
        }
}

template <typename T, typename C>

void kd_btree<T, C>::erase(T &data) {
        pair<T, T> interval = make_pair(data, data);
        vector<T> exists = range_query(interval);

        if (exists.size() > 0) {
                point_kd_bnode<T> *leaf = choose_leaf(data, this->root_offset);
                if (leaf) {
                        //erase the entry from the leaf
                        long to_erase = compare_all(leaf->points, data, this->comparators);
                        auto erase_it = leaf->points.begin() + to_erase;
                        (to_erase >= 0)? leaf->points.erase(erase_it) : null_func();
                        store_node(leaf->my_offset, leaf);
                        //update the parental region
                        region<T> par_reg = make_parent_region(leaf);
                        region_kd_bnode<T> *pnode = load_node(leaf->parent_offset);
                        if (pnode) {
                                assign_new_region(pnode, par_reg.region_rec, leaf->my_offset);
                                store_node(pnode->my_offset, pnode);
                                // underful logic
                                kd_bnode<T> *current = leaf;
                                region_kd_bnode<T> *parent = pnode;
                                vector<T> eliminated_data;
                                while (current && (current->my_offset != this->root_offset) && underfull<T>(current)) {
                                        //eliminate the current node
                                        eliminate_node(current, parent, eliminated_data);
                                        //go up
                                        delete(current);
                                        current = parent;
                                        parent = load_node(current->parent_offset);
                                }
                                if (current && (current->my_offset == this->root_offset)) {
                                        //if you reached the root check if it needs elimination and do it
                                        eliminate_root(current);
                                }
                                if (current)
                                        delete(current);
                                if (parent)
                                        delete(parent);
                                size_t eliminated = eliminated_data.size();
                                //insert again all the entries you eliminated
                                for (size_t j = 0; j < eliminated; ++j)
                                        insert(eliminated_data[j]);
                        }
                }
        }
}

template <typename T, typename C>

long kd_btree<T, C>::next_pos(bool node_to_store) {
        if (this->eliminated_stack.empty())
                return end_pos(this->file);
        pair<long, bool> top_offset = this->eliminated_stack.top();
        if (!(top_offset.second ^ node_to_store)) {
                this->eliminated_stack.pop();
                return top_offset.first;
        }
        else
                return end_pos(this->file);
}
