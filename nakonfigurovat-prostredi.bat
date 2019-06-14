@echo off

set directorypath=%~dp0%
set directorypath=%directorypath:\=\\%

set filepath=%directorypath%ssipf-chytry-dum.code-workspace

if not exist %filepath% (
    (
        echo {
        echo    "folders": [
        echo        {
        echo            "path": "."
        echo        }
        echo    ],
        echo    "settings": {
        echo        "arduino.path": "%directorypath%arduino",
        echo        "arduino.defaultBaudRate": 9600
        echo    }
        echo }
    ) > %filepath%
)

set filepath=%directorypath%.vscode\\arduino.json

if not exist %filepath% (
    (
        echo {
        echo    "programmer": "AVRISP mkII"
        echo }
    ) > %filepath%
)
