#include "segment_tree.hpp"


void segment_tree::build_rec(vector<size_t> &arr_build, size_t node, size_t left, size_t right) {
        if (left == right) {
                sg_vec[node].lb = sg_vec[node].rb = left;
                sg_vec[node].sum = arr_build[left];
        }
        else {
                size_t imid = (left + right) / 2;
                sg_vec[node].lb = left;
                sg_vec[node].rb = right;
                size_t lchild = 2 * node + 1;
                size_t rchild = 2 * node + 2;
                build_rec(arr_build, lchild, left, imid);
                build_rec(arr_build, rchild, imid + 1, right);

                sg_vec[node].sum = sg_vec[lchild].sum + sg_vec[rchild].sum;
        }
}


void segment_tree::build(vector<size_t> &arr_build) {
        size_t vec_len = arr_build.size();

        sg_vec.resize(4 * vec_len - 1);

        build_rec(arr_build, 0, 0, vec_len - 1);
}

segment_tree::segment_tree(vector<size_t> &arr_build) {
        build(arr_build);
}

string segment_tree::stringnify_tree() {
        string str;
        for (segment &sg : sg_vec)
                str += "[" + to_string(sg.lb) + ", " + to_string(sg.rb) + "]:" + to_string(sg.sum) + ", ";
        return str;
}

void segment_tree::add_rec(size_t place, size_t val, size_t idx) {
        if ((sg_vec[idx].lb == sg_vec[idx].rb) && (place == sg_vec[idx].lb))
                sg_vec[idx].sum += val;
        else {
                sg_vec[idx].sum += val;
                const size_t lchild = 2 * idx + 1;
                const size_t rchild = 2 * idx + 2;
                if ((sg_vec[lchild].lb <= place) && (sg_vec[lchild].rb >= place))
                        add_rec(place, val, lchild);
                else
                        add_rec(place, val, rchild);
        }
}


void segment_tree::add(size_t place, size_t val) {
        add_rec(place, val, 0);
}


size_t segment_tree::sum_rec(size_t left, size_t right, size_t idx) {
        if ((left == sg_vec[idx].lb) && (right == sg_vec[idx].rb))
                return sg_vec[idx].sum;
        else {
                const size_t lchild = 2 * idx + 1;
                const size_t rchild = 2 * idx + 2;
                bool all_on_left = (left >= sg_vec[lchild].lb) && (right <= sg_vec[lchild].rb);
                bool all_on_right = (left >= sg_vec[rchild].lb) && (right <= sg_vec[rchild].rb);
                if (all_on_left) //all interval is in left-child
                        return sum_rec(left, right, lchild);
                else if (all_on_right) //all interval is in right child
                        return sum_rec(left, right, rchild);
                else { //it falls both on right and on left
                        //this means only one thing left bound is in left child right bound is in right child
                        // so i have to do the recursion left to the rb of the left child to the left child and
                        // right to the rb of the right child
                        size_t this_sum = sum_rec(left, sg_vec[lchild].rb, lchild) +
                                sum_rec(sg_vec[rchild].lb, right, rchild);
                        return this_sum;
                }
        }
}

size_t segment_tree::sum(size_t left, size_t right) {
        if (left > right) //error code lower bound exceeds upper bound
                return SIZE_MAX;
        else
                return sum_rec(left, right, 0);
}
