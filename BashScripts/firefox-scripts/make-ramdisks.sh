#!/bin/sh

MOUNT_DIR=$HOME/firefox-cc
BACKUP_CONFIG=$HOME/firefox-config.tar.gz

#Clear previous session
rm -rf $MOUNT_DIR/config
rm -rf $MOUNT_DIR/cache
rm -rf $HOME/.cache/mozilla/firefox
rm -rf $HOME/.mozilla/firefox

#Create links
ln -s $MOUNT_DIR/config  $HOME/.mozilla/firefox
ln -s $MOUNT_DIR/cache   $HOME/.cache/mozilla/firefox

#Create cache and config dirs on ramdisk
mkdir $MOUNT_DIR/config
mkdir $MOUNT_DIR/cache

#Extract files into config dir
tar -zxf $BACKUP_CONFIG -C $MOUNT_DIR/config
