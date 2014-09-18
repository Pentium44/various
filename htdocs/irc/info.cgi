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

echo "<h2>Information - Server</h2>"

if echo exit | telnet irc.welnetirc.ml 6677 | grep Connected > /dev/null 2>&1
then
	echo "Server status: <b style='color: #00ff00;'>Online</b><br>"
else
	echo "Server status: <b style='color: #ff0000;'>Offline</b><br>"
fi

echo "Users: <b>"
cat tmp.txt | grep 251 | sed 's/\:irc\.welnetirc\.ml\ 251\ Pento\ \://g'
echo "</b><br>"
echo "IRCd: <b>"
cat tmp.txt | grep 002 | sed 's/\:irc\.welnetirc\.ml\ 002\ Pento\ \:Your\ host\ is\ u1\.linuxshell\.picrofo\.com\,\ running\ version\ //g'
echo "<b><br><br>"

echo "
<h2>Information - Services</h2>
<b> Registering a nickname on WelNet</b>
<ul>
	<li>Connect to the server with an IRC client.</li>
	<li>After connected, message the service bot (Kiriashi). Example: /msg Kiriashi @register &lt;password here&gt;</li>
	<li>Then you have the ability to login, this will set your cloak: /msg Kiriashi @login &lt;password here&gt;</li>
</ul>
If someone is using your nickname, don't worry, you can get it back. Use: /msg Kiriashi @claim &lt;username here&gt; &lt;password here&gt;<br><br>
"

cat styles/footer.txt

rm tmp.txt
