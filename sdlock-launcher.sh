#!/bin/bash

PASSWORD=test

SDLOCK_ARGS="$PASSWORD"

if [ -e ./sdlock ]
then
	./sdlock $SDLOCK_ARGS
else
	~/.sdlock $SDLOCK_ARGS
fi

# EOF

