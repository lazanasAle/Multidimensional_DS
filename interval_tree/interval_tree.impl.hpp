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
void interval_node<T>::update_height()
{

}

template<typename T>
void interval_node<T>::update_max_end()
{
    
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
    
}

template<typename T>
void interval_node<T>::right_left_rotation()
{
    
}



// will add comments to make it clear what each piece of code represents.
//template<typename T>