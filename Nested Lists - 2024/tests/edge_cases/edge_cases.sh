for file in $(find /home/comp2017/multi-type-linked-list/tests/edge_cases -type f -name "*.in"); do
    if [ -f "$file" ]; then
        argument="$(cat "$file")"
        argument2=$(echo "$file" | cut -d '.' -f1).expected
        diff <(./mtll <<< "$argument") "$argument2"
    fi
done