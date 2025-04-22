# suricata

## build
apt -y install autoconf automake build-essential cargo \
    cbindgen libjansson-dev libpcap-dev libpcre2-dev libtool \
    libyaml-dev make pkg-config rustc zlib1g-dev
    
cp dist/rust-bindings.h /usr/local/include/
make CFLAGS="-I/usr/local/include" LDFLAGS="-L/usr/local/lib -lparser" -j10 && make install

## function
1. decode.c (PacketSetData).line.784-> line_no&&task_id

2. detect.c (Detect).line.2170->OutPutDetectPacket

3. output-json-alert.c (AlertOutPut).line.1228->OutPutAlertPacket

## generate the install package (.tar.gz)
./build.sh