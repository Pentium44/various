#!/bin/sh
# Clest HTML generation script

# Clest 0.0.1a - This program is in its beta stages
# (C) Chris Dorman, 2014 - Some rights reserved
# GPLv3

PATH="$PATH:`pwd`"

. `pwd`/settings.sh

doupdate() {
	
	# Check if header has content
	if [ ! -s "data/header" ]; then
		echo "ERROR, Missing header code!" >> $CLESTLOG
		exit 1
	fi
	
	# Check if footer has content
	if [ ! -s "data/footer" ]; then
		echo "ERROR, Missing header code!" >> $CLESTLOG
		exit 1
	fi
		
	while read line           
	do           
		# Update html for current file
		cat data/header > $WEBDIR/$line.html
		cat data/backup/$line >> $WEBDIR/$line.html
		cat data/footer >> $WEBDIR/$line.html
	done < data/filelist
	
	echo "Success, update complete!" >> $CLESTLOG      
}

update() {
	# Needed variables
	OLDHEADER=$(cat data/header-OLD)
	HEADER=$(cat data/header)
	OLDFOOTER=$(cat data/footer-OLD)
	FOOTER=$(cat data/footer)
	
	if [ "$HEADER" != "$OLDHEADER" ]; then
		doupdate
	fi
	
	if [ "$FOOTER" != "$OLDFOOTER" ]; then
		doupdate
	fi
	
}

genpage() {
	
	PAGETITLE=$(cat data/ptitle)
	PAGECONTENT="data/p"
	
	if [ -s "$PAGECONTENT" ]; then
		# Check if header has content
		if [ ! -s "data/header" ]; then
			echo "ERROR, Missing header code!" >> $CLESTLOG
			exit 1
		fi
		
		# Check if footer has content
		if [ ! -s "data/footer" ]; then
			echo "ERROR, Missing header code!" >> $CLESTLOG
			exit 1
		fi
		
		# If everything checks out, write file
		# Check if page exists then write it
		if [ ! -f data/backup/$PAGETITLE ]; then
		
			cat data/header >> $WEBDIR/$PAGETITLE.html
			cat $PAGECONTENT >> $WEBDIR/$PAGETITLE.html
			cat data/footer >> $WEBDIR/$PAGETITLE.html
		
			echo "$PAGETITLE" >> data/filelist
			cat $PAGECONTENT >> data/backup/$PAGETITLE
			
			cp /dev/null data/p
			cp /dev/null data/ptitle
		
		else
			echo "Error, page exists" >> $CLESTLOG
		fi
		
		echo "Success, page created successfully" >> $CLESTLOG
	fi
}

genblogpost() {
	# File in which is checked for generation.
	BLOGIN="data/b"
	BLOGTITLEIN=$(cat data/btitle)
	
	# List generator for blog posts.
	BLOGLISTIN="data/blist"
	DATEANDTIME=$(date +"%H%M%S%m%d%Y")
	
	if [ -s "$BLOGIN" ]; then
		# Check if header has content
		if [ ! -s "data/header" ]; then
			echo "ERROR, Missing header code!" >> $CLESTLOG
			exit 1
		fi
		
		# Check if footer has content
		if [ ! -s "data/footer" ]; then
			echo "ERROR, Missing header code!" >> $CLESTLOG
			exit 1
		fi
		
		# If everything checks out, write file
		# echo header footer and blog content to end of new file
		cat data/header >> $WEBDIR/blog-$DATEANDTIME.html
		cat $BLOGIN >> $WEBDIR/blog-$DATEANDTIME.html
		cat data/footer >> $WEBDIR/blog-$DATEANDTIME.html
		
		echo "<a href='blog-$DATEANDTIME.html'>$BLOGTITLEIN</a><br>" >> $BLOGLISTIN
		
		# backup post
		cat $BLOGIN >> data/backup/blog-$DATEANDTIME
		echo "blog-$DATEANDTIME" >> data/filelist
		cat $BLOGLISTIN >> data/backup/blog
		
		# Rewrite bloglist
		cat data/header > $WEBDIR/blog.html
		cat $BLOGLISTIN >> $WEBDIR/blog.html
		cat data/footer >> $WEBDIR/blog.html
		
		cp /dev/null data/b
		cp /dev/null data/btitle
		echo "Success, blogging successful" >> $CLESTLOG
	fi
}

case $1 in
	b ) genblogpost;;
	p ) genpage;;
	update ) update;;
	* ) ;;
esac
			