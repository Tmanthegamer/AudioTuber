# AudioTuber
Server and Client application that uses a server to convert YouTube videos into music that can be streamed onto an Android Smartphone

#Special Note: Windows is not support but it will be added in the future.

#Server Requirements
1) youtube_dl
2) boost
3) internet connection
4) Ubuntu (Every other platform is untested)

#Server Build Instructions
Use the setup script provided in Server directory to install all of the 
required components. For now, the main OS being used is Ubuntu but there are plans to further the development into Windows as well using the boost C++ library.

1) (Ubuntu only)Install all of the boost libraries with:
	$ sudo apt-get install libboost-all-dev
2) Download and install youtube-dl
	$ wget https://yt-dl.org/latest/youtube-dl -O /usr/local/bin/youtube-dl
	$ chmod a+x /usr/local/bin/youtube-dl
	$ hash -r
3) Create an AUDIOTUBER environment variable and link the music.sh file to the command-line
	$ export AUDIOTUBER=/path/to/AudioTuber/Server
	$ sudo ln -s $AUDIOTUBER/music.sh /usr/bin/
4) Install the jq utility for parsing json files.
5) Compile the Server 
	$ g++ -std=c++11 *.cpp -o server.out -lboost_system -lboost_filesystem

#Client Dev Build Instructions
1) Ensure that oracle java v8 (v9 may work, but it's untested)
	$ sudo apt-add-repository ppa:webupd8team/java 
	$ sudo apt-get update
	$ sudo apt-get install oracle-java8-installer
	$ export JAVA_HOME=/usr/lib/jvm/java-8-oracle
2) Download and install Android Studio (Google is your friend)
3) The project is inside: "/path/to/Audiotuber/Client"

#Generalized To do list:
-Programmatically use the export statement to get the path
	>export path usuage
	>within compile time
-More specialized Ubuntu installation script.
-Implement the Client design
-Allow the server to operate as a Windows OR Linux program.
