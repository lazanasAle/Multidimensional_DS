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

template <typename T>
interval_node<T>::~interval_node()
{
    if(this->left)
        delete(this->left);
    if(this->right)
        delete(this->right);
    delete(inter);
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
interval<T> *interval_node_iterator<T>::context() 
{
	return this->node->inter;
}

template <typename T>
interval_node_iterator<T> interval_node_iterator<T>::next()
{
    if(this->is_null())
    {
        return interval_node_iterator<T>(nullptr);

    }
    if(this->node->right)
    {
        interval_node<T> *next_node=this->node->right;

        while(next_node->left)
        {
            next_node=next_node->left;
        }
        return interval_node_iterator<T>(next_node);
    }

    interval_node<T> *rn =this->node;
    while((rn->parent) && (rn->parent->right==rn))
    {
        rn=rn->parent;
    }
    return interval_node_iterator<T>(rn->parent);

}

//same logic as next
template <typename T>
interval_node_iterator<T> interval_node_iterator<T>::previous()
{
    if(this->is_null())
    {
        return interval_node_iterator<T>(nullptr);

    }
    if(this->node->left)
    {
        interval_node<T> *previous_node=this->node->left;

        while(previous_node->right)
            previous_node=previous_node->right;
        return interval_node_iterator<T>(previous_node);
    }

    interval_node<T> *prn =this->node;
    while((prn->parent) && (prn->parent->left==prn))
        prn=prn->parent;
    return interval_node_iterator<T>(prn->parent);  
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

template <typename T>
interval_node<T> *&interval_tree<T>::search(interval<T> &inter, interval_node<T> *&subtree_root)
{
    if(subtree_root== nullptr)
    {
        return subtree_root;

    }
    else if(comparator(inter.low, subtree_root->inter->low)==0 &&
            comparator(inter.high, subtree_root->inter->high)==0)
    {
        return subtree_root;
    }
    else if(comparator(inter.low, subtree_root->inter->low)<0)
    {
        return search(inter,subtree_root->left);

    }
    else
        return search(inter,subtree_root->right);
}

template <typename T>
void interval_tree<T>::update_max_end_up(interval_node<T> *node)
{
    while (node!= nullptr)
    {
        node->update_max_end();
        node=node->parent;
    }
}

template <typename T>
void interval_tree<T>::avl_balance(interval_node<T> *&node)
{
    if(node ==nullptr)
        return;
    else if (node->balance_factor()>1)
    {
        if(node->left && node->left->balance_factor()<0)
        {
            node->left_right_rotation();
        }
        else
        {
            node->right_rotation;
        }
    }
    else if(node->balance_factor()<-1)
    {
        if(node->right && node->right->balance_factor() > 0)
        {
            node->right_left_rotation();

        }
        else
            node->left_rotation();
    }

}

template <typename T>
void interval_tree<T>::add_node(interval_node<T> *&new_node, interval_node<T> *&subtree_root)
{
    if(subtree_root==nullptr)
    {
        subtree_root=new_node;
    }
    else if(comparator(new_node->inter->low, subtree_root->inter->low)<0)
    {
        new_node->parent=subtree_root;
        add_node(new_node,subtree_root->left);
    }
    else 
    {
        new_node->parent= subtree_root;
        add_node(new_node, subtree_root->right);
    }

    avl_balance(subtree_root);
    subtree_root->update_height();
    subtree_root->update_max_end();
}


template <typename T>
interval_node<T> *interval_tree<T>::max_node(interval_node<T> *&subtree_root)
{
    interval_node<T> *curr=subtree_root;
    while(curr->right!= nullptr)
    {
        curr= curr ->right;
    }
    return curr;
}

//same logic as max_node
template <typename T>
interval_node<T> *interval_tree<T>::min_node(interval_node<T> *&subtree_root)
{
    interval_node<T> *curr=subtree_root;
    while(curr->left != nullptr)
    {
        curr= curr ->left;
    }
    return curr;
}

template <typename T>
void interval_tree<T>::remove_node(interval<T> &inter, interval_node<T> *&sub_root)
{

    //...
}

template <typename T>
void interval_tree<T>::insert(interval<T> &inter)
{
    interval<T> *new_inter= new interval<T>(inter.low, inter.high);
    interval_node<T> *new_node= new interval_node<T>(new_inter);
    add_node(new_node, root);
}

template <typename T>
void interval_tree<T>::erase(interval<T> &inter)
{
    remove_node(inter, root);
}

template <typename T>
interval_node_iterator<T> interval_tree<T>::find(interval<T> &inter)
{
    return interval_node_iterator<T>(search(inter,root));
}

template<typename T>
void interval_tree<T>::interval_search_rec(interval<T> &inter,vector<interval<T>> &result, interval_node<T> *subtree_root)
{

}

template <typename T>
vector<interval<T>> interval_tree<T>::interval_search(interval<T> &inter)
{
    vector<interval<T>> result;
    interval_search_rec(inter, result, root);
    return result;
}

//..


template <typename T>
vector<interval<T>> interval_tree<T>::inorder()
{
    vector<interval<T>> to_return;
    inorder(root, to_return);
    return to_return;
}

template <typename T>
interval_tree<T>::~interval_tree()
{
    if(root)
        delete(root);
}

//comments

// will add comments to make it clear what each piece of code represents.
//template<typename T>