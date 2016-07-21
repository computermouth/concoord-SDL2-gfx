#!/bin/bash

for FILE in $@; do
	#name
	NAME="$(xmlstarlet sel -N x="http://www.w3.org/2000/svg" -t -m //x:path -v @id ${FILE})"

	#path
	BOTH=$(xmlstarlet sel -N x="http://www.w3.org/2000/svg" -t -m //x:path -v @d ${1} | sed -s 's/M.*C //g' | sed -s 's/              /\n/g' | sed -s 's/ Z//g' | awk '{print $1}')

	#x
	X=$(echo "${BOTH}" | rev | cut -d"," -f2- | cut -d"." -f2- | rev)
	Y=$(echo "${BOTH}" | cut -d"," -f2- | rev | cut -d"." -f2- | rev)

	X=$(echo "${X}" | tr '\n' ', ')
	Y=$(echo "${Y}" | tr '\n' ', ')

	X=${X::-1}
	Y=${Y::-1}

	X=$(echo "${X}" | sed -s 's/,/, /g')
	Y=$(echo "${Y}" | sed -s 's/,/, /g')

	echo -e "\nx:\n${X}"
	echo -e "\ny:\n${Y}"
done
