#!/bin/fish

set exe ./ex19

echo "--> Start: l-n-l-e-w-s-n-w-a-a-EOF"

set input "l\nn\nl\ne\nw\ns\nn\nw\na\na\na\na\na"

echo -e $input | $exe

return $status
