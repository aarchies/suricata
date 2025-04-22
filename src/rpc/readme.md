# 安装包依赖
# 安装 gRPC
sudo apt-get install -y libgrpc-dev libgrpc++-dev

# 安装 protobuf 和 protobuf-c
sudo apt-get install -y libprotobuf-dev protobuf-compiler
sudo apt-get install -y libprotobuf-c-dev protobuf-c-compiler

# 安装 pkg-config（如果没有的话）
sudo apt-get install -y pkg-config

# 生成
cd /root/suricata-7.0.5/src/rpc
protoc --c_out=. packet.proto
protoc --grpc-c_out=. --plugin=protoc-gen-grpc-c=/usr/local/bin/protoc-gen-grpc-c packet.proto