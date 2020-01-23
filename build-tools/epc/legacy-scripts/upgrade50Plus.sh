#!/bin/bash

IP=$1

t2s() {
    /nonlinear/text2soled/text2soled multitext "- Do not switch off! -@s2c" "$1" "$2" "$3" "$4" "$5" "$6"
}

pretty() {
    echo "pretty arguments: $*"
    HEADLINE="$1"
    BOLED_LINE_1="$2"
    BOLED_LINE_2="$3"
    SOLED_LINE_1="$4"
    SOLED_LINE_2="$5"

    t2s "${HEADLINE}@b1c" "${BOLED_LINE_1}@b3c" "${BOLED_LINE_2}@b4c" "${SOLED_LINE_1}@s0c" "${SOLED_LINE_2}@s1c"
}

executeAsRoot() {
    echo "sscl" | sshpass -p 'sscl' ssh sscl@$IP "sudo -S /bin/bash -c '$1' 1>&2 > /dev/null"
    return $?
}

quit() {
    pretty "$1" "$2" "$3" "$4" "$5" "$6"
    exit 1
}

print_scp_progress() {
    TARGET_SIZE="0"
    SOURCE_SIZE=$(ls -lah $2 | awk {'print $5'})

    TARGET_SIZE_BYTES="0"
    SOURCE_SIZE_BYTES=$(ls -la $2 | awk {'print $5'})
    
    executeAsRoot "touch $2"

    while [ ! "$TARGET_SIZE_BYTES" = "$SOURCE_SIZE_BYTES" ]; do
        TARGET_SIZE=$(sshpass -p 'sscl' ssh sscl@$IP "ls -lah $3 | awk {'print \$5'}")
        TARGET_SIZE_BYTES=$(sshpass -p 'sscl' ssh sscl@$IP "ls -la $3 | awk {'print \$5'}")
        pretty "Copying..." "partition $1 content" "$TARGET_SIZE of $SOURCE_SIZE" "Copying partition $1" "$TARGET_SIZE / $SOURCE_SIZE"
        sleep 1
    done
}

print_dd_progress() {
    MSG=$1
    FILE=$2    
    touch $FILE
    while [ -e "$FILE" ]; do
        OUT=$(cat $FILE | tr '\r' '\n' | tail -n1 | grep -o "[0-9]* bytes")
        pretty "Dumping..." "partition $1 content" "$OUT Bytes." "Dumping partition $1" "$OUT Bytes."
        sleep 1 
    done    
} 


check_preconditions() {
    pretty "Checking preconditions..." "" "" "Checking" "preconditions..."
    [ -z "$IP" ] && quit "Usage:" "$0 <IP-of-ePC>" "" "wrong usage" ""
    ping -c1 $IP 1>&2 > /dev/null || quit "No connection!" "ePC is not reachable at" "$IP, update failed." "Update failed." "ePC not reachable."
    executeAsRoot "sfdisk -d /dev/sda | grep sda5 | grep 42049536" || quit "Unexpected partition" "ePC partition 5 is not at" "expected position, update failed." "Update failed." "partition error"
    executeAsRoot "sfdisk -d /dev/sda | grep sda5 | grep 20482422" || quit "Unexpected partition" "ePC partition 5 is not of" "expected size, update failed." "Update failed." "partition error"
    pretty "Checking preconditions" "done." "" "Checking preconditions" "done."
}

tear_down_playground() {
    pretty "Stopping C15 processes..." "" "" "Stopping C15" "processes..."
    systemctl stop playground
    systemctl stop bbbb
    pretty "Stopping C15 processes" "done." "" "Stopping C15" "processes done."
}

unmount_doomed() {
    pretty "Unmounting partitions..." "" "" "Unmounting partitions..." ""
    executeAsRoot "umount /boot/efi" || quit "Unmounting failed." "Unmounting /boot/efi failed," "update failed." "Update failed:" "umount failed."
    pretty "Unmounting partitions done." "" "" "Unmounting partitions" "done."
}

create_partitions() {
    pretty "Creating partitions..." "" "" "Creating partitions..." ""
    PART="label: gpt
          label-id: 7D22A5F5-C3A7-4C35-B879-B58C9B422919
          device: /dev/sda
          unit: sectors
          first-lba: 2048

          /dev/sda1 : start=        2048, size=     1048576, type=C12A7328-F81F-11D2-BA4B-00A0C93EC93B, uuid=37946461-1176-43E6-9F0F-5B98652B8AB9
          /dev/sda2 : start=     1050624, size=    16777216, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, uuid=274A1E65-7546-4887-86DD-771BAC588588
          /dev/sda3 : start=    17827840, size=    16777216, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, uuid=E30DE52F-B006-442E-9A4A-F332A9A0FF00
          /dev/sda4 : start=    34605056, size=     7444480, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, uuid=22c47cae-cf10-11e9-b217-6b290f556266
          /dev/sda5 : start=    42049536, size=    20482422, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, uuid=45c5a8ae-cf10-11e9-aefa-5f647edf4354"
    
    executeAsRoot "echo \"$PART\" | sfdisk --no-reread /dev/sda" || quit "Partitioning failed." "Could not repartition the" "ePC SSD, update failed." "Update failed:" "Partitioning failed."
    executeAsRoot "partprobe" || quit "Partitioning" "Could not re-read the SSD" "partition table. Update failed" "Update failed:" "re-read part. table failed"
    executeAsRoot "mkfs.fat /dev/sda1" || quit "Partitioning" "Could not make filesystem." "Update failed." "Update failed:" "mkfs part 1 failed"
    executeAsRoot "mkfs.ext4 /dev/sda2" || quit "Partitioning" "Could not make filesystem." "Update failed." "Update failed:" "mkfs part 2 failed"
    executeAsRoot "mkfs.ext4 /dev/sda3" || quit "Partitioning" "Could not make filesystem." "Update failed." "Update failed:" "mkfs part 3 failed"
    executeAsRoot "mkfs.ext4 /dev/sda4" || quit "Partitioning" "Could not make filesystem." "Update failed." "Update failed:" "mkfs part 4 failed"
    pretty "Creating partitions done." "" "" "Creating partitions" "done."
}

copy_partition_content() {
    pretty "Copying..." "partitions content." "Mounting partition 3..." "Copying..." "mounting partition 3"
    executeAsRoot "mount /dev/sda3 /mnt" || quit "Copying failed." "Could not mount partition 3." "Update failed." "Update failed:" "mount part 3 failed"
    
    pretty "Copying..." "partitions content." "Chmod partition 3..." "Copying..." "chmod partition 3"
    executeAsRoot "chmod 777 /mnt" || quit "Copying failed." "Could not chmod partition 3." "Update failed." "Update failed:" "chmod part 3 failed"
    
    print_scp_progress "1" ./p1.raw.gz /mnt/p1.raw.gz &
    sshpass -p 'sscl' scp ./p1.raw.gz sscl@$IP:/mnt || quit "Copying failed." "Could not copy p1.raw.gz." "Update failed." "Update failed:" "copy part 1 failed"
    
    print_scp_progress "2" ./p2.raw.gz /mnt/p2.raw.gz &
    sshpass -p 'sscl' scp ./p2.raw.gz sscl@$IP:/mnt || quit "Copying failed." "Could not copy p2.raw.gz." "Update failed." "Update failed:" "copy part 2 failed"
    
    pretty "Copying..." "partitions content done." "" "Copying..." "partitions done."
}

dd_partitions() {
    print_dd_progress "1" /tmp/dd1.log &
    executeAsRoot "cat /mnt/p1.raw.gz | gzip -d - | dd of=/dev/sda1 bs=1M status=progress" > /tmp/dd1.log 2>&1  || quit "Dumping failed." "Could not dump partition 1." "Update failed." "Update failed:" "dd /dev/sda1 failed"
    rm /tmp/dd1.log
    
    print_dd_progress "2" /tmp/dd2.log &
    executeAsRoot "cat /mnt/p2.raw.gz | gzip -d - | dd of=/dev/sda2 bs=1M status=progress" > /tmp/dd2.log 2>&1 || quit "Dumping failed." "Could not dump partition 2." "Update failed." "Update failed:" "dd /dev/sda2 failed"
    rm /tmp/dd2.log

    pretty "Dumping..." "done." "" "Dumping done." "" 
}

unmount_tmp() {
    pretty "Unmounting..." "temporary storage ..." "" "Unmounting" "tmp"
    executeAsRoot "umount /mnt" || quit "Unmounting failed." "Could not unmount temporary" "storage at /mnt. Update failed." "Update failed:" "umount /mnt failed"
    executeAsRoot "chmod 777 /mnt" || quit "Changing mode failed." "Could not change permissions" " of /mnt. Update failed." "Update failed:" "chmod /mnt"
    pretty "Unmounting..." "temporary storage" "done." "Unmounting" "tmp done."
}

copy_partition_3_content() {
    pretty "Copying..." "...partition 3 content" "to temporary storage." "Copying" "partition 3."
    print_scp_progress "3" ./p3.raw.gz /mnt/p3.raw.gz &
    sshpass -p 'sscl' scp ./p3.raw.gz sscl@$IP:/mnt || quit "Copying failed." "Could not copy partition 3" "content onto device. Update failed" "Update Failed:" "copy part 3 failed"
    pretty "Copying..." "...partition 3 content" "done." "Copying" "part 3 done."
}

dd_partition_3() {
    print_dd_progress "3" /tmp/dd3.log &
    executeAsRoot "cat /mnt/p3.raw.gz | gzip -d - | dd of=/dev/sda3 bs=1M status=progress" > /tmp/dd3.log 2>&1 || quit "Dumping failed." "Could not dd partition 3." "Update failed." "Update failed:" "dump part 3 failed"
    rm /tmp/dd3.log
    pretty "Dumping partition 3..." "...done." "" "Dumping part. 3..." "...done." 
}

install_grub() {
    pretty "Finalization..." "...mounting partition" "/dev/sda2" "Finalization..." "...mounting." 
    executeAsRoot "mount /dev/sda2 /mnt" || quit "Finalization failed" "Could not mount partition 2" "for installing grub. Update failed." "Update failed:" "mount sda2 failed"
    pretty "Finalization..." "...mounting partition" "/dev/sda1" "Finalization..." "...mounting." 
    executeAsRoot "mount /dev/sda1 /mnt/boot/" || quit "Finalization failed" "Could not mount partition 1" "for installing grub. Update failed." "Update failed:" "mount sda1 failed"
    pretty "Finalization..." "...mounting partition" "/dev" "Finalization..." "...mounting." 
    executeAsRoot "mount --rbind /dev /mnt/dev" || quit "Finalization failed" "Could not mount /dev" "for installing grub. Update failed." "Update failed:" "mount /dev failed"
    pretty "Finalization..." "...mounting partition" "/sys" "Finalization..." "...mounting." 
    executeAsRoot "mount --rbind /sys /mnt/sys" || quit "Finalization failed" "Could not mount /sys" "for installing grub. Update failed." "Update failed:" "mount /sys failed"
    pretty "Finalization..." "...mounting partition" "/proc" "Finalization..." "...mounting." 
    executeAsRoot "mount --rbind /proc /mnt/proc" || quit "Finalization failed" "Could not mount /proc" "for installing grub. Update failed." "Update failed:" "mount /proc failed"
    pretty "Finalization..." "...installing grub." "(1/2)" "Finalization..." "...installing grub." 
    executeAsRoot "chroot /mnt grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=arch_grub --recheck" || quit "Finalization failed" "Could not install grub." "Update failed." "Update failed:" "grub-install failed"
    pretty "Finalization..." "...installing grub." "(2/2)" "Finalization..." "...installing grub." 
    executeAsRoot "chroot /mnt grub-mkconfig -o /boot/grub/grub.cfg" || quit "Finalization failed" "Could not install grub." "Update failed." "Update failed:" "grub-mkconfig failed"
    pretty "Finalization..." "...make cpio." "" "Finalization..." "...make cpio." 
    executeAsRoot "chroot /mnt mkinitcpio -p linux-rt" || quit "Finalization failed" "Could not make cpio." "Update failed." "Update failed:" "mkinitcpio failed"
    pretty "Finalization done." "" "" "Finalization done." "" 
}

reboot_device() {
    pretty "Rebooting ePC..." "Please wait while" "the ePC is rebooting." "Rebooting ePC..." "Please wait."

    executeAsRoot "reboot"
    
    sleep 5
    while ! ping -c1 $IP; do 
        sleep 1
    done
    
    pretty "" "Your C15 has been" "successfully upgraded!" "Your C15 has been" "successfully upgraded."
}

start_playground() {
    systemctl start bbbb
    systemctl start playground
}

main() {
    check_preconditions
    tear_down_playground
    unmount_doomed
    create_partitions
    copy_partition_content
    dd_partitions
    unmount_tmp
    copy_partition_3_content
    dd_partition_3
    install_grub
    reboot_device
    start_playground
    exit 0
}

main









