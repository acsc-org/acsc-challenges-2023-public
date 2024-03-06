<?php 
	$params = session_get_cookie_params();
	$params['httponly'] = true;
	$params['samesite'] = 'Lax';
	session_set_cookie_params($params);
	session_start();
?>
<!doctype html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
		<meta name="description" content="">
		<meta name="author" content="Kuki Godam">
		<script src="https://code.jquery.com/jquery-3.4.1.slim.js" integrity="sha256-BTlTdQO9/fascB1drekrDVkaKd9PkwBymMlHOiG+qLI=" crossorigin="anonymous"></script>
		<title>Register</title>

		<!-- Bootstrap core CSS -->
		<link href="./bootstrap.min.css" rel="stylesheet">

		<!-- Custom styles for this template -->
		<link href="./style.css" rel="stylesheet">
	</head>
	<body class="text-center">
		<div class="col-xl-3">
		<h2>Register</h2>
		<br>
		<form class="form-signin" action="register" method="GET">
		<?php

		if(isset($_GET['username']) && isset($_GET['password'])){
			$username = trim($_GET['username']);
			if(strlen($username) < 4){
				echo "<h5 class='text-danger'>Username must be at least 4 characters long!</h5>";
			}else{
				$conn = new mysqli("localhost","admin","admin","dashboard_db");
				if ($conn->connect_error) {
						die("Connection failed: " . $conn->connect_error);
				}
				$sql = "INSERT INTO users (username, password, role) VALUES (?,?,'user')";
				$stmt = $conn->prepare($sql);
				$stmt->bind_param("ss", $username,$_GET['password']);
				$result = $stmt->execute();

				if($result){
					echo "<h5 class='text-success'>Registered successfully!</h5>";
				}else{
					echo "<h5 class='text-danger'>Username already exist!</h5>";
				}
			}
		}
		?>
		<label for="username" class="sr-only">Username</label>
		<input type="username" name="username" class="form-control" placeholder="Username" required="" autofocus="">
		<label for="password" class="sr-only">Password</label>
		<input type="password" name="password" class="form-control" placeholder="Password" required="">
		
		<button class="btn btn-lg btn-primary btn-block mb-2" type="submit">Register</button>
		<a href="login">Click here to Login</a>
		<p class="mt-5 mb-3 text-muted">© Kuki.Godam, 2022</p>
		</form>
		
		</div>
</body>
</html>
