#!/bin/bash
# ==============================================
# | Author: Taylo @ https://github.com/i-Taylo |
# ==============================================
set -e

abort() {
    echo >&2 "$1"
    exit 1
}

command_exists() {
    command -v "$1" > /dev/null 2>&1
}

install_command() {
    if command_exists apt; then
        sudo apt install -y "$1"
    elif command_exists pacman; then
        sudo pacman -S --noconfirm "$1"
    elif command_exists dnf; then
        sudo dnf install -y "$1"
    elif command_exists yum; then
        sudo yum install -y "$1"
    elif command_exists apk; then
        sudo apk add "$1"
    elif command_exists pkg; then
        pkg install -y "$1"  # For Termux
    else
        abort "Package manager not supported. Please install $1 manually."
    fi
}

ROOT="$(pwd)"
BUILD_DIR="$ROOT/build"
DEPS=(make cmake g++)

[ -d $BUILD_DIR ] && rm -rf $BUILD_DIR/*
[ ! -d "$BUILD_DIR" ] && mkdir -p "$BUILD_DIR"

echo "Checking dependencies..."
for dep in "${DEPS[@]}"; do
    if ! command_exists "$dep"; then
        echo "Installing $dep..."
        install_command "$dep"
    else
        echo "$dep is already installed."
    fi
done

echo "Entering -> $BUILD_DIR"
cd "$BUILD_DIR" || abort "Failed to enter build directory."

echo "Running CMake..."
cmake .. || abort "CMake configuration failed."

echo "Building project..."
make || abort "Build process failed."

echo "Build completed successfully. Leaving -> $BUILD_DIR"
