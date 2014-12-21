<?php
# Simple script to check if mt / voxelands / freeminer server is online.
# check_if_minetestserver_up was used from the protocol.txt file.

$argv = $_SERVER['argv'];
$ip = htmlentities(stripslashes($argv[1]));
$port = htmlentities(stripslashes($argv[2]));
#=======================================================================
# functions
function check_if_minetestserver_up($host, $port)
{
	$socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
	$timeout = array("sec" => 1, "usec" => 0);
	socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, $timeout);
	$buf = "\x4f\x45\x74\x03\x00\x00\x00\x03\xff\xdc\x01";
	socket_sendto($socket, $buf, strlen($buf), 0, $host, $port);
	$buf = socket_read($socket, 1000);
	if($buf != "")
	{
		# We got a reply! read the peer id from it.
		$peer_id = substr($buf, 9, 2);
		
		# Disconnect
		$buf = "\x4f\x45\x74\x03".$peer_id."\x00\x00\x03";
		socket_sendto($socket, $buf, strlen($buf), 0, $host, $port);
		socket_close($socket);
		
		# WOOO server's online :3
		return 1;
	}
	# ah damn, server's offline :p
	return 0;
}

function check($ip, $port) {
	$check = check_if_minetestserver_up(gethostbyname($ip),$port);
	if (!$check)
	{
		gentext(0);
	}
	else
	{
		gentext(1);
	}
}

function gentext($stat) {
	if ($stat == 1)
	{
		$status = "<div style='color:#00ff00;'>Online</div>";
	}
	else
	{
		$status = "<div style='color:#ff0000;'>Offline</div>";
	}
	echo $status;
}

/*
function genimg($status) {
        $img = imagecreate(56, 16); # was 150
        $background = imagecolorallocate($img,0,0,0);
        $fgname = imagecolorallocate($img, 255, 255, 255);
        if ($status == 1)
		{
			$fgstatus = imagecolorallocate($img, 0, 255, 0);
			$status = "Online";
		}
		else
		{
			$fgstatus = imagecolorallocate($img, 255, 0, 0);
			$status = "Offline";
		}
		imagestring($img, 4, 0,0, $status, $fgstatus);
        header("Content-type: image/png");
        imagepng($img);
        imagedestroy($img);
}
*/

#=======================================================================
if ($ip and $port) {
	check($ip, $port);
}
else
{
	die();
}
?>

