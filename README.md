# Qt_fileShare
## Description:
Simple gui file transfer program written in [qt5-c++](https://www.qt.io/).

## Known bugs:
* Not that great of a user experience.
* Linux -> Linux file transfer works perfectly, but some bytes cause problems in Windows. Most probably because native c++ methods were used for file reading and writing instead of qt methods, which would've handled platform dependency issues.