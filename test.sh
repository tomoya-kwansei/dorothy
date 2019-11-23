#!/usr/bin/env bash

make test

function try() {
    ./cpu $1
}

for script in $(ls script/ | sed s/.txt//g); do
    ./compiler script/$script.txt $script
done

for bin in $(ls bin/ | sed s/.bin//g); do
    try bin/$bin.bin > txt/$bin.txt
    echo $bin: $?
done

rm -rf a.out
