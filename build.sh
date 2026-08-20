#!/usr/bin/env bash
# build.sh - KEE portable build script (Linux / macOS)
# Usage:
#   ./build.sh [x64|x86] [Debug|Release]
#   ./build.sh            # builds x64 Release (default)
#   ./build.sh x86 Debug
set -e

ARCH="${1:-x64}"
CONFIG="${2:-Release}"

echo "[KEE] Building ${ARCH} ${CONFIG} on $(uname -s) ..."

# -- Prerequisites check ------------------------------------------------
for cmd in cmake pkg-config; do
    if ! command -v "$cmd" &>/dev/null; then
        echo "[ERROR] '$cmd' not found. Install it and re-run."
        exit 1
    fi
done

# Check for protobuf
if ! pkg-config --exists protobuf 2>/dev/null; then
    echo "[WARN] protobuf not found via pkg-config."
    echo "       On Ubuntu/Debian:  sudo apt install libprotobuf-dev protobuf-compiler"
    echo "       On Fedora/RHEL:    sudo dnf install protobuf-devel"
    echo "       On macOS:          brew install protobuf"
    echo "       Continuing - the build may fail if protobuf is not on the include path."
fi

# -- CMake configure + build -------------------------------------------
if [ "$ARCH" = "x86" ]; then
    CMAKE_ARCH_FLAGS="-DCMAKE_C_FLAGS=-m32 -DCMAKE_CXX_FLAGS=-m32"
else
    CMAKE_ARCH_FLAGS=""
fi

BUILD_DIR="build_cmake_${ARCH}_${CONFIG}"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. \
    -DCMAKE_BUILD_TYPE="${CONFIG}" \
    ${CMAKE_ARCH_FLAGS}

JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
cmake --build . --config "${CONFIG}" --parallel "${JOBS}"

cd ..

echo
echo "[KEE] Build complete."
echo "Output in: ${BUILD_DIR}/bin/${ARCH}/${CONFIG}/"