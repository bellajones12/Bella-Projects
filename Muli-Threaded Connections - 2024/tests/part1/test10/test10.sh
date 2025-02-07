DIFF=$(diff <(./pkgmain test10/test10.bpkg -file_check) test10/test10.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    if [ -e "file1.data" ]; then
        echo "Test 10 passed"
        echo ""
    else
        echo "Test 10 failed"
        echo "File did not exist"
        echo ""
    fi
else
    echo "Test 10 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi
