DIFF=$(diff <(./btide config.cfg < test04/test04.in) test04/test04.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    echo "Test 4 passed!"
    echo ""
else
    echo "Test 4 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi