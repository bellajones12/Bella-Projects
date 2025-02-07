DIFF=$(diff <(./btide config.cfg < test03/test03.in) test03/test03.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    echo "Test 3 passed!"
    echo ""
else
    echo "Test 3 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi