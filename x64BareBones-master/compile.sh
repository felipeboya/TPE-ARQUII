#!/bin/bash
docker start TPE-ARQUI 
docker exec -it TPE-ARQUI make clean -C /root/Toolchain
docker exec -it TPE-ARQUI make clean -C /root/
docker exec -it TPE-ARQUI make -C /root/Toolchain
docker exec -it TPE-ARQUI make -C /root/
docker stop TPE-ARQUI 
