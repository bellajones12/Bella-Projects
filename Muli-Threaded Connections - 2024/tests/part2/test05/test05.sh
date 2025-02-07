DIFF=$(diff <(./btide config.cfg < test05/test05.in) test05/test05.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    echo "Test 5 passed!"
    echo ""
else
    echo "Test 5 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi