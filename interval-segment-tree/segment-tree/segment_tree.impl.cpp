#include "segment_tree.hpp"


segment get_from_array(vector<size_t> &arr, size_t left, size_t right) {
        segment sg;
        auto left_it = arr.begin() + left;
        auto right_it = arr.begin() + right + 1;
        sg.lb = *min_element(left_it, right_it);
        sg.rb = *max_element(left_it, right_it);
        sg.sum = accumulate(left_it, right_it, 0);
        return sg;
}

void segment_tree::build_rec(vector<size_t> &arr_build, size_t left, size_t right) {
        if (left < right) {
                size_t imid = (left + right) / 2;
                if (sg_vec.empty()) {
                        segment me = get_from_array(arr_build, left, right);
                        sg_vec.push_back(me);
                }
                segment lseg = get_from_array(arr_build, left, imid);
                segment rseg = get_from_array(arr_build, imid + 1, right);
                sg_vec.push_back(lseg);
                sg_vec.push_back(rseg);
                build_rec(arr_build, left, imid);
                build_rec(arr_build, imid + 1, right);
        }
}


void segment_tree::build(vector<size_t> &arr_build) {
        size_t vec_len = arr_build.size();
        sort(arr_build.begin(), arr_build.end());
        build_rec(arr_build, 0, vec_len - 1);
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
