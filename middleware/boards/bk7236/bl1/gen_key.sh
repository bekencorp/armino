#!/bin/bash

SCH=$1
PRVKEY_FILE_NAME=$2
PUBKEY_FILE_NAME=$3

if [ "$SCH" == "ec256" ]; then
	openssl ecparam -name secp256r1 -genkey -out $PRVKEY_FILE_NAME
	openssl ec -in $PRVKEY_FILE_NAME -inform PEM -out $PUBKEY_FILE_NAME -pubout
elif [ "$SCH" == "ec521" ]; then
	openssl ecparam -name secp521r1 -genkey -out $PRVKEY_FILE_NAME
	openssl ec -in $PRVKEY_FILE_NAME -inform PEM -out $PUBKEY_FILE_NAME -pubout
elif [ "$SCH" == "rsa1024" ]; then
	openssl genrsa -out $PRVKEY_FILE_NAME 1024
	openssl rsa -in $PRVKEY_FILE_NAME -inform PEM -out $PUBKEY_FILE_NAME -pubout
elif [ "$SCH" == "rsa2048" ]; then
	openssl genrsa -out $PRVKEY_FILE_NAME 2048
	openssl rsa -in $PRVKEY_FILE_NAME -inform PEM -out $PUBKEY_FILE_NAME -pubout
else
	echo "Invalid scheme name ${SCH}"
fi

