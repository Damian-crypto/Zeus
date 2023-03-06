#!/bin/bash

# Black        0;30     Dark Gray     1;30
# Red          0;31     Light Red     1;31
# Green        0;32     Light Green   1;32
# Brown/Orange 0;33     Yellow        1;33
# Blue         0;34     Light Blue    1;34
# Purple       0;35     Light Purple  1;35
# Cyan         0;36     Light Cyan    1;36
# Light Gray   0;37     White         1;37

RED='\033[0;31m';
GREEN='\033[0;32m';
NONE='\033[0;0m';

MYDIR=$(pwd)

function configure_game() {
    cmake -DBUILD_SIMPLEGAME=ON -DMYPROJECT_DIR="${MYDIR}" -S . -B build/
}

function configure_leveleditor() {
    cmake -DBUILD_SANDBOX=ON -DMYPROJECT_DIR="${MYDIR}" -S . -B build/
}

if [[ $2 = "" ]] then
    if [[ $1 = "game" ]] then
        printf "${GREEN}[BUILD SYSTEM] Configuring game....${NONE}\n";
        configure_game;
        printf "${GREEN}[BUILD SYSTEM] Copying assets...${NONE}\n";
        cp -ru SimpleGame/assets build/SimpleGame && sh build.sh && cd build/SimpleGame && ./SimpleGame;
    elif [[ $1 = "sandbox" ]] then
        printf "${GREEN}[BUILD SYSTEM] Configuring sandbox....${NONE}\n";
        configure_leveleditor;
        printf "${GREEN}[BUILD SYSTEM] Copying assets...${NONE}\n";
        cp -ru Sandbox/assets build/Sandbox && sh build.sh && cd build/Sandbox && ./Sandbox;
    elif [[ $1 = "clean" ]] then
        printf "${RED}Clearing up dynamic builds${NONE}\n";
        sh clean.sh;
    fi
else
    if [[ $1 = "sandbox" ]] then
        if [[ $2 = "debug" ]] then
            cd build/Sandbox && gdb Sandbox;
        elif [[ $2 = "debug-tui" ]] then
            cd build/Sandbox && gdb -tui Sandbox;
        fi
    elif [[ $1 = "game" ]] then
        if [[ $2 = "debug" ]] then
            cd build/SimpleGame && gdb SimpleGame;
        elif [[ $2 = "debug-tui" ]] then
            cd build/SimpleGame && gdb-tui SimpleGame;
        fi
    fi
fi
