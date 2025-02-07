DIFF=$(diff <(./pkgmain test08/test08.bpkg -all_hashes) test08/test08.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    echo "Test 8 passed!"
    echo ""
else
    echo "Test 8 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi
