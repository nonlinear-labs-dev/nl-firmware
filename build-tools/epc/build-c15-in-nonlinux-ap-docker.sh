executeInNonlinuxAp() {
    sshpass -p "sscl" ssh -o ConnectTimeout=2 -o StrictHostKeyChecking=no -p 5022 sscl@localhost "$1"
    return $? 
}

touch serial.log
qemu-system-x86_64 -chardev stdio,id=char0,logfile=serial.log,signal=off -serial chardev:char0 -display none -nic user,hostfwd=tcp::5022-:22 -smp 8 -m 8G -hda /workdir/disk.raw -drive if=pflash,format=raw,readonly,file=/usr/share/OVMF/OVMF_CODE.fd&
tail -f serial.log &

while ! executeInNonlinuxAp "echo"; do sleep 1; echo "Waiting for ssh server..." && date; done
tar -zc /sources -O | executeInNonlinuxAp "tar zxf -"
executeInNonlinuxAp "mkdir build && cd build && cmake -DBUILD_BBBB=Off -DBUILD_BBB=Off -DBUILD_EPC=Off -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_ONLINEHELP=On /home/sscl/sources"
cat /workdir/gwt-2.8.2.zip | executeInNonlinuxAp "dd of=build/projects/NonMaps/gwt-2.8.2.zip"
executeInNonlinuxAp "cd build && make -j4" 
executeInNonlinuxAp "cd build && sudo make install" 
executeInNonlinuxAp "rm -rf build"
executeInNonlinuxAp "rm -rf /home/sscl/sources"
executeInNonlinuxAp "cd / && ./createUpdateFromRunningOs.sh"
sshpass -p "sscl" rsync -e "ssh -o StrictHostKeyChecking=no -p 5022" sscl@localhost:/update.tar /workdir/
