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

set filepath=%directorypath%.vscode\\c_cpp_properties.json

if not exist %filepath% (
    (
        echo {
        echo    "configurations": [
        echo        {
        echo            "name": "Win32",
        echo            "defines": [
        echo                "USBCON"
        echo            ],
        echo            "includePath": [
        echo                "%directorypath%arduino\\**"
        echo            ]
        echo        }
        echo    ],
        echo    "version": 4
        echo }
    ) > %filepath%
)
