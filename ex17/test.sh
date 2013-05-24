#!/bin/bash

set -e

rm -rf ./db.dat
./ex17 db.dat c 10 20
./ex17 db.dat s 1 zed zed@zedshaw.com
./ex17 db.dat s 2 frank frank@zedshaw.com
./ex17 db.dat s 3 joe joe@zedshaw.com

./ex17 db.dat l

./ex17 db.dat d 2
./ex17 db.dat l

./ex17 db.dat g 3
