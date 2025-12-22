#include "segment_tree.hpp"


size_t segment_tree::build_rec(vector<size_t> &arr_build, size_t left, size_t right) {
        segment sg;
        size_t imid = (left + right) / 2;
        auto left_it = arr_build.begin() + left;
        auto right_it = arr_build.begin() + right + 1;
        if (left < right) {
                sg.lb = *min_element(left_it, right_it);
                sg.rb = *max_element(left_it, right_it);
                sg.sum = build_rec(arr_build, left, imid) + build_rec(arr_build, imid + 1, right);
                sg_vec.push_back(sg);
                return sg.sum;
        }
        else {
                sg.lb = sg.rb = sg.sum = arr_build[left];
                sg_vec.push_back(sg);
                return sg.sum;
        }
}


void segment_tree::build(vector<size_t> &arr_build) {
        size_t vec_len = arr_build.size();
        build_rec(arr_build, 0, vec_len - 1);
        reverse(sg_vec.begin(), sg_vec.end());
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
