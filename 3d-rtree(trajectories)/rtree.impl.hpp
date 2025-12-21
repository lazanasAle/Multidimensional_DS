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
    vector<rectangle<T>*> old_rectangles;
    for(auto &r: group)
    {
        old_rectangles.push_back(&r.region_rec);
    }

    rectangle<T> old_mbr=this->make_region_rectangle(old_rectangles);
    //    rectangle<T> old_mbr=this->make_point_rectangle(old_points);
    double old_area= rect_area(old_mbr, this->comparators);
    
    vector<rectangle<T>*> new_rectangles=old_rectangles;
    new_rectangles.push_back(&entry.region_rec);
    rectangle<T> new_mbr=this->make_region_rectangle(new_rectangles);
    double new_area= rect_area(new_mbr, this->comparators);
    return new_area-old_area;
}

template<typename T, typename C>
pair<size_t,size_t> rtree<T,C>::pick_seeds_points(vector<point<T>> &entries)
{
        double max_waste=-1.0;
        pair<size_t, size_t> seeds={0,1};
        size_t n= entries.size();

        for(size_t i=0; i<n; i++)
        {
                for(size_t j=i+1; j<n; j++)
                {
                        //create MBR that has both entries
                        vector<T*> combined={&entries[i].location, &entries[j].location};

                        rectangle<T> mbr=this->make_point_rectangle(combined);
                        double area_mbr=rect_area(mbr, this->comparators);
                        
                        //for points area=0 obviously. So that waste=area(MBR)
                        //we need the most 'distant' points
                        double waste=area_mbr; 

                        if(waste>max_waste)
                        {
                                max_waste=waste;
                                seeds={i,j};
                        }
                }
        }
        return seeds;
}

template<typename T, typename C>
pair<size_t,size_t> rtree<T,C>::pick_seeds_regions(vector<region<T>> &entries)
{
        double max_waste=-1.0;
        pair<size_t, size_t> seeds={0,1};
        size_t n= entries.size();

        for(size_t i=0; i<n; i++)
        {
                for(size_t j=i+1; j<n; j++)
                {
                        //create MBR that has both entries
                        vector<rectangle<T*>> combined={&entries[i].region_rec, &entries[j].region_rec};

                        rectangle<T> mbr=this->make_region_rectangle(combined);
                        double area_i=rect_area(entries[i].region_rec, this->comparators);
                        double area_j=rect_area(entries[j].region_rec, this->comparators);
                        double area_mbr=rect_area(mbr, this->comparators);
                        
                        //waste is the area of MBR minus the area of entry_i minus the area of entry_j
                        double waste=area_mbr- area_i-area_j; 

                        if(waste>max_waste)
                        {
                                max_waste=waste;
                                seeds={i,j};
                        }
                }
        }

        return seeds;   
}

//Picking next
template <typename T, typename C>
size_t rtree<T, C>::pick_next_point(vector<point<T>> &remaining,
                        vector<point<T>> &group1,
                        vector<point<T>> &group2)
{
        double max_diff=-1.0;
        size_t next_idx;

        for(size_t i=0; i<remaining.size(); i++)
        {
                double d1=area_increase(group1, remaining[i]);
                double d2=area_increase(group2, remaining[i]);

                double diff=abs(d1-d2);

                if(diff>max_diff)
                {
                        max_diff=diff;
                        next_idx=i;
                }
        }
        return next_idx;
}

template <typename T, typename C>
size_t rtree<T,C>::pick_next_region(vector<region<T>> &remaining,
                        vector<region<T>> &group1,
                        vector<region<T>> &group2)
{
        double max_diff=-1.0;
        size_t next_idx;

        for(size_t i=0; i<remaining.size(); i++)
        {
                double d1=area_increase(group1, remaining[i]);
                double d2=area_increase(group2, remaining[i]);

                double diff=abs(d1-d2);

                if(diff>max_diff)
                {
                        max_diff=diff;
                        next_idx=i;
                }
        }
        return next_idx;
}

//assigning to gorups

template <typename T, typename C>
bool rtree<T,C>::assign_to_group1(vector<point<T>> &group1,
                                vector<point<T>> &group2,
                                point<T> &entry)
{
        double d1=area_increase(group1,entry);
        double d2=area_increase(group2,entry);

        if(abs(d1-d2)< 1e-9)
        {
                vector<T*> g1_points,g2_points;
                for(auto &p: group1) g1_points.push_back(&p.location);
                for(auto &p: group2) g2_points.push_back(&p.location);

                if(g1_points.empty()) return true;
                if(g2_points.empty()) return false;

                double area1=rect_area(this->make_point_rectangle(g1_points), this->comparators);
                double area2=rect_area(this->make_point_rectangle(g2_points), this->comparators);

                if(abs(area1-area2)< 1e-9)
                {
                        return group1.size()<= group2.size();

                }

                return area1<area2;
        }

        return d1<d2;
}



template <typename T, typename C>
bool rtree<T,C>::assign_to_group1(vector<region<T>> &group1,
                                vector<region<T>> &group2,
                                region<T> &entry)
{
        double d1=area_increase(group1,entry);
        double d2=area_increase(group2,entry);

        if(abs(d1-d2)< 1e-9)
        {
                vector<rectangle<T>*> g1_rectangles,g2_rectangles;
                for(auto &r: group1) g1_rectangles.push_back(&r.region_rec);
                for(auto &r: group2) g2_rectangles.push_back(&r.region_rec);

                if(g1_rectangles.empty()) return true;
                if(g2_rectangles.empty()) return false;

                double area1=rect_area(this->make_region_rectangle(g1_rectangles), this->comparators);
                double area2=rect_area(this->make_region_rectangle(g2_rectangles), this->comparators);

                if(abs(area1-area2)< 1e-9)
                {
                        return group1.size()<= group2.size();

                }

                return area1<area2;
        }

        return d1<d2;
}


//Main split algorithm. Guttman's quadratic split
template <typename T, typename C>
kd_bnode<T> *rtree<T,C>::split_node(kd_bnode<T> *node)
{

}



//will add commenting soon