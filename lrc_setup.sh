EID=$1
SERVER_PATH=/misc/scratch/$EID
echo "path to vscode server folder: $SERVER_PATH"
mkdir -p $SERVER_PATH
mkdir -p $SERVER_PATH/.vscode-server
ln -s $SERVER_PATH/.vscode-server .vscode-server
echo "symbolic link set"
