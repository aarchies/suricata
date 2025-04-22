# Suricata

## 编译
apt -y install autoconf automake build-essential cargo \
    cbindgen libjansson-dev libpcap-dev libpcre2-dev libtool \
    libyaml-dev make pkg-config rustc zlib1g-dev
    
cp dist/rust-bindings.h /usr/local/include/
make CFLAGS="-I/usr/local/include" LDFLAGS="-L/usr/local/lib -lparser" -j10 && make install

## 函数修改部分
1. decode.c (PacketSetData).line.784-> 
line_no
task_id
增加线路号及任务id解析

2. detect.c (Detect).line.2170->
OutPutDetectPacket 调用go函数Entrance

3. output-json-alert.c (AlertOutPut).line.1228->
OutPutAlertPacket  调用go函数Entrance

## 生成安装包 (这将在根目录生成源码包.tar.gz)
./build.sh