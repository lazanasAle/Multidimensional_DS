//R-tree implementation
// We are using Guttman's quadreatic spliy algorithm

template <typename T, typename C>
rtree<T,C>::rtree(cmp_vector<T> *cmp_vec, 
        function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
        function<rectangle<T> (vector<T *> &)> point_rectangle_fn)
        : kd_btree<T,C>(cmp_vec, region_rectangle_fn, point_rectangle_fn)
        {}



