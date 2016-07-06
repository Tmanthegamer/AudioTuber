#########################################################################
SERVER
    
    SCRIPT
        -[L] if bestaudio fails, attempt to do post-processing instead
    SERVICE
        -[H] create the platform that will listen in for connections using sockets
        -[H] determine streaming method for android, TCP/IP ??? 
        -[M] create authentication method to connect to ONLY one device
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
