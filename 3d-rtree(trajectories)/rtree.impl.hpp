//R-tree implementation
//We are using Guttman's quadreatic split algorithm
//Implementing spatial-aware node splitting that minimizes MBR overlap

template <typename T, typename C>
rtree<T,C>::rtree(cmp_vector<T> *cmp_vec, 
        function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
        function<rectangle<T> (vector<T *> &)> point_rectangle_fn)
        : kd_btree<T,C>(cmp_vec, region_rectangle_fn, point_rectangle_fn)
        {}

//calculating area increase for adding a point to a group
//return new_area-old_area
template <typename T, typename C>
double rtree<T,C>::area_increase(vector<point<T>> &group, point<T> &entry)
{
    if(group.empty()) return 0.0;

    //get current MBR of group
    vector<T*> old_points;
    for(auto &p: group)
    {
        old_points.push_back(&p.location);
    }

    rectangle<T> old_mbr = this->make_point_rectangle(old_points); 
    double old_area = rect_area(old_mbr, this->comparators);

    //Get MBR with new entry added
    vector<T*> new_points=old_points;
    new_points.push_back(&entry.location);

    rectangle<T> new_mbr = this->make_point_rectangle(new_points);
    double new_area = rect_area(new_mbr, this->comparators);

    return new_area-old_area;
}

//calculate area increase for adding a region to a group
//returns new_area-old_area
template <typename T, typename C>
double rtree<T,C>::area_increase(vector<region<T>> &group, region<T> &entry)
{
    if(group.empty()) return 0.0;

    //get current MBR of group
    vector<rectangle<T>*> old_rectangles;
    for(auto &r: group)
    {
        old_rectangles.push_back(&r.region_rec);
    }

    rectangle<T> old_mbr=this->make_region_rectangle(old_rectangles);
    double old_area= rect_area(old_mbr, this->comparators);
    
    //Get MBR with new entry added
    vector<rectangle<T>*> new_rectangles=old_rectangles;
    new_rectangles.push_back(&entry.region_rec);

    rectangle<T> new_mbr = this->make_region_rectangle(new_rectangles);
    double new_area = rect_area(new_mbr, this->comparators);

    return new_area-old_area;
}

//pick two seed points that waste the most space when grouped.
// Find pair (i,j) with max MBR area
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
        return seeds; //returning the pair of indices representing the 2 seeds
}

//pick 2 seed regions that waste the most space when grouped
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
                        vector<rectangle<T>*> combined={&entries[i].region_rec, &entries[j].region_rec};
                          
                        rectangle<T> mbr = this->make_region_rectangle(combined);
                        double area_i = rect_area(entries[i].region_rec, this->comparators);
                        double area_j = rect_area(entries[j].region_rec, this->comparators);
                        double area_mbr = rect_area(mbr, this->comparators);

                        //waste is the area of MBR minus the area of entry_i minus the area of entry_j
                        //waste=area(MBR(i,j))-area(i)-area(j)
                        double waste=area_mbr- area_i-area_j; 

                        if(waste>max_waste)
                        {
                                max_waste=waste;
                                seeds={i,j};
                        }
                }
        }

        return seeds; //returning pair of indices representing the 2 seeds  
}

//Picking next point to distribute during split
//chooses entry with max preference difference between groups
//returns index in 'remaining' vector

template <typename T, typename C>
size_t rtree<T, C>::pick_next_point(vector<point<T>> &remaining,
                        vector<point<T>> &group1,
                        vector<point<T>> &group2)
{
        double max_diff=-1.0;
        size_t next_idx=0;

        //for each remaining entry,find which group prefers it more
        for(size_t i=0; i<remaining.size(); i++)
        {
                double d1=area_increase(group1, remaining[i]);
                double d2=area_increase(group2, remaining[i]);

                double diff=abs(d1-d2);

                //pick entry with strongest preference
                if(diff>max_diff)
                {
                        max_diff=diff;
                        next_idx=i;
                }
        }
        return next_idx;
}

//picking next region to distribute during split. Chooses entry with max
//preference difference between groups
template <typename T, typename C>
size_t rtree<T,C>::pick_next_region(vector<region<T>> &remaining,
                        vector<region<T>> &group1,
                        vector<region<T>> &group2)
{
        double max_diff=-1.0;
        size_t next_idx=0;

        //for each remaining entry, find which groups prefers it more
        for(size_t i=0; i<remaining.size(); i++)
        {
                double d1=area_increase(group1, remaining[i]);
                double d2=area_increase(group2, remaining[i]);

                double diff=abs(d1-d2);

                //pick entry with strongest preference
                if(diff>max_diff)
                {
                        max_diff=diff;
                        next_idx=i;
                }
        }
        return next_idx; //returning index in remaining vector
}

//Assigning to groups

//decide which group to assign a point entry
// min area increase -> min area -> min size
//returns true for group1 and false for group2 

template <typename T, typename C>
bool rtree<T,C>::assign_to_group1(vector<point<T>> &group1,
                                vector<point<T>> &group2,
                                point<T> &entry)
{
        double d1=area_increase(group1,entry);
        double d2=area_increase(group2,entry);

        //compare area increases
        if(abs(d1-d2)< 1e-9)
        {
                vector<T*> g1_points,g2_points;
                for(auto &p: group1) g1_points.push_back(&p.location);
                for(auto &p: group2) g2_points.push_back(&p.location);

                if(g1_points.empty()) return true;
                if(g2_points.empty()) return false;

                //compare resulting areas
                rectangle<T> temp_rect1 = this->make_point_rectangle(g1_points);
                double area1 = rect_area(temp_rect1, this->comparators);
                
                rectangle<T> temp_rect2 = this->make_point_rectangle(g2_points);
                double area2 = rect_area(temp_rect2, this->comparators);

                //balancing group sizes
                if(abs(area1-area2)< 1e-9)
                {
                        return group1.size()<= group2.size();

                }

                return area1<area2;
        }

        return d1<d2; 
}

//Decide which group to assign a region entry
// min area increase -> min area -> min size
//returns true for group1 and false for group2 

template <typename T, typename C>
bool rtree<T,C>::assign_to_group1(vector<region<T>> &group1,
                                vector<region<T>> &group2,
                                region<T> &entry)
{
        double d1=area_increase(group1,entry);
        double d2=area_increase(group2,entry);

        //compare area increases
        if(abs(d1-d2)< 1e-9)
        {
                vector<rectangle<T>*> g1_rectangles,g2_rectangles;
                for(auto &r: group1) g1_rectangles.push_back(&r.region_rec);
                for(auto &r: group2) g2_rectangles.push_back(&r.region_rec);

                if(g1_rectangles.empty()) return true;
                if(g2_rectangles.empty()) return false;

                //compare resulting areas
                rectangle<T> temp_r1 = this->make_region_rectangle(g1_rectangles);
                double area1 = rect_area(temp_r1, this->comparators);

                rectangle<T> temp_r2 = this->make_region_rectangle(g2_rectangles);
                double area2 = rect_area(temp_r2, this->comparators);

                //balance group sizes
                if(abs(area1-area2)< 1e-9)
                {
                        return group1.size()<= group2.size();
                }

                return area1<area2;
        }

        return d1<d2;
}

//Main split algorithm. Guttman's quadratic split
//splits and overflowing node into 2 nodes with min MBR overlap
// The algorithm says pick 2 seed entries (most wasteful pair), then create 2 groups starting with seeds
//Next, distribute remaining entries (pick next entry, assign to group with less area increase)

template <typename T, typename C>
kd_bnode<T> *rtree<T,C>::split_node(kd_bnode<T> *node)
{
        //case 1=leaf node (constraint points)
        if(!node->tag)
        {
                //point node -leaf
                point_kd_bnode<T> *pnode=static_cast<point_kd_bnode<T> *>(node);
                vector<point<T>> all_points= pnode->points;

                size_t m=pnode->minimum_fill; //min entries per node

                //picking seeds
                auto seeds=pick_seeds_points(all_points);

                vector<point<T>> group1= {all_points[seeds.first]};
                vector<point<T>> group2= {all_points[seeds.second]};
                vector<point<T>> remaining;

                //move all entries besides seeds to remaining
                for(size_t i=0; i<all_points.size(); i++)
                {
                        if(i!= seeds.first && i!=seeds.second)
                        {
                                remaining.push_back(all_points[i]);
                        }
                }

                //distribute entries
                while(!remaining.empty())
                {
                        if(group1.size()+remaining.size()==m)
                        {
                                //checking if one group needs all the other entries
                                group1.insert(group1.end(),remaining.begin(), remaining.end());
                                break;
                        }

                        if(group2.size()+remaining.size()==m)
                        {
                                //checking if one group needs all the other entries
                                group2.insert(group2.end(),remaining.begin(), remaining.end());
                                break;
                        }

                        size_t next= pick_next_point(remaining,group1,group2);

                        if(assign_to_group1(group1, group2, remaining[next]))
                        {
                                group1.push_back(remaining[next]);
                        }
                        else
                        {
                                group2.push_back(remaining[next]);
                        }
                        remaining.erase(remaining.begin()+next);
                }

                //create 2 new nodes
                pnode->points=move(group1);
                point_kd_bnode<T> *new_node=new point_kd_bnode<T>(this->comparators, this->make_point_rectangle);
                new_node->points=move(group2);
                new_node->level=node->level;

                return new_node; //return newly created sibling node
        }
        //case 2= internal node (contains regions)
        else
        {
                region_kd_bnode<T> *rnode=static_cast<region_kd_bnode<T> *>(node);
                vector<region<T>> all_regions=rnode->regions;

                size_t m=rnode->minimum_fill;

                //pick seeds
                auto seeds=pick_seeds_regions(all_regions);

                vector<region<T>> group1= {all_regions[seeds.first]};
                vector<region<T>> group2= {all_regions[seeds.second]};
                vector<region<T>> remaining;

                //collect non-seed entries
                for(size_t i=0; i<all_regions.size(); i++)
                {
                        if(i!= seeds.first && i!=seeds.second)
                        {
                                remaining.push_back(all_regions[i]);
                        }
                }

                //distribute entries
                while(!remaining.empty())
                {
                        //check min fill constraint for group1+2

                        if(group1.size()+remaining.size()==m)
                        {
                                //checking if one group needs all the other entries
                                group1.insert(group1.end(),remaining.begin(), remaining.end());
                                break;
                        }

                        if(group2.size()+remaining.size()==m)
                        {
                                //checking if one group needs all the other entries
                                group2.insert(group2.end(),remaining.begin(), remaining.end());
                                break;
                        }

                        //pick next entry and assign to best group
                        size_t next= pick_next_region(remaining,group1,group2);

                        if(assign_to_group1(group1, group2, remaining[next]))
                        {
                                group1.push_back(remaining[next]);
                        }
                        else
                        {
                                group2.push_back(remaining[next]);
                        }
                        remaining.erase(remaining.begin()+next);
                }

                //create 2 nodes from final groups
                rnode->regions=move(group1);
                region_kd_bnode<T> *new_node=new region_kd_bnode<T>(this->comparators, this->make_region_rectangle);
                new_node->regions=move(group2);
                new_node->level=node->level;

                return new_node;
        }       
}


//will add commenting soon