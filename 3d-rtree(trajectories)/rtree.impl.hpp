//R-tree implementation
// We are using Guttman's quadreatic split algorithm

template <typename T, typename C>
rtree<T,C>::rtree(cmp_vector<T> *cmp_vec, 
        function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
        function<rectangle<T> (vector<T *> &)> point_rectangle_fn)
        : kd_btree<T,C>(cmp_vec, region_rectangle_fn, point_rectangle_fn)
        {}




//main split algorithm. Guttman's quadratic split
template <typename T, typename C>
kd_bnode<T> *rtree<T,C>::split_node(kd_bnode<T> *node)
{

}




//will add commenting soon