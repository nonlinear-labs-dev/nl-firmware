#!/bin/bash

sourceDir=$1
binaryDir=$2

# 1/5 - compile typescript via tsc
echo "1/5 - starting typescript compilation";
tsc "$sourceDir"/lib/build.ts
if [ $? -ne 0 ]; then
    echo "1/5 - typescript compilation failed";
    exit 1
fi

# 2/5 - execute compiled javascript via node
echo "2/5 - starting node";
node "$sourceDir"/lib/build.js /tmp/generated
if [ $? -ne 0 ]; then
    echo "2/5 - node failed";
    exit 2
fi

# 3/5 - compile generated headers
echo "3/5 - starting g++ build of tests";
mkdir -p /tmp/generated/build
g++ /tmp/generated/src/main.cpp -o /tmp/generated/build/tests
if [ $? -ne 0 ]; then
    echo "3/5 - g++ failed";
    exit 3
fi

# 4/5 - run generated test-application
echo "4/5 - starting tests";
./tmp/generated/build/tests
if [ $? -ne 0 ]; then
    echo "4/5 - tests failed";
    exit 4
fi

#copy sources
echo "5/5 - copy generated headers into appropriate directories"
/sourceDir/copy-results-to-source.sh /tmp "$binaryDir"
if [ $? -ne 0 ]; then
  echo "5/5 - copying of headers to build-dir failed";
  exit 5
fi
