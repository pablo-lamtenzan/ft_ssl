#! /bin/bash

EXEC=${1:-ft_ssl}
DEST_MINE="mine.log"
DEST_SSL="ssl.log"

ALGORITHMS=("md5" "sha1" "sha224" "sha256" "sha384" "sha512")

HAS_ERROR=0
CMD=
ALGO=

# $1 msg_lengh
function check_one()
{
    CMD="python3 -c \"print('a'*${1})\" | <exec>"
    python3 -c "print('a'*${1})" | openssl $ALGO > $DEST_SSL
    python3 -c "print('a'*${1})" | ./${EXEC} $ALGO > $DEST_MINE
    diff $DEST_SSL $DEST_MINE
    if [ $? -eq 0 ] ; then
        CHECK_RES=1
    else
        CHECK_RES=0
    fi
}

# $1=Test nb
function print_result()
{
    echo -n $'\033[36m'"Test #${1}:"$'\033[33m'"'${CMD}'" $'\033[0m'
    if [ $CHECK_RES -eq 1 ] ; then
        echo $'\033[32m'"[PASS]"$'\033[0m'
    else
        echo $'\033[31m'"[FAIL]"$'\033[0m'
        HAS_ERROR=1
    fi
}

if [ ! -f $EXEC ] ; then
    echo "error: target executable $EXEC not found."
fi

for name in ${ALGORITHMS[@]}; do
    ALGO=$name
    echo $'\033[33m'"TEST ALGO:" $'\033[36m'${ALGO}$'\033[0m'
    check_one "10"
    print_result "1"
    check_one "63"
    print_result "2"
    check_one "61"
    print_result "3"
    check_one "420420"
    print_result "4"
    check_one "696969"
    print_result "5"
    check_one "0"
    print_result "6"
done

rm $DEST_MINE $DEST_SSL
exit $HAS_ERROR
