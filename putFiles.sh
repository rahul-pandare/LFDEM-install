#!/bin/bash
# Script to put files on AWS VM

KEY_PATH="/Users/rahul/keys/aws.pem" # Key path
IP="3.134.253.90" # Machine IP

chmod 400 "$KEY_PATH"

scp -i "$KEY_PATH" SuiteSparse-5.4.0.zip ubuntu@"$IP":
scp -i "$KEY_PATH" lfdem.zip ubuntu@"$IP":
scp -i "$KEY_PATH" initial_Install.sh ubuntu@"$IP":

# NOTE
#1. Update the key path and ip address to the instance created in this file.
#2. Run this file from the $PWD to put the required files
#3. Then just run the 'installation.sh' from the cluster to install lfdem
#4. Make sure the instance is run on intel x86 chip