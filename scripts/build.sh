#!/bin/bash

target_folder=$(pwd)/build

echo "[*] Clearing previous builds"
rm -rf $target_folder

echo "[*] Creating required folders"
mkdir -p $target_folder
mkdir -p dist/static
mkdir -p dist/shared
mkdir -p bin

echo "[*] Compiling Static library"
make static

echo "[*] Compiling Shared Library"
make shared

echo "[*] Finishing installation"
cp -r ./dist/static $target_folder/static/
cp -r ./dist/shared $target_folder/shared/
cp -r ./includes/ $target_folder/includes/
cp -r ./bin/ $target_folder/bin/