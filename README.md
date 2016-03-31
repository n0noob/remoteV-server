# remoteV-server
Server that runs in the background in linux machine, allows users to connect and play media files. 

Server requires mpv to be installed in the system. This server communicates with clients using some well defined set of commands sent through the network. It works as the mediator which reads the commands comming from some particular socket and runs the respective JSON command to communicate to mpv or using some other methods of IPC for other processes.
