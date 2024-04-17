<?php
function updateRecordedWhen($databaseConnection, $coldRoomSensorNumber) {

    if ($databaseConnection->connect_error) {
        die("Connection failed: " . $databaseConnection->connect_error);
    }

    $sqlSelect = "SELECT RecordedWhen FROM coldroomtemperatures WHERE ColdRoomSensorNumber = 5";
    $result = $databaseConnection->query($sqlSelect);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $date = $row["RecordedWhen"];

        $sqlUpdate = "UPDATE stockitems SET CustomFields = JSON_SET(CustomFields, '$.RecordedWhen', '$date') WHERE IsChillerStock = 1";

        if ($databaseConnection->query($sqlUpdate) === TRUE) {
        } else {
            echo "Error updating date: " . $databaseConnection->error;
        }
    } else {
        echo "No matching record found in coldroomtemperatures.";
    }
}

function getDateByItem($databaseConnection, $id){   
    $Query = "
                SELECT CustomFields
                FROM stockitems 
                WHERE StockItemID = ? LIMIT 1";

    $Statement = mysqli_prepare($databaseConnection, $Query);
    mysqli_stmt_bind_param($Statement, "i", $id);
    mysqli_stmt_execute($Statement);
    $R = mysqli_stmt_get_result($Statement);
    $R = mysqli_fetch_all($R, MYSQLI_ASSOC);

    return $R;
}

?>