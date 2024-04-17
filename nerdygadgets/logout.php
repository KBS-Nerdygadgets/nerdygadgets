<?php 

if(session_status() === PHP_SESSION_NONE){
    session_start();
}

if(isset($_SESSION['token'])){
    unset($_SESSION['token']);
}

header('Location: login.php');

?>