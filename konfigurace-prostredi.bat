@echo off

set directorypath=%~dp0%
set directorypath=%directorypath:\=\\%

set arduinopath=%directorypath%arduino
set filepath=%directorypath%ssipf-arduino-dum.code-workspace

if not exist %filepath% (
    (
        echo {
        echo    "folders": [
        echo        {
        echo            "path": "."
        echo        }
        echo    ],
        echo    "settings": {
        echo        "arduino.path": "%arduinopath%"
        echo    }
        echo }
    ) > %filepath%
)
