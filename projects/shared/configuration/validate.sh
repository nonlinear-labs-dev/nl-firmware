# run process
echo "starting process";
bash process.sh
succeeded=$?
if [ $succeeded -ne 0 ]; then
    echo "process failed";
    exit $succeeded
fi

# compile validation
echo "validation - starting g++";
g++ ./generated/validate.cpp -o ./generated/validate
if [ $? -ne 0 ]; then
    echo "validation - g++ failed";
    exit 5
fi

# run generated validation
echo "validation - starting ./generated/validate";
./generated/validate
if [ $? -ne 0 ]; then
    echo "validation - ./generated/validate failed";
    exit 6
fi
