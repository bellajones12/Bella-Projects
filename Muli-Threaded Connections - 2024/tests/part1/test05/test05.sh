DIFF=$(diff <(./pkgmain test05/test05.bpkg -chunk_check) test05/test05.expected)

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
