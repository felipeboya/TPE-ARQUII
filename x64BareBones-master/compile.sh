#!/bin/bash
docker start tpe-arqui 
docker exec -it tpe-arqui make clean -C /root/Toolchain
docker exec -it tpe-arqui make clean -C /root/
docker exec -it tpe-arqui make -C /root/Toolchain
docker exec -it tpe-arqui make -C /root/
docker stop tpe-arqui 
