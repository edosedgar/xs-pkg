#!/usr/bin/bash

USER_NAME=""
USER_GROUPS=""
HOME_DIRECTORY=""
HASH_PASSWD=""
PASSWD=""
WATCHDOG=65536

CUR_USER=`id -u`
if [ "$CUR_USER" -ne 0 ];
then
        echo "Please log in as root.";
        exit 1;
fi

for i in seq 1 "$WATCHDOG"
do
        #Enter user name
        while [[ -z "${USER_NAME// }" ]]
        do
                echo "Enter user name:";
                read -r USER_NAME;
        done
        #Enter list of groups
        echo "Enter preferred groups(separated by a comma, without whitespaces):";
        read -r USER_GROUPS;
        #Enter home directory
        while [[ -z "${HOME_DIRECTORY// }" ]] 
        do
                echo "Enter your home directory(if it doesn't exist, it will be created):";
                read -r HOME_DIRECTORY; 
        done
        #Enter hash is created by crypt(sha-512 alg with sault)
        while [[ -z "${HASH_PASSWD// }" ]]
        do
                echo "Enter password hash:"
                read -r HASH_PASSWD;
        done

        useradd -d "$HOME_DIRECTORY" -G "$USER_GROUPS" -m -p "$HASH_PASSWD" "$USER_NAME"

        if [ $? -ne 0 ];
        then
                echo "useradd failed";
                exit $?;
        fi

        USER_NAME="";
        USER_GROUPS="";
        HOME_DIRECTORY="";
        HASH_PASSWD="";
done

