#include "segment_tree.hpp"


void segment_tree::build_rec(vector<size_t> &arr_build, size_t node, size_t left, size_t right) {
        if (left == right) {
                sg_vec[node].lb = sg_vec[node].rb
                        = sg_vec[node].sum = arr_build[left];
        }
        else {
                size_t imid = (left + right) / 2;
                auto left_it = arr_build.begin() + left;
                auto right_it = arr_build.begin() + right;
                sg_vec[node].lb = *min_element(left_it, right_it + 1);
                sg_vec[node].rb = *max_element(left_it, right_it + 1);
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
