<?php
	ini_set('display_errors', 1);
	ini_set('display_startup_errors', 1);
	error_reporting(E_ALL);
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
	<h1 class="mt-5">Report to admin</h1>
	<p class="lead">Submit the URL here, admin will review the link shortly</p>
	<div class="col-xl-6 mt-5">
		<form class="form-add-note" action="report" method="POST">
		<label for="link" class="sr-only">Link</label>
		<input type="text" name="url" class="form-control" placeholder="http://example.com" required="true" autofocus="true">
		
		<button class="btn btn-primary btn-block mt-2" type="submit">Report</button>
		</form>
		<?php
			if(isset($_POST['url'])){
				if (filter_var($_POST['url'], FILTER_VALIDATE_URL) && preg_match("^http(s)?^",parse_url($_POST['url'], PHP_URL_SCHEME))) {
					$return_value = 1;
					system("node /bot/bot.js ".urlencode($_POST['url']),$return_value);
					if($return_value == 0){
						echo "<p class='text-success'>Admin will view the URL shortly!</p>";
					}else{
						echo "<p class='text-danger'>Failed to view the URL! Please try again</p>";
					}
				} else {
					echo "<p class='text-danger'>Invalid URL!</p>";
				}
			}
		?>
	</div>
</main>

</body>

</html>
