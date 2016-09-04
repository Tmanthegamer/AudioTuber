#!/bin/sh
#Initial set up file that 
#TUBEPATH=$(pwd)
#export  TUBER=${TUBEPATH}
#sudo ln -s "$TUBER/music" /usr/bin/

wget https://yt-dl.org/latest/youtube-dl -O /usr/local/bin/youtube-dl
chmod a+x /usr/local/bin/youtube-dl
hash -r
