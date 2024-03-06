#!/bin/bash


# start clam daemon
/usr/sbin/clamd

# start clam on-access daemon
/usr/sbin/clamonacc
while [ $? -ne 0 ]; do 
    /usr/sbin/clamonacc
done

# web server
python3 /app/app.py