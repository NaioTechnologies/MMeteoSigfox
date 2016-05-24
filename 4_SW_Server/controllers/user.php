<?

require_once($config['models_folder']."user.php");

$head['css'] = "index.css;user.css";

if(isset($controller->splitted_url[1])) {
	switch ($controller->splitted_url[1]) {
		case 'login':
			$head['title'] = "Connexion";
			if ($user->rank == 0) {
				if (isset($_POST['submit'])) {
					// PROCESS DATA FROM FORM
					$user = new User();
					$user->login($_POST['login'], $_POST['password']);

					if($user->getId() != NULL) {
						// SUCESSFULL LOGIN
						$_SESSION['userid'] = $user->getId();
						header('Location: '.$config['rel_root_folder']);
					}
					else {
						header('Location: '.$config['rel_root_folder'].'user/login?error=1');
					}
				}
				include ($config['views_folder']."user.login.html");
			} else {
				header('Location: '.$config['rel_root_folder']);
			}
			break;
		case 'logout':
			session_destroy();
			header('Location: '.$_SERVER['HTTP_REFERER']);
			break;
		case 'signin':
			$head['js'] = "captcha.js";
			$head['title'] = "Inscription";
			if ($user->rank == 0) {
				if (isset($_POST['submit'])) {
					// PROCESS DATA FROM FORM
					$user = new User();
					$user->password = password_hash($_POST['password'], PASSWORD_BCRYPT);
					$user->username = $_POST['login'];
					$user->mail = strtolower($_POST['mail']);
					$user->rank = 400;
					$user->stepalert = 0;

					if($_POST['captcha'] == -2) {
						if($user->availableName()) {
							if($user->availableMail()) {
								if($_POST['password'] != "" AND $user->username != "" AND $user->mail != "") {
									$user->create();
									header('Location: '.$config['rel_root_folder'].'user/login?status=created');
								}
								else {
									header('Location: '.$config['rel_root_folder'].'user/signin?error=empty');
								}
							}
							else {
								header('Location: '.$config['rel_root_folder'].'user/signin?error=mail');
							}
						}
						else {
							header('Location: '.$config['rel_root_folder'].'user/signin?error=name');
						}
					}
					else {
						header('Location: '.$config['rel_root_folder'].'user/signin?error=captcha');
					}
				}
				include ($config['views_folder']."user.signin.html");
			} else {
				header('Location: '.$config['rel_root_folder']);
			}
			break;
		case 'password_lost':
			$head['title'] = "Récupération de mot de passe";
			if ($user->rank == 0) {
				if (isset($_POST['submit'])) {
					// PROCESS DATA FROM FORM
					$user = new User();
					$user->mail = strtolower($_POST['mail']);

					if($user->availableMail()) {
						header('Location: '.$config['rel_root_folder'].'user/password_lost?error=1');
					}
					else {
						$user->sendPassword();
						header('Location: '.$config['rel_root_folder'].'user/login?status=password_sent');
					}
				}
				include ($config['views_folder']."user.password_lost.html");
			} else {
				header('Location: '.$config['rel_root_folder']);
			}
			break;
		case 'account':
			if ($user->rank >= 200) {
				$head['title'] = "Compte utilisateur";

				// If we are editing the profile
				if (isset($_POST['submit'])) {
					$receivedUser = new User();
					$receivedUser->username = $_POST['username'];
					if($receivedUser->username != $user->username && $receivedUser->availableName())
						$user->username = $receivedUser->username;
					else if($receivedUser->username != $user->username)
						$nameError=1;
					$receivedUser->mail = strtolower($_POST['mail']);
					if($receivedUser->mail != $user->mail && $receivedUser->availableMail())
						$user->mail = $receivedUser->mail;
					else if ($receivedUser->mail != $user->mail)
						$mailError=1;
					if($_POST['password']!='')
						$user->password=password_hash($_POST['password'], PASSWORD_BCRYPT);
					$user->phone = $_POST['phone'];

					$user->update();

					$updated = 1;
				}
				include ($config['views_folder']."user.account.html");
			}
			else {
				header('Location: '.$config['rel_root_folder']);
			}
			break;
		default:
			$notfound = 1;
			break;
	}
}
else {
	$notfound = 1;
}

?>