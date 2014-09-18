#!/bin/bash

echo "Content-type: text/html; charset=utf-8;"
echo ""

getusers() {
	sleep 1
	echo "NICK Pento"
	echo "USER u u u u"
	sleep 1
	echo "LUSERS"
	echo "QUIT"
}

getusers | telnet irc.welnetirc.ml 6677 > tmp.txt


cat styles/header.txt

echo "<h2>Server information</h2>"

if echo exit | telnet irc.welnetirc.ml 6677 | grep Connected > /dev/null 2>&1
then
	echo "Server status: <b style='color: #00ff00;'>Online</b><br>"
else
	echo "Server status: <b style='color: #ff0000;'>Offline</b><br>"
fi

echo "Users: <b>"
cat tmp.txt | grep 251 | sed 's/\:u1\.linuxshell\.picrofo\.com\ 251\ Pento\ \://g'
echo "</b><br>"
echo "IRCd: <b>"
cat tmp.txt | grep 002 | sed 's/\:u1\.linuxshell\.picrofo\.com\ 002\ Pento\ \:Your\ host\ is\ u1\.linuxshell\.picrofo\.com\,\ running\ version\ //g'
echo "<b>"
cat styles/footer.txt

rm tmp.txt
