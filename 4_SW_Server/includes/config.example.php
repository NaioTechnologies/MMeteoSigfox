<?

$config['version'] = "XXXX";
$config['token'] = "XXXX";

/*****
** Management of folder names
*****/

// It is the include folder name
$config['include_folder']=basename(__DIR__);
// This is the absolute folder to the root of the website
$config['abs_root_folder']=str_replace($config['include_folder'],"",__DIR__);
// This is the relative folder to the root of the website from the DocumentRoot (can also be called subfolder)
$config['rel_root_folder']=str_replace("\\", "/", $config['abs_root_folder']);
$config['rel_root_folder']=str_replace($_SERVER['DOCUMENT_ROOT'],"",$config['rel_root_folder']);
if($config['rel_root_folder']=="") $config['rel_root_folder']="/";
// Here all the absolute paths to specific folders
$config['views_folder'] = $config['abs_root_folder']."views/";
$config['controllers_folder'] = $config['abs_root_folder']."controllers/";
$config['models_folder'] = $config['abs_root_folder']."models/";
$config['includes_folder'] = $config['abs_root_folder']."includes/";
$config['third_folder'] = $config['abs_root_folder']."third/";

// Here all the relative url to specific folders
$config['views_url'] = $config['rel_root_folder']."views/";

/*****
** SQL Database configuration
*****/

$config['SQL_host'] = "XXXX";
$config['SQL_user'] = "XXXX";
$config['SQL_pass'] = "XXXX";
$config['SQL_db'] = "XXXX";

/*****
** Mails config for alerts
*****/

$config['bot_mail'] = "XXXX";

?>