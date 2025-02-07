OUTPUT=$(./pkgmain test02/test02.bpkg -all_hashes)


if [ -z "$OUTPUT" ]; then
    echo "Test 2 passed!"
    echo ""
else
    echo "Test 2 failed"
    echo "Unexpected output:"
    echo "$OUTPUT"
    echo ""
fi
