if ! docker images | grep -q "clang-format:14"; then
docker build -t clang-format:14 - <<EOF
FROM ubuntu:latest

RUN apt-get update && apt-get install -y clang-format-14
EOF
fi
