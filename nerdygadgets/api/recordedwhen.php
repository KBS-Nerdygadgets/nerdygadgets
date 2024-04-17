<?php 

include "../database.php";
include "../functions/recordedwhen.php";

// database connectie ophalen
$databaseConnection = connectToDatabase();

if(isset($_GET['id'])){
    // datum updaten in database in customfields
    updateRecordedWhen($databaseConnection, 5);

    // ophalen van de datum bij een item
    $fields = getDateByItem($databaseConnection, $_GET['id']);

    // json decoden zodat je de date goed kan ophalen
    $obj = json_decode($fields[0]['CustomFields']);

    echo $obj->RecordedWhen;
}

?>
