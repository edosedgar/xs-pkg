#!/bin/sh

MOUNT_DIR=$HOME/google-chrome-cc
BACKUP_CONFIG=$HOME/google-config.tar.gz

if [ -z "$(ls -A $MOUNT_DIR/config)" ]; then
        true
else
        mv $BACKUP_CONFIG $BACKUP_CONFIG.old
        cd $MOUNT_DIR/config
        tar -czf $BACKUP_CONFIG ./*
        rm $BACKUP_CONFIG.old
fi

