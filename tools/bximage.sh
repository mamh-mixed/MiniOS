#!/bin/bash

if [ -e os.img ] ; then
    echo 'os.img is alreay exists' > /dev/null
else 
    bximage -q -mode=create -hd=100 -imgmode=flat bin/os.img
fi