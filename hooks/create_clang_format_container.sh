if ! podman images | grep -q "clang-format:14"; then
    podman build -t clang-format:14 $1/hooks > /dev/null
fi
