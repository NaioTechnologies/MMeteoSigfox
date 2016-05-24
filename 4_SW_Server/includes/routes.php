<?

/*****
** This file contains the routing from any request to the correct view and controller
*****/

$controller = new stdClass;
$view = new stdClass;

$controller->full_url = $_SERVER['REQUEST_URI'];
$controller->url_no_param = explode('?',$controller->full_url);

// URL without ?parameters and /subfolder/
$controller->base_url=str_replace('RACINE'.$config['rel_root_folder'],'','RACINE'.$controller->url_no_param[0]);
$controller->splitted_url = explode ('/',$controller->base_url);

// By default we use the desktop
$notfound = 0;

// Routing to the correct page from the correct link
switch ($controller->splitted_url[0])
{
    case "index": case "" :
        $controller->name="";
        $view->name="index";
        break;
    case "user" :
    case "station" :
    case "admin" :
        $controller->name=$controller->splitted_url[0];
        $view->name="";
        break;
    default : 
        $controller->name="";
        $view->name="";
		$notfound = 1;
        break;
}

require_once('session.php');

if($controller->name != "") {
	include ($config['controllers_folder'].$controller->name.".php");
}
if($view->name != "") {
	include ($config['views_folder'].$view->name.".html");
}

if($notfound) {
    $head['title'] = "Erreur 404";
    include ($config['views_folder']."404.html");
}

?>