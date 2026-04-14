# phasespace
This is the project that integrates PhaseSpace motion capture with LabVIEW applications.

## Pre-operation
* Turn on all computers and machines.
* Wait for at least one minute so that the machines are all initialized.
* The whole project is located under `Documents/Github/phasespace/`.
* Please *DO NOT* change the tracker profiles because the ID numbers are all set . 

## Required modules
* PSP (PhaseSpace Project):

This is the main server for this project. Try to execute *PSP.exe* first. Please enter the following command in the command line. The default executable file is located under `x64/Release/` folder.
```
PSP.exe
```

* socket_target.vi:

This is the client for the socket and is written in LabVIEW, which can be integrated with other LabVIEW applications. The file is located under `LabVIEW/` folder and can be run at either host or target computer. Only need to double-click on the vi file and run it for execution.

## Unused LabVIEW modules but still exist in the project
* socket_client.vi (deprecated):

This is the client for the socket and is written in C++. The program can be run by the SysExec module in LabVIEW. There is an example of using the SysExec module in LabVIEW under `LabVIEW/` folder called *socket_client.vi*.

* phasespace_server.vi (deprecated):

This is the server side for the socket and also handles the communications with the PhaseSpace machine. The vi file is responsible for the execution of the server and is also located under `LabVIEW/` folder. 
