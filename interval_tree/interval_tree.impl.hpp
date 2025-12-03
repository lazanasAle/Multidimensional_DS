//Interval methods
template <typename T>
interval<T>::interval(T l, T h)
{
    this->low=l;
    this->high=h;
}

template <typename T>
bool interval<T>::overlaps(interval<T> &other)
{
    return (this->low <= other.high && this->high >=other.low);

}

template <typename T>
bool interval<T>::contains(T point)
{
    return (point >= this->low && point <= this->high);

}

//Methods for interval nodes

template<typename T>
interval_node<T>::interval_node(interval<T> *i)
{
    this->max_end=i->high;
    this->left= this->right =  this->parent =nullptr;
    this->height=0;
    this->inter=i;
}


template<typename T>
int32_t interval_node<T>:: balance_factor()
{
    int32_t left_height=(this->left)? this->left->height:-1
    int32_t right_height=(this->right)? this->right->height:-1
    return (left_height-right_height);


}

template<typename T>
void interval_node<T>::update_height()
{
    int32_t left_height=(this->left)? this->left->height:-1;
    //if this->left: this->left->hegiht. otherwise return -1
    
    int32_t right_height=(this->right)? this->right->height: -1;
    this->height =max(left_height,right_height)+1;
    // +++
}

template<typename T>
void interval_node<T>::update_max_end()
{
    T left_max=(this->left)? this->left->max_end: this->inter->high;   
    T right_max=(this->right)? this->right->max_end: this->inter->high;
    this->max_end= max(this->inter->high, max(left_max,right_max));   

}

template<typename T>
void interval_node<T>::left_rotation()
{
    
}

template<typename T>
void interval_node<T>::right_rotation()
{
    
}

template<typename T>
void interval_node<T>::left_right_rotation()
{
    if(this->left)
    {
        this->left->left_rotation();
        this->right_rotation();
    }
}

template<typename T>
void interval_node<T>::right_left_rotation()
{
    if(this->right)
    {
        this->right->right_rotation();
        this->left_rotation();
    }
}


//Interval node iterator methods
//...


// will add comments to make it clear what each piece of code represents.
//template<typename T>