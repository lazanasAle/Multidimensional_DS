//R-tree implementation
// We are using Guttman's quadreatic split algorithm

template <typename T, typename C>
rtree<T,C>::rtree(cmp_vector<T> *cmp_vec, 
        function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
        function<rectangle<T> (vector<T *> &)> point_rectangle_fn)
        : kd_btree<T,C>(cmp_vec, region_rectangle_fn, point_rectangle_fn)
        {}

template <typename T, typename C>
double rtree<T,C>::area_increase(vector<point<T>> &group, point<T> &entry)
{
    if(group.empty()) return 0.0;
    vector<T*> old_points;
    for(auto &p: group)
    {
        old_points.push_back(&p.location);
    }
    rectangle<T> old_mbr=this->make_point_rectangle(old_points);
    double old_area= rect_area(old_mbr, this->comparators);

    vector<T*> new_points=old_points;
    new_points.push_back(&entry.location);
    rectangle<T> new_mbr=this->make_point_rectangle(new_points);
    double new_area= rect_area(new_mbr, this->comparators);

    return new_area-old_area;
}


template <typename T, typename C>
double rtree<T,C>::area_increase(vector<region<T>> &group, region<T> &entry)
{
    if(group.empty()) return 0.0;
    vector<rectangles<T>*> old_rectangles;
    for(auto &r: group)
    {
        old_rectangles.push_back(&r.region_rec);
    }

    rectangle<T> old_mbr=this->make_point_rectangle(old_points);
    double old_area= rect_area(old_mbr, this->comparators);
    
    vector<rectangle<T>*> new_rectangles=old_rectangles;
    new_rectangles.push_back(&entry.region_rec);
    rectangle<T> new_mbr=this->make_region_rectangle(new_rectangles);
    double new_area= rect_area(new_mbr, this->comparators);
    return new_area-old_area;
}

//main split algorithm. Guttman's quadratic split
template <typename T, typename C>
kd_bnode<T> *rtree<T,C>::split_node(kd_bnode<T> *node)
{

}




//will add commenting soon