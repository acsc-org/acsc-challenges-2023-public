<?php
	$params = session_get_cookie_params();
	$params['httponly'] = true;
	$params['samesite'] = 'Lax';
	session_set_cookie_params($params);
	session_start();
	unset($_SESSION['user']);
	echo '<meta http-equiv="refresh" content="0;url=login">';
?>