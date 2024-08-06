OUTPUT=$(./pkgmain test07/test07.bpkg -hashes_of 496a143ef4454083095f2792268914c5e7007c4e71c9ec0623c2b2b5952cjfie)

# Check if the output is empty
if [ -z "$OUTPUT" ]; then
    echo "Test 7 passed!"
    echo ""
else
    echo "Test 7 failed"
    echo "Unexpected output:"
    echo "$OUTPUT"
    echo ""
fi
