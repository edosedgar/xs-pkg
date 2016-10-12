#!/usr/bin/bash

WATCHDOG=65536
USERADD_OPT=""
FILE_NAME=""
CHECK_HOME_DIR=""
USERADD="useradd"
# Current values
USER_NAME=""
HASH_PASSWD=""
GROUPS_M=""
UID_M=""
HOME_DIR=""
INFO=""
SHELL_M=""

# Show help
if [[ "$1" == "-h" || "$1" == "--help" ]];
then
        echo "Usage: [OPTION] FILE"
        echo "Add users, who were decribed in FILE, in system."
        echo "Format records in FILE:"
        echo "<USER_NAME>:<HASH_PASSWD>:<GROUPS>:<UID>:<HOME_DIR>:<INFO>:SHELL>"
        echo ""
        echo "<USER_NAME>       - user name to log in."
        echo "<HASH_PASSWD>     - hashed password by crypt()."
        echo "<GROUPS>          - additional groups, items should be separated"
        echo "                    by a comma."
        echo "<UID>             - user id, the default is determinated by useradd."
        echo "<HOME_DIR>        - home directory of new user, but take into"
        echo "                    account, that script doesn't allow to attach one"
        echo "                    home directory to 2 and more users. If folder"
        echo "                    doesn't exist, it will be created."
        echo "<INFO>            - additional info about user(name, phone etc.)."
        echo "<SHELL>           - user shell, default is /usr/bin/nologin."
        echo ""
        echo "NOTE: Script should be executed as root."
        echo "Author: Edgar Kaziahmedov edos@linux.com"
        exit 0;
fi

# Check on root
CUR_USER=`id -u`
if [ "$CUR_USER" -ne 0 ];
then
        echo "Please log in as root.";
        echo "Try '""$0"" --help' for more information."
        exit 1;
fi

# Check on existance FILE
FILE_NAME="$1"
if [ ! -f "$PWD"/"$FILE_NAME" ];
then
        echo "File doesn't exist!"
        exit 1;
fi

for i in `seq 1 "$WATCHDOG"`
do
        USERADD_OPT=""
        USER_NAME=`cat "$PWD"/"$FILE_NAME" | awk -F ":" -v it="$i" 'NR==it {print $1; exit 0}'`
        HASH_PASSWD=`cat "$PWD"/"$FILE_NAME" | awk -F ":" -v it="$i" 'NR==it {print $2; exit 0}'`
        GROUPS_M=`cat "$PWD"/"$FILE_NAME" | awk -F ":" -v it="$i" 'NR==it {print $3; exit 0}'`
        UID_M=`cat "$PWD"/"$FILE_NAME" | awk -F ":" -v it="$i" 'NR==it {print $4; exit 0}'`
        HOME_DIR=`cat "$PWD"/"$FILE_NAME" | awk -F ":" -v it="$i" 'NR==it {print $5; exit 0}'`
        INFO=`cat "$PWD"/"$FILE_NAME" | awk -F ":" -v it="$i" 'NR==it {print $6; exit 0}'`
        SHELL_M=`cat "$PWD"/"$FILE_NAME" | awk -F ":" -v it="$i" 'NR==it {print $7; exit 0}'`
        # Check on EOF
        if [[ "$USER_NAME" == "" &&  "$HASH_PASSWD" == "" && "$GROUPS_M" == "" &&  
              "$UID_M" == "" && "$HOME_DIR" == "" && "$INFO" == "" && "$SHELL_M" == "" ]];
        then
                exit 0;
        fi
        # Let's process all options
        if [[ -z "${USER_NAME// }" ]];
        then
                echo "<USER_NAME> on line ""$i"" wasn't set. Line was skipped."
                continue
        fi
        USERADD_OPT="${USER_NAME// }"
        # HASH_PASSWD
        if [[ ! -z "${HASH_PASSWD// }" ]];
        then
                USERADD_OPT=$USERADD_OPT" -p ""${HASH_PASSWD// }"       
        fi
        # GROUPS
        if [[ ! -z "${GROUPS_M// }" ]];
        then
                USERADD_OPT=$USERADD_OPT" -G ""${GROUPS_M// }"       
        fi
        # UID
        if [[ ! -z "${UID_M// }" ]];
        then
                USERADD_OPT=$USERADD_OPT" -o -u ""${UID_M// }"       
        fi
        # HOME_DIR
        if [[ ! -z "${HOME_DIR// }" ]];
        then    
                CHECK_HOME_DIR=`cat /etc/passwd | awk -F ":" -v HOME_DIR="${HOME_DIR// }" '{if ($6 == HOME_DIR) print 1}'`
                if [ "$CHECK_HOME_DIR" == "1" ];
                then
                        echo "<HOME_DIR> on line ""$i"" is used by another user. Line was skipped."
                        continue
                fi
                USERADD_OPT=$USERADD_OPT" -m -d ""${HOME_DIR// }"
        fi 
        # INFO
        if [[ ! -z "${INFO// }" ]];
        then
                USERADD_OPT=$USERADD_OPT" -c ""$INFO"       
        fi
        # SHELL
        if [[ ! -z "${SHELL_M// }" ]];
        then    
                USERADD_OPT=$USERADD_OPT" -s ""$SHELL_M" 
        else
                USERADD_OPT=$USERADD_OPT" -s /bin/bash"
        fi
        # Main action
        #"$USERADD"" ""$USERADD_OPT"
        # Check 
        if [ $? -eq 9 ];
        then 
                echo "<USER_NAME> on line ""$i"" exists, choose another. Line was skipped."
                continue
        fi
        # Well done
        echo "$i"". ""$USERADD"" ""$USERADD_OPT"" [DONE]"
done
