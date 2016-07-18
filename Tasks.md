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
    
    YOUTUBER
        -[M] calls the music.sh bash script
        -[H] gets new file info (script modded to dump json file)
        -[H] loads the newly created file
        -[M] lists all available songs(files)
    SONG
        -[H] SongQueued and SongReady inherits from Song
        -[H] SongQueued and SongReady inherits from Song
    
    SERVICE
        -[H] create the platform that will listen in for connections using sockets
        -[M] create authentication method to connect to ONLY one device
        -[~] determine streaming method for android, TCP/IP 
        
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
