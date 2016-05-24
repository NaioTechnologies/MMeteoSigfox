<?

/**********************************************************
***********************************************************
**  
**  This class is to manage User object
**  
***********************************************************
**********************************************************/

require_once($config['third_folder']."class.uuid.php");

class User
{
	private $id = NULL;
	private $date_create = NULL;
	private $date_update = NULL;
	private $owner = NULL;
	private $version = NULL;

	public $username = NULL;
	public $password = NULL;
	public $mail = NULL;
	public $phone = NULL;
	public $rank = NULL;
	public $lastalert_rised = NULL;
	public $stepalert = NULL;


	/*****
	** Return user ID
	*****/
	public function getId() {
		return $this->id;
	}
	/*****
	** Connect to correct account using ID and stores its ID
	*****/
	public function checkID($id) {
		global $config;
		
		$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
			or die ("Could not connect to server\n");

		$query = "SELECT id FROM t_users WHERE id=$1";

		pg_prepare($con, "prepare1", $query) 
			or die ("Cannot prepare statement\n");
		$result = pg_execute($con, "prepare1", array($id))
			or die ("Cannot execute statement\n");

		pg_close($con);

		if(pg_num_rows($result) == 1) {
			$this->id = $id;
			return 1;
		}
		else {
			return 0;
		}
	}
	/*****
	** Connect to correct account using user/pass and stores its ID
	*****/
	public function login($login, $pass) {
		global $config;
		
		$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
			or die ("Could not connect to server\n");

		$query = "SELECT id, password FROM t_users WHERE username=$1";

		pg_prepare($con, "prepare1", $query) 
			or die ("Cannot prepare statement\n");
		$result = pg_execute($con, "prepare1", array($login))
			or die ("Cannot execute statement\n");

		pg_close($con); 

		if(pg_num_rows($result) == 1) {
			$user = pg_fetch_assoc($result);
			if (password_verify($pass, $user['password']))
				$this->id = $user['id'];
		}
	}
	/*****
	** Populate the object using its ID
	*****/
	public function populate() {
		global $config;
		
		if($this->id != NULL) {
			$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
				or die ("Could not connect to server\n");

			$query = "SELECT * FROM t_users WHERE id=$1";

			pg_prepare($con, "prepare1", $query) 
				or die ("Cannot prepare statement\n");
			$result = pg_execute($con, "prepare1", array($this->id))
				or die ("Cannot execute statement\n");

			pg_close($con);

			$user = pg_fetch_assoc($result);

			$this->date_create = $user['date_create'];
			$this->date_update = $user['date_update'];
			$this->owner = $user['owner'];
			$this->version = $user['version'];
			$this->username = $user['username'];
			$this->password = $user['password'];
			$this->mail = $user['mail'];
			$this->phone = $user['phone'];
			$this->rank = $user['rank'];
			$this->lastalert_rised = $user['lastalert_rised'];
			$this->stepalert = $user['stepalert'];
		}
		else {
			die("Cannot populate an User without ID");
		}
	}
	/*****
	** Checks if the user's name is available or not
	*****/
	public function availableName() {
		global $config;
		
		$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
			or die ("Could not connect to server\n");

		$query = "SELECT id FROM t_users WHERE lower(username)=$1";

		pg_prepare($con, "prepare1", $query) 
			or die ("Cannot prepare statement\n");
		$result = pg_execute($con, "prepare1", array(strtolower($this->username)))
			or die ("Cannot execute statement\n");

		pg_close($con);

		if(pg_num_rows($result) < 1) {
			return 1;
		}
		else {
			if(pg_num_rows($result)==1) {
				$user = pg_fetch_assoc($result);
				$this->id = $user['id'];
			}
			return 0;
		}
	}
	/*****
	** Checks if the user's mail address exists in the database
	*****/
	public function availableMail() {
		global $config;
		
		$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
			or die ("Could not connect to server\n");

		$query = "SELECT id FROM t_users WHERE lower(mail)=$1";

		pg_prepare($con, "prepare1", $query) 
			or die ("Cannot prepare statement\n");
		$result = pg_execute($con, "prepare1", array(strtolower($this->mail)))
			or die ("Cannot execute statement\n");

		pg_close($con);

		if(pg_num_rows($result) < 1) {
			return 1;
		}
		else {
			if(pg_num_rows($result)==1) {
				$user = pg_fetch_assoc($result);
				$this->id = $user['id'];
			}
			return 0;
		}
	}
	/*****
	** Creates a new user.
	*****/
	public function create() {
		global $config;

		$this->id = UUID::generate(UUID::UUID_RANDOM,UUID::FMT_STRING);

		$this->date_create = time();
		$this->date_update = time();
		$this->owner = $config['token'];
		$this->version = 1;

		$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
			or die ("Could not connect to server\n");

		$query = "INSERT INTO t_users (id, date_create, date_update, owner, version, username, password, mail, phone, rank, lastalert_rised, stepalert) VALUES
			($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";

		pg_prepare($con, "prepare1", $query) 
			or die ("Cannot prepare statement\n");
		pg_execute($con, "prepare1", array($this->id, $this->date_create, $this->date_update, $this->owner, $this->version, $this->username, $this->password, $this->mail, $this->phone, $this->rank, $this->lastalert_rised, $this->stepalert))
			or die ("Cannot execute statement\n");

		pg_close($con);
	}
	/*****
	** Update the user profile
	*****/
	public function update() {
		global $config;
		global $user;

		$this->date_update = time();
		$this->version = $this->version + 1;

		$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
			or die ("Could not connect to server\n");

		if($this->password=='') {
			$query = "UPDATE t_users SET date_update = $1, version = $2, username = $3, mail = $4, phone = $5, rank = $6, lastalert_rised = $7, stepalert = $8 WHERE id = $9";
			pg_prepare($con, "prepare1", $query) 
				or die ("Cannot prepare statement\n");
			pg_execute($con, "prepare1", array($this->date_update, $this->version, $this->username, $this->mail, $this->phone, $this->rank, $this->lastalert_rised, $this->stepalert, $this->id))
				or die ("Cannot execute statement\n");
		}
		else {
			$query = "UPDATE t_users SET date_update = $1, version = $2, username = $3, password = $4, mail = $5, phone = $6, rank = $7, lastalert_rised = $8, stepalert = $9 WHERE id = $10";
			pg_prepare($con, "prepare1", $query) 
				or die ("Cannot prepare statement\n");
			pg_execute($con, "prepare1", array($this->date_update, $this->version, $this->username, $this->password, $this->mail, $this->phone, $this->rank, $this->lastalert_rised, $this->stepalert, $this->id))
				or die ("Cannot execute statement\n");
		}

		pg_close($con);
	}
	/*****
	** Generates a random passwords, update the base and send the new password by mail.
	*****/
	public function sendPassword() {
		global $config;

		$newPass = randomPassword();
		$this->password = password_hash($newPass, PASSWORD_BCRYPT);

		$con = pg_connect("host=".$config['SQL_host']." dbname=".$config['SQL_db']." user=".$config['SQL_user']." password=".$config['SQL_pass'])
			or die ("Could not connect to server\n");

		$query = "UPDATE t_users SET password = $1 WHERE id = $2";

		pg_prepare($con, "prepare1", $query) 
			or die ("Cannot prepare statement\n");
		pg_execute($con, "prepare1", array($this->password, $this->id))
			or die ("Cannot execute statement\n");

		pg_close($con);

		$this->populate();

		$url = "http://".$_SERVER['SERVER_NAME'].$config['rel_root_folder'];

		$message = "Bonjour ".$this->username.",<br>\r\n";
		$message .= "<br>\r\n";
		$message .= "Voici votre nouveau mot de passe <a href='".$url."'>MMétéo</a> : <b>".$newPass."</b><br>\r\n";
		$message .= "<br>\r\n";
		$message .= "Chaleureusement,<br>\r\n";
		$message .= "<br>\r\n";
		$message .= "Votre MMétéo dévoué.<br>\r\n";

		$headers = 'From: '. $config['bot_mail'] . "\r\n" .
		'Reply-To: '. $config['bot_mail'] . "\r\n" .
		'X-Mailer: PHP/' . phpversion() . "\r\n" .
		'MIME-Version: 1.0' . "\r\n" .
		'Content-type: text/html; charset=UTF-8' . "\r\n"; 

		mail($this->mail, 'MMétéo - Nouveau mot de passe', $message, $headers);
	}
}

function randomPassword() {
    $alphabet = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
    $pass = array(); //remember to declare $pass as an array
    $alphaLength = strlen($alphabet) - 1; //put the length -1 in cache
    for ($i = 0; $i < 8; $i++) {
        $n = rand(0, $alphaLength);
        $pass[] = $alphabet[$n];
    }
    return implode($pass); //turn the array into a string
}

?>