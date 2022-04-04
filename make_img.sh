#!/bin/bash
# set -x # echo on
img_path="build/foo.img"
img_files="build/img.img/*"
BLOCKSIZE=512
NUM_BLOCKS=32768

dd if=/dev/zero of=$img_path bs=$BLOCKSIZE count=$NUM_BLOCKS
parted $img_path mklabel msdos # add a MBR
parted $img_path mkpart primary fat32 2048s 30270s # add a partiton
parted $img_path set 1 boot on # set partition boot flag
base_img_block_dev=$(sudo losetup -f) # get next block device
sudo losetup $base_img_block_dev $img_path # mount entire disk
part_img_block_dev=$(sudo losetup -f) # get next block device
sudo losetup $part_img_block_dev $img_path -o 1048576 # mount partition
sudo mkdosfs -F32 -f2 $part_img_block_dev # format part with FAT32
sudo mount $part_img_block_dev /mnt/fatgrub # mount part
sudo cp -r $img_files /mnt/fatgrub # copy files
grub-install --root-directory=/mnt/fatgrub --no-floppy -d /usr/lib/grub/i386-pc --modules="normal part_msdos ext2 multiboot" $base_img_block_dev 
sudo umount /mnt/fatgrub
sudo losetup -d $base_img_block_dev
sudo losetup -d $part_img_block_dev
