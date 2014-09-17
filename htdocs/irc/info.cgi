#!/bin/bash

echo "Content-type: text/html; charset=utf-8;"
echo ""

cat styles/header.txt

echo "<h2>Server information</h2>"

if pgrep python > /dev/null
then
	echo "Server status: <b style='color: #00ff00;'>Online</b><br>"
else
	echo "Server status: <b style='color: #ff0000;'>Offline</b><br>"
fi

cat styles/footer.txt
