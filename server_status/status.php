<?php
$argv = $_SERVER['argv'];
$ip = htmlentities(stripslashes($argv[1]));
$port = htmlentities(stripslashes($argv[2]));
#=======================================================================
# functions
function check($ip, $port) {
	$check = @fsockopen($ip, $port, $errno, $errstr, 2);
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

