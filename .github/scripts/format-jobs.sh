#!/bin/bash

if [[ -n "$1" ]]; then
    jobs="${1}"
fi

jobs_count=$(echo "$jobs" | jq '.total_count')
jobs_skipped=$(echo "$jobs" | jq '[ .jobs[] | select(.conclusion == "skipped")] | length')

echo "*$jobs_count* jobs total, *$jobs_skipped* jobs skipped"

tree_vertical="│"
tree_horizontal="─"
tree_t="├"
tree_angle="└"

success_badge="✅ Success"
neutral_badge="🟡 Neutral"
skipped_badge="⭕ Skipped"
cancelled_badge="🚫 Cancelled"
timed_out_badge="🕙 Timed out"
action_required_badge="▶️ Action required"
failed_badge="❌ Failed"



for((i=0;i<jobs_count;i++)); do
    job=$(echo "$jobs" | jq ".jobs[$i]")
    job_name=$(echo "$job" | jq ".name")
    job_result=$(echo "$job" | jq ".conclusion")
    echo -e "\n"
    # neutral, success, skipped, cancelled, timed_out, action_required, failure
    if [[ ${job_result} == *"success"* ]]; then
        job_status=$success_badge
        echo "$job_status *$job_name*"
        continue
    elif [[ ${job_result} == *"neutral"* ]]; then
        job_status=$neutral_badge
        echo "$job_status *$job_name*"
    elif [[ ${job_result} == *"skipped"* ]]; then
        continue
        job_status=$skipped_badge
    elif [[ ${job_result} == *"cancelled"* ]]; then
        job_status=$cancelled_badge
        echo "$job_status *$job_name*"
        continue
    elif [[ ${job_result} == *"timed_out"* ]]; then
        job_status=$timed_out_badge
        echo "$job_status *$job_name*"
    elif [[ ${job_result} == *"action_required"* ]]; then
        job_status=$action_required_badge
        echo "$job_status *$job_name*"
    elif [[ ${job_result} == *"failure"* ]]; then
        job_status=$failed_badge
        echo "$job_status *$job_name*"
    fi
    
    steps=$(echo "$job" | jq ".steps")
    steps_count=$(echo "$steps" | jq ". | length")
    for((j=0;j<steps_count;j++)); do

        step=$(echo "$steps" | jq ".[$j]")
        step_name=$(echo "$step" | jq ".name")
        step_result=$(echo "$step" | jq ".conclusion")

        step_start_time=$(echo "$step" | jq ".started_at")
        step_end_time=$(echo "$step" | jq ".completed_at")

        # Get appropriate status badge
        if [[ ${step_result} == *"success"* ]]; then
            step_status=$success_badge
        elif [[ ${step_result} == *"skipped"* ]]; then
            step_status=$skipped_badge
        elif [[ ${step_result} == *"cancelled"* ]]; then
            step_status=$skipped_badge
        elif [[ ${step_result} == *"timed_out"* ]]; then
            step_status=$timed_out_badge
        elif [[ ${step_result} == *"failure"* ]]; then
            step_status=$failed_badge
        fi

        # Get appropriate tree symbol
        if (( j == steps_count-1 )); then
            tree_symb="$tree_angle"
        else
            tree_symb="$tree_t"
        fi

        echo " $tree_symb $step_status $step_name"

    done
done

