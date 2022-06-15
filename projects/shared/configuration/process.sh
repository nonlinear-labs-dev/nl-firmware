# 1/4 - compile typescript via tsc
echo "1/4 - starting typescript compilation";
tsc ./lib/build.ts
if [ $? -ne 0 ]; then
    echo "1/4 - typescript compilation failed";
    exit 1
fi

# 2/4 - execute compiled javascript via node
echo "2/4 - starting node";
node ./lib/build.js
if [ $? -ne 0 ]; then
    echo "2/4 - node failed";
    exit 2
fi

# 3/4 - compile generated headers
echo "3/4 - starting g++";
g++ ./generated/main.cpp -o ./generated/main
if [ $? -ne 0 ]; then
    echo "3/4 - g++ failed";
    exit 3
fi

# 4/4 - run generated main
echo "4/4 - starting ./generated/main";
./generated/main
if [ $? -ne 0 ]; then
    echo "4/4 - ./generated/main failed";
    exit 4
fi
