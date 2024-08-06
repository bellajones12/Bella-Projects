OUTPUT=$(./btide config.cfg < test02/test02.in)


if [ -z "$OUTPUT" ]; then
    echo "Test 2 passed!"
    echo ""
else
    echo "Test 2 failed"
    echo "Unexpected output:"
    echo "$OUTPUT"
    echo ""
fi
