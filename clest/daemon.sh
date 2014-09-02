#!/bin/sh
# Clest 0.0.1a - This program is in its beta stages
# (C) Chris Dorman, 2014 - Some rights reserved
# GPLv3

PATH="$PATH:`pwd`"

cp data/header data/header-OLD
cp data/footer data/footer-OLD

clestLoop() {
	while true; do
		# Run blog and page updates
		gh.sh b # generate blog
		gh.sh p # generate other pages
		gh.sh update # dynamically rewrite html when changed
		sleep 10 # sleep for ten seconds and start over
	done
}

case $1 in
	start ) clestLoop;;
	* ) echo "Error, usage: ** {start}";;
esac