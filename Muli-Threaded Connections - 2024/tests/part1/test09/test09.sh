DIFF=$(diff <(./pkgmain test09/test09.bpkg -file_check) test09/test09.expected)

# Check if the diff command produced any output
if [ $? -eq 0 ]; then
    if [ -e "file2.data" ]; then
        echo "Test 9 passed"
        echo ""
    else
        echo "Test 9 failed"
        echo "File did not exist"
        echo ""
    fi
else
    echo "Test 9 failed"
    echo "Different output:"
    echo "$DIFF"
    echo ""
fi


