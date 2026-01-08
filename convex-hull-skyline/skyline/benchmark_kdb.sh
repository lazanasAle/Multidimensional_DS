#! /bin/sh

make

max_budget=10000000
max_revenue=10000
max_popularity=10
max_vote_avg=10
max_runtime=20
max_votes=800

best_options=("max" "min")

for (( i=50000; i<=900000; i+= 50000 ))
do
        lower_budget=$((RANDOM % max_budget))
        upper_budget=$((lower_budget + RANDOM % (max_budget - lower_budget + 1)))

        lower_revenue=$((RANDOM % max_revenue))
        upper_revenue=$((lower_revenue + RANDOM % (max_revenue - lower_revenue + 1)))

        lower_popularity=$((RANDOM % max_popularity))
        upper_popularity=$((lower_popularity + RANDOM % (max_popularity - lower_popularity + 1)))

        lower_vote_avg=$((RANDOM % max_vote_avg))
        upper_vote_avg=$((lower_vote_avg + RANDOM % (max_vote_avg - lower_vote_avg + 1)))

        lower_runtime=$((RANDOM % max_runtime))
        upper_runtime=$((lower_runtime + RANDOM % (max_runtime - lower_runtime + 1)))

        lower_votes=$((RANDOM % max_votes))
        upper_votes=$((lower_votes + RANDOM % (max_votes - lower_votes + 1)))

        best_budget_num=$((RANDOM % 2))
        best_budget=${best_options[$best_budget_num]}

        best_revenue_num=$((RANDOM % 2))
        best_revenue=${best_options[$best_revenue_num]}

        best_popularity_num=$((RANDOM % 2))
        best_popularity=${best_options[$best_popularity_num]}

        best_vote_avg_num=$((RANDOM % 2))
        best_vote_avg=${best_options[$best_vote_avg_num]}

        best_runtime_num=$((RANDOM % 2))
        best_runtime=${best_options[$best_runtime_num]}

        best_vote_cnt_num=$((RANDOM % 2))
        best_vote_cnt=${best_options[$best_vote_cnt_num]}

        echo running with: $i $lower_budget $upper_budget $lower_revenue $upper_revenue $lower_popularity $upper_popularity \
                $lower_vote_avg $upper_vote_avg $lower_runtime $upper_runtime $lower_votes $upper_votes \
                $best_budget $best_revenue $best_popularity $best_vote_avg $best_runtime $best_vote_cnt

        ./kd_btree_test $i $lower_budget $upper_budget $lower_revenue $upper_revenue $lower_popularity $upper_popularity \
                $lower_vote_avg $upper_vote_avg $lower_runtime $upper_runtime $lower_votes $upper_votes \
                $best_budget $best_revenue $best_popularity $best_vote_avg $best_runtime $best_vote_cnt >> /tmp/tmp_txt.txt
done

make clean
