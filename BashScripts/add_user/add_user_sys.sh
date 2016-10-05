#!/usr/bin/bash

USER_NAME=""
USER_GROUPS=""
HOME_DIRECTORY=""
HASH_PASSWD=""
PASSWD=""
MAX_USERS=65536

CUR_USER=`id -u`
if [ "$CUR_USER" -ne 0 ];
then
        echo "Please log in as root.";
        exit 1;
fi

for i in seq 1 "$MAX_USERS"
do
        echo "Enter user name:";
        read -r USER_NAME;
        echo "Enter preferred groups(separated by a comma, without whitespaces):";
        read -r USER_GROUPS;
        echo "Enter your home directory(if it doesn't exist, it will be created):";
        read -r HOME_DIRECTORY; 
        echo "Enter password hash:"
        read -r HASH_PASSWD;
        useradd -d "$HOME_DIRECTORY" -G "$USER_GROUPS" -m -p "$HASH_PASSWD" "$USER_NAME"
done

