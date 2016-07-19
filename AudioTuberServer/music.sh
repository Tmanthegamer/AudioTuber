#!/bin/bash

URL=${1}
TUBER="/home/tyler/AudioTuber/AudioTuberServer"
TEMP="${TUBER}/temp"
STORAGE="storage/%(title)s.%(ext)s"

function DownloadSong() {
    mkdir ${TEMP}

#youtube-dl -q -f 140 -o ${TUBER}/${STORAGE} ${URL} #original test

#youtube-dl -f 'bestaudio[ext=m4a]' --verbose -o ${TUBER}/${STORAGE} ${URL} #without json

    youtube-dl -f 'bestaudio[ext=m4a]' --write-info-json -o "${TEMP}/%(title)s.%(ext)s" "${URL}" 
    
    GOING=0
    ESCAPE=1
    while test $GOING -le $ESCAPE; do
        FILES="${TEMP}/*.*"
        echo "going:${GOING}"
        for f in $FILES;
        do
            NAME=$(basename "${f}")
            echo "Comparing ${NAME}"
            if [[ $NAME == *".info.json" ]]
            then
                GOING=$((GOING + 2))
                cat "${TEMP}/${NAME}" | jq '.fulltitle' >>"${TEMP}/newsongs.txt"
                break
            fi
        done
    done
    
}

function RemoveTempFiles() {
    FILES="${TEMP}/*.*"
    for f in $FILES
    do
        NAME=$(basename "${f}")
        echo "Working with :::${f}:::"
        mv "${f}" "${TUBER}/storage/${NAME}"
    done
    
    rm -r ${TEMP}
}

if [ $# -ne 1 ] 
then
    RemoveTempFiles
else
    DownloadSong
fi


