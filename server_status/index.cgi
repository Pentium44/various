#!/bin/sh
# Check server status

# Get server status:
PHPEXEC="php-cgi -q"

#mainsuvival=`$PHPEXEC status.php servers.voxelands.com 30000`
maincreative=`$PHPEXEC status.php servers.voxelands.com 30002`
mainprivate=`$PHPEXEC status.php servers.voxelands.com 30003`
mainadventure=`$PHPEXEC status.php servers.voxelands.com 30001`
menchesurvival=`$PHPEXEC status.php menche.us 30000`
pentiumserver=`$PHPEXEC status.php localhost 30000`
scsurvival=`$PHPEXEC status.php stormchaser3000.no-ip.org 30001`
splashserver=`$PHPEXEC status.php side2side.raspberryip.com 30000`

echo "Content-Type: text/html"
echo ""

echo "<!DOCTYPE html>
<html>
<head>
	<title>- Voxelands Server List -</title>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8;">
	<style type='text/css'>
	body {
		color: #000000;
	}
	
	table {
		border: solid 1px #222222;
		width: 100%;
	}
	
	td {
		border: solid 1px #454545;
		width: 33%;
	}
	</style>
</head>
<table>
<tr>
	<td>Server name</td>
	<td>Address:Port</td>
	<td>Status</td>
</tr>
<tr>
	<td>Voxelands Creative Server</td>
	<td>servers.voxelands.com:30002</td>
	<td>$maincreative</td>
</tr>
<tr>
	<td>Voxelands Private Server</td>
	<td>servers.voxelands.com:30003</td>
	<td>$mainprivate</td>
</tr>
<tr>
	<td>Voxelands Adventure Server</td>
	<td>servers.voxelands.com:30001</td>
	<td>$mainadventure</td>
</tr>
<tr>
	<td>Menche's Survival Server</td>
	<td>menche.us:30000</td>
	<td>$menchesurvival</td>
</tr>
<tr>
	<td>Pentium44's Survival Server</td>
	<td>kiriashi.cddo.cf:30000</td>
	<td>$pentiumserver</td>
</tr>
<tr>
	<td>Stormchaser3000's Voxelands Survival Server</td>
	<td>stormchaser3000.no-ip.org:30001</td>
	<td>$scsurvival</td>
</tr>
<tr>
	<td>Splash's Raspberry Pi Server</td>
	<td>side2side.raspberryip.com:30000</td>
	<td>$splashserver</td>
</tr>
</table>
</html>"
