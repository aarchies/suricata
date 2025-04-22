#/bin/bash
 
make clean
tar --warning=no-file-changed --exclude=suricata-7.0.5.tar.gz -czvf suricata-7.0.5.tar.gz .