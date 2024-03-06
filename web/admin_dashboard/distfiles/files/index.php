<?php
	$params = session_get_cookie_params();
	$params['httponly'] = true;
	$params['samesite'] = 'Lax';
	session_set_cookie_params($params);
	session_start();

	if (!isset($_SESSION["user"])){
		echo '<meta http-equiv="refresh" content="0;url=login">';
		die();
	}
?>
<html>

<head>
	<title>Dashboard</title>
	<script src="https://code.jquery.com/jquery-3.4.1.slim.js" integrity="sha256-BTlTdQO9/fascB1drekrDVkaKd9PkwBymMlHOiG+qLI=" crossorigin="anonymous"></script>
	<!-- Bootstrap core CSS -->
	<link href="./bootstrap.min.css" rel="stylesheet">
</head>

<body>
<nav class="navbar navbar-expand-lg navbar-dark bg-dark static-top">
	<div class="container">
		<a class="navbar-brand" href="index">Dashboard</a>
		<button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
			<span class="navbar-toggler-icon"></span>
		</button>
		<div class="collapse navbar-collapse" id="navbarResponsive">
			<ul class="navbar-nav ml-auto">
				<li class="nav-item active">
					<a class="nav-link" href="index">Home</a>
				</li>
				<!-- Admin only
				<li class="nav-item">
					<a class="nav-link" href="addadmin">Add Admin User</a>
				</li>
				-->
				<li class="nav-item">
					<a class="nav-link" href="report">Report</a>
				</li>
				<li class="nav-item">
					<a class="nav-link" href="logout">Logout</a>
				</li>
			</ul>
		</div>
	</div>
</nav>

<main role="main" class="container">
	<h1 class="mt-5">Welcome <?=htmlentities($_SESSION["user"]["username"]);?></h1>
	<?=($_SESSION["user"]["role"] === "admin") ? '<p class="lead">ACSC{REDACTED}</p>' : "";?>
	

</main>

</body>

</html>
