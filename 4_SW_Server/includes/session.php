<?

require_once($config['models_folder']."user.php");

ini_set("session.cookie_lifetime",60*60*24*30);
session_start();

$user = new User();
$user->rank == 0; // All users are visitors

if(isset($_SESSION['userid'])) {
	if ($user->checkID($_SESSION['userid'])) {
		$user->populate();
	}
	else {
		session_destroy();
	}
}

?>