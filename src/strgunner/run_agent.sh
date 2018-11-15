#!/bin/sh

if [ "$#" -ne 1 ]; then
	echo "Usage: ./run_agent.sh ROM_PATH" >&2
	exit 1
fi

make agent

LD_LIBRARY_PATH="." ./sg_bot $1
