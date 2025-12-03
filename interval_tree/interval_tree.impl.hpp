
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

//will implement these here soon
       // void update_height(); //update the height of the tree based on the children' heights
       // void update_max_end(); //update max_end based on children and its own interal
       // void left_rotation();
       // void right_rotation();
       // void left_right_rotation();
       /// void right_left_rotation();