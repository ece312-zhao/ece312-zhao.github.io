#! /usr/bin/env bash

set -eu -o pipefail

SERVER_PATH=/misc/scratch/$(whoami)/.vscode-server
DEFAULT_PATH=$HOME/.vscode-server

if [ -d $DEFAULT_PATH ]; then
    echo "Found .vscode-server directory at $DEFAULT_PATH. Aborting"
    exit 0
fi

echo "Path to vscode server folder: $SERVER_PATH"

mkdir -p $SERVER_PATH

ln -s $SERVER_PATH $DEFAULT_PATH

echo "Symbolic link set"
