#!/bin/sh

MOUNT_DIR=$HOME/google-chrome-cc
BACKUP_CONFIG=$HOME/google-config.tar.gz

#Clear previous session
rm -rf $MOUNT_DIR/config
rm -rf $MOUNT_DIR/cache
rm -rf $HOME/.cache/google-chrome
rm -rf $HOME/.config/google-chrome

#Create links
ln -s $MOUNT_DIR/config  $HOME/.config/google-chrome
ln -s $MOUNT_DIR/cache   $HOME/.cache/google-chrome

#Create cache and config dirs on ramdisk
mkdir $MOUNT_DIR/config
mkdir $MOUNT_DIR/cache

#Extract files into config dir
tar -zxf $BACKUP_CONFIG -C $MOUNT_DIR/config
