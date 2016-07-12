#!/bin/sh

URL=${1}
TUBER="/home/tyler/AudioTuber/AudioTuberServer"
STORAGE="storage/%(title)s.%(ext)s"

if [ $# -ne 1 ]; then
    #chmod +x ${RIX_FILE}.sh
    echo $0: usage: music.sh pants
    exit 1
fi

#youtube-dl -q -f 140 -o ${TUBER}/${STORAGE} ${URL}

youtube-dl -f 'bestaudio[ext=m4a]' --write-info-json -o ${TUBER}/${STORAGE} ${URL} 

sleep 10
echo "File located at:[${TUBER}/${STORAGE}]"

