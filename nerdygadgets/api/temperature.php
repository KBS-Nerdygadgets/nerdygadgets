<?php 

include "../database.php";
include "../functions/temperature.php";

// database connectie ophalen
$databaseConnection = connectToDatabase();

if(isset($_GET['id'])){
    // temperatuur updaten in database in customfields
    updateTemperature($databaseConnection, 5);

    // ophalen van temperatuur bij een item
    $fields = getTemperatureByItem($databaseConnection, $_GET['id']);

    // json decoden zodat je de temperature goed kan ophalen
    $obj = json_decode($fields[0]['CustomFields']);

    echo $obj->Temperature;
}

?>
