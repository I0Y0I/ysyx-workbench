#!/bin/fish

set ex build/ex17
set db "build/test.db"
set cmd $ex $db
set l $cmd l

if test -e $db
    rm $db
end

set -x fish_trace 1
echo "--> Test create database."
$cmd c 100 100
$l

echo "--> Test create row, create id=1,2,10"
$cmd s 1 test1 1@test
$l
$cmd s 2 test2 2@test
$l
$cmd s 10 test10 10@test
$l

echo "--> Create id 100, should fail!"
$cmd s 100 test100 100@test
$l

echo "--> Test delete, delete id=2"
$cmd d 2
$l

echo "--> Test get, get id=2,10"
$cmd g 2
$cmd g 10

echo "--> Test find, find by name"
$cmd f test1
$cmd f test2

echo "--> Test find, find by email"
$cmd f 1@test
$cmd f 2@test
