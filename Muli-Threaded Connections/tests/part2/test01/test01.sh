DIFF=$(diff <(./btide config.cfg < test01/test01.in) test01/test01.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    echo "Test 1 passed!"
    echo ""
else
    echo "Test 1 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi