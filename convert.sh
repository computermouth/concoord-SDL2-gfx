#!/bin/bash

#name
NAME="$(xmlstarlet sel -N x="http://www.w3.org/2000/svg" -t -m //x:path -v @id ${1})"

#path
BOTH=$(xmlstarlet sel -N x="http://www.w3.org/2000/svg" -t -m //x:path -v @d ${1} | sed -s 's/M.*C //g' | sed -s 's/              /\n/g' | sed -s 's/ Z//g' | awk '{print $1}')

#x
X=$(echo "${BOTH}" | rev | cut -d"," -f2- | cut -d"." -f2- | rev)
Y=$(echo "${BOTH}" | cut -d"," -f2- | rev | cut -d"." -f2- | rev)



echo -e "\nx:\n${X}"
echo -e "\ny:\n${Y}"

