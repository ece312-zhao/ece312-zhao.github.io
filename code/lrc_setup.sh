#! /usr/bin/env bash

set -eu -o pipefail

SERVER_PATH=/misc/scratch/$(whoami)/.vscode-server

echo "Path to vscode server folder: $SERVER_PATH"

mkdir -p $SERVER_PATH

ln -s $SERVER_PATH $HOME/.vscode-server

echo "Symbolic link set"
