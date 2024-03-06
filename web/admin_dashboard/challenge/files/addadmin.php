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
	$conn = new mysqli("localhost","admin","admin","dashboard_db");
	if ($conn->connect_error) {
			die("Connection failed: " . $conn->connect_error);
	}

	$sql = "SELECT * FROM secrets";
	$stmt = $conn->prepare($sql);
	$stmt->execute();
	$result = $stmt->get_result();
	$row = $result->fetch_assoc();
	if($row){
		$A = gmp_import($row['A']);
		$C = gmp_import($row['C']);
		$M = gmp_init("0xc4f3b4b3deadbeef1337c0dedeadc0dd");
	}
	if (!isset($_SESSION['X'])){
		$X = gmp_import($_SESSION["user"]["username"]);
		$_SESSION['X'] = gmp_mod(gmp_add(gmp_mul($A, $X),$C),$M);
		$_SESSION["token-expire"] = time() + 30; 
	}else{
		if(time() >= $_SESSION["token-expire"]){
			$_SESSION['X'] = gmp_mod(gmp_add(gmp_mul($A, $_SESSION['X']),$C),$M);
			$_SESSION["token-expire"] = time() + 30; 
		}
	}
?>
<html>

<head>
	<title>Add admin account</title>
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
	<div class="col-xl-6 mt-5">
		<form class="form-add-note" action="addadmin" method="POST">
		<label for="title" class="sr-only">Username</label>
		<input type="text" name="username" class="form-control" placeholder="Username" required="" autofocus="">
		<input type="password" name="password" class="form-control" placeholder="Password" required="" autofocus="">
		<input type="hidden" name="csrf-token" value="<?=gmp_strval($_SESSION['X'],16)?>">
		<button class="btn btn-primary btn-block mt-2" type="submit">Add admin user</button>
		</form>
		<?php
			if(isset($_REQUEST['username']) && isset($_REQUEST['password']) && isset($_REQUEST['csrf-token'])){
				if($_SESSION["user"]["role"] !== "admin"){
					echo "<p class='text-danger'>No permission!</p>";
				}else{
					if($_REQUEST['csrf-token'] === gmp_strval($_SESSION['X'],16)){
						$sql = "INSERT INTO users (username, password, role) VALUES (?,?,'admin')";
						$stmt = $conn->prepare($sql);
						$stmt->bind_param("ss", $_REQUEST['username'], $_REQUEST['password']);
						$result = $stmt->execute();
						echo "<p class='text-success'>Added successfully!</p>";
					}else{
						echo "<p class='text-danger'>Wrong token!</p>";
					}
				}
			}
		?>
	</div>
</main>

</body>

</html>
