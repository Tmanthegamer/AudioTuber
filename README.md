# AudioTuber
Server and Client application that uses a server to convert YouTube videos into music that can be streamed onto an Android Smartphone

#Server Requirements
1) youtube_dl: use the setup script provide in AudioTuberServer directory
2) boost filesystem
3) compile with: g++ -std=c++11 *.cpp -o server.out -lboost_system -lboost_filesystem

