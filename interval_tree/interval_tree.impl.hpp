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
    int32_t left_height=(this->left)? this->left->height:-1;
    int32_t right_height=(this->right)? this->right->height:-1;
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
    //safety check first (can't rotate left if there is no child to pivot in)
    if(this->right ==nullptr)
        return;
    interval_node<T> *varA=this->left; //varA=the current left child
    interval_node<T> *varB=this->right->left; //varB=the left child of the right node
    interval_node<T> *varC=this->right->right; //varC=the right vhild of the right node
    
    //saving the current node's interval data
    interval<T> *tmp=this->inter; 

    this->inter=this->right->inter; //overwrite the current node's data with the right child's data
    //moves ups the right child without changing 'this' mem. adress
    
    this->left= new interval_node<T>(tmp); //node creation. it will act as the new left child
    //it will contain the old root data

    this->left->parent=this; //set the parent of this new node to be the current one
    this->left->left=varA; //link again the subtrees

    if(varA) //updates varA's parent pointer to the new left node if varA exists
    {
        varA->parent=this->left;

    }
    //connect the middle subtree (varB) to be the right child of the new left node
    this->left->right=varB; 

    if(varB) //update varB' parent pointer if varB exists
        varB->parent=this->left;

    this->right=varC; //link again the subtrees
    //we will attach the original right-right subtree -varC- to the current node's right

    if(varC) //update varC's parent pointer to the current node (again, if varC exists)
        varC->parent=this;

    //now updating height+max_end for the new left child first
    //it is lower in the tree
    this->left->update_height();
    this->left->update_max_end();

    //update height+max_end for the current node (the new root)
    this->update_height();
    this->update_max_end();   
}

//exact same logic with left rotation just above
template <typename T>
void interval_node<T>::right_rotation() 
{
	if (this->left == nullptr)
		return;
	interval_node<T> *varA = this->left->left;
	interval_node<T> *varB = this->left->right;
	interval_node<T> *varC = this->right;

	interval<T> *tmp = this->inter;
	this->inter = this->left->inter;
	this->right = new interval_node<T>(tmp);
	this->right->parent = this;
	this->left = varA;

	if (varA)
			varA->parent = this;
	this->right->left = varB;
	
	if (varB)
		varB->parent = this->right;
	this->right->right = varC;

	if (varC)
		varC->parent = this->right;

	this->right->update_height();
	this->right->update_max_end();
	
	this->update_height();
	this->update_max_end();
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
template <typename T>
interval_node_iterator<T>::interval_node_iterator(interval_node<T> *n)
{
    this->node=n;
}

template <typename T>
bool interval_node_iterator<T>::is_null()
{
    return (this->node==nullptr);
}

template <typename T>
interval_node_iterator<T> interval_node_iterator<T>::next()
{

}

template <typename T>
interval_node_iterator<T> interval_node_iterator<T>::previous()
{
    
}


//Interval tree methods

template <typename T>
interval_tree<T>::interval_tree(function<int (T &,T &)> cmp)
{
    this->root = nullptr;
    this->comparator=cmp;
}

template <typename T>
bool interval_tree<T>::empty()
{
    return (this->root ==nullptr);
}


// will add comments to make it clear what each piece of code represents.
//template<typename T>