#!/usr/bin/env sh
cc main.c `pkg-config --cflags raylib` `pkg-config --libs raylib` -o main