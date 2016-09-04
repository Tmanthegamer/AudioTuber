=========================================================================
LEGEND:
    TASK    -   Generalized title of task list
    [~]     -   Priority level unknown
    [H]     -   Priority level high
    [M]     -   Priority level medium
    [L]     -   Priority level low
    FIN     -   Finished task
=========================================================================

#########################################################################
SERVER
    
    SCRIPT
        -[L] if bestaudio fails, attempt to do post-processing instead
        -[H] Give newly downloaded files to the C++ Server
    
    YOUTUBER
        -[M] programmatically find script/storage/server location
        -FIN calls the music.sh bash script
        -FIN gets new file info (script modded to dump json file)
        -FIN loads the newly created file
        -FIN lists all available songs(files)
        -[M] use the script to "refresh" the song list with the new 
             songs downloaded (dump all songs, reload it)
        -FIN use regular expressions to get song info
        
    SONG
        -FIN Songs grabbed from filesystem using Boost Library
        -FIN File exts / filename / filepath
        -FIN Single queued song gets converted into a ready song.
    
    SERVICE
	-[L] Refine the clean up resources
	-[H] Acquire packets from Youtuber (check if packets are good)
	-[H] Send packets to a client
	-[M] Design a struct for a client's information and use maps
	-[L] Remove excess code that was from previous project
        -[M] create authentication method to connect to ONLY one device
        -FIN TCP will be used to transmit the data
	-[L] Create wrapper functions that will handle sending the different data types
		1) Preparing to send the data (data init)		
		2) Sending a header (song info, how many packets, etc.)
		3) Sending all the packets (actual data sending)
		4) Sending the final packet (data verification)
		5) Clean up after sending the data.
        
    PLAYLIST
        -[L] get all of the songs and send list to client 
        -[L] remove extensions when sending list

=========================================================================
CLIENT
    
    CONNECTION
        -[L] after streaming method researched, connect to active server
        -[L] login screen authentication
        -[L] receive list of songs
        -[L] receive song stream data completely
    AUDIO
        -[L] play m4a song in an activity
        -[L] play song in background
        -[L] play m4a song received by server
    VISUAL
        -[L] dummy home page "logged in"
        -[L] dummy login screen
        -[L] dummy currently playing screen
        -[L] dummy music playlist screen   
    PLAYLISTS
        -[L] receive playlist string and parse it into a stringlist
        -[L] store it in activity
        
#########################################################################
