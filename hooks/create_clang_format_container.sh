if ! podman images | grep -q "clang-format:14"; then
podman build -t clang-format:14 - <<EOF
FROM ubuntu:latest
RUN apt-get update && apt-get install -y clang-format-14 python3 git
EOF
fi
