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
		<title>Login</title>

		<!-- Bootstrap core CSS -->
		<link href="./bootstrap.min.css" rel="stylesheet">

		<!-- Custom styles for this template -->
		<link href="./style.css" rel="stylesheet">
	</head>
	<body class="text-center">
		<div class="col-xl-3">
			<h2>Login</h2>
			<br>
			<form class="form-signin" action="login" method="GET">
			<?php 
			if(isset($_GET['username']) && isset($_GET['password'])){
				$conn = new mysqli("localhost","admin","admin","dashboard_db");	
			if ($conn->connect_error) {
				die("Connection failed: " . $conn->connect_error);
			}
			$sql = "SELECT * FROM users WHERE username = ? and password = ?";
			$stmt = $conn->prepare($sql);
			$stmt->bind_param("ss", $_GET['username'],$_GET['password']);
			$stmt->execute();
			$result = $stmt->get_result();
			$row = $result->fetch_assoc();
			if($row){
				$_SESSION["user"] = $row;
				echo '<meta http-equiv="refresh" content="0;url=index">';
			}else{
				echo "<h5 class='text-danger'>Wrong username or password!</h5>";
			}
				}
			?>
			<label for="username" class="sr-only">Username</label>
			<input type="username" name="username" class="form-control" placeholder="Username" required="" autofocus="">
			<label for="password" class="sr-only">Password</label>
			<input type="password" name="password" class="form-control" placeholder="Password" required="">
			<button class="btn btn-lg btn-primary btn-block mb-2" type="submit">Login</button>
			<a href="register">Click here to Register</a>
			<p class="mt-5 mb-3 text-muted">© Kuki.Godam, 2022</p>
			</form>
		</div>
</body>
</html>
