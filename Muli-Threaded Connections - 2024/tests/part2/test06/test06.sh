DIFF=$(diff <(./btide config.cfg < test06/test06.in) test06/test06.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    echo "Test 6 passed!"
    echo ""
else
    echo "Test 6 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi