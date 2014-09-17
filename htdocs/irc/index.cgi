#!/bin/bash

echo "Content-type: text/html; charset=utf-8;"
echo ""

cat styles/header.txt

echo "<h2>Welcome!</h2>"
echo "WelNet is setup for people who want to chat with comfort. I have been on many of the major IRC networks, and I was often treated with disrespect when I wanted to chat about anything. The goal thats set is to show the community respect and help with what you need. I'll talk with anyone about anything from life troubles to programming."
echo "<br><br>"
echo "<h3>Connect!</h3>"
echo "
Address/port: irc.cddo.cf/6667<br>
Common channels: #welnet; #programming.
"

cat styles/footer.txt
