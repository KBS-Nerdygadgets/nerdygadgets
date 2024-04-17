<?php 

if(session_status() === PHP_SESSION_NONE){
    session_start();
}

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

include "database.php";

include "functions/cart.php";
include "functions/payment.php";
include "functions/auth.php";

$databaseConnection = connectToDatabase();

?>