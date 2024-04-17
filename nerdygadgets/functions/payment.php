<?php

function createCustomer($databaseConnection, $firstName, $fullName, $phoneNumber, $address, $postalCode, $email, $password = null){
    $result = null;

    // begin transactie om een gebruiker aan te maken
    $databaseConnection->begin_transaction();

    if($password !== null){
        $password = strtoupper(hash("sha256", $password));
    }

    try {
        // kijk of de customer en person bestaat
        $existPeople = getPeopleByEmail($databaseConnection, $email);
        $existCustomer = null;
        if($existPeople !== null){
            $existCustomer = getCustomerByPersonId($databaseConnection, $existPeople['PersonID']);
        }

        if($existCustomer !== null && $existPeople !== null){
            // customer bestaat
            if($password !== null){
                // als er een gebruiker aan moet worden gemaakt
                // check of de gebruiker al bestaat
                $peopleLogin = getPeopleByLogin($databaseConnection, $email);

                if($peopleLogin !== null){
                    // gebruiker bestaat al, kan dus niet aanmaken
                    $_SESSION['error'] = "Er bestaat al een gebruiker met dit emailadres";
                } else{
                    // gebruiker bestaat nog niet dus logonname en password moet worden geupdate
                    $updateQuery = "
                    UPDATE people 
                    SET FullName = ?, PreferredName = ?, SearchName = ?, LogonName = ?, HashedPassword = ? 
                    WHERE PersonId = ?";

                    $req = $databaseConnection->prepare($updateQuery);
                    $searchName = $firstName . ' ' . $fullName;

                    $req->bind_param("sssssi", $fullName, $firstName, $searchName, $email, $password, $existPeople['PersonID']);
                
                    if($req->execute()){
                        // gebruiker bestaat nu
                        $result = array();
                        $result['CustomerID'] = $existCustomer['CustomerID'];
                        $result['PersonID'] = $existPeople['PersonID'];
                    } else{
                        $_SESSION['error'] = "Kon de gebruiker niet aanmaken door een fout in onze database";
                    }
                }
            } else{
                // if user does not have an address already, add a address to the database
                if(($existCustomer['DeliveryAddressLine1'] == '' && $existCustomer['DeliveryPostalCode'] == '') || ($address !== '' && $postalCode !== '')){
                    // moet een address worden toegevoegd
                    $updateQuery = "
                    UPDATE customers 
                    SET DeliveryAddressLine1 = ?, DeliveryPostalCode = ?
                    WHERE CustomerID = ?";

                    $req = $databaseConnection->prepare($updateQuery);

                    $req->bind_param("ssi", $address, $postalCode, $existCustomer['CustomerID']);
                
                    if($req->execute()){
                        // gebruiker heeft adresgegevens nu
                        $result = array();
                        $result['CustomerID'] = $existCustomer['CustomerID'];
                        $result['PersonID'] = $existPeople['PersonID'];
                    } else{
                        $_SESSION['error'] = "Kon de adresgegevens niet aanpassen door een fout in onze database";
                    }
                } else{
                    // gebruiker heeft adresgegevens in de database
                    $result = array();
                    $result['CustomerID'] = $existCustomer['CustomerID'];
                    $result['PersonID'] = $existPeople['PersonID'];
                }
            }
        } else{
            // customer bestaat niet

            // De persoon toevoegen aan de database
            if($password == null){
                $insertQuery = "
                INSERT INTO `people` 
                (`PersonID`, `FullName`, `PreferredName`, `SearchName`, `IsPermittedToLogon`, `LogonName`, `IsExternalLogonProvider`, `HashedPassword`, `IsSystemUser`, `IsEmployee`, `IsSalesperson`, `UserPreferences`, `PhoneNumber`, `FaxNumber`, `EmailAddress`, `Photo`, `CustomFields`, `OtherLanguages`, `LastEditedBy`, `ValidFrom`, `ValidTo`) 
                VALUES 
                (NULL, ?, ?, ?, '0', 'NO LOGON', '0', NULL, '0', '0', '0', NULL, ?, ?, ?, NULL, NULL, NULL, 1, '2023-11-30 17:56:38.000000', '2023-11-30 17:56:38.000000');
                ";

                $req = $databaseConnection->prepare($insertQuery);

                $req->bind_param("ssssss", $fullName, $firstName, $fullName, $phoneNumber, $phoneNumber, $email);
            } else{
                $insertQuery = "
                INSERT INTO `people` 
                (`PersonID`, `FullName`, `PreferredName`, `SearchName`, `IsPermittedToLogon`, `LogonName`, `IsExternalLogonProvider`, `HashedPassword`, `IsSystemUser`, `IsEmployee`, `IsSalesperson`, `UserPreferences`, `PhoneNumber`, `FaxNumber`, `EmailAddress`, `Photo`, `CustomFields`, `OtherLanguages`, `LastEditedBy`, `ValidFrom`, `ValidTo`) 
                VALUES 
                (NULL, ?, ?, ?, '1', ?, '0', ?, '0', '0', '0', NULL, ?, ?, ?, NULL, NULL, NULL, 1, '2023-11-30 17:56:38.000000', '2023-11-30 17:56:38.000000');
                ";

                $req = $databaseConnection->prepare($insertQuery);

                $req->bind_param("ssssssss", $fullName, $firstName, $fullName, $email, $password,  $phoneNumber, $phoneNumber, $email);
            }
        
            if($req->execute()){
            // Als de persoon succesvol is gemaakt
                $person = getPerson($databaseConnection, $databaseConnection->insert_id);
                
                if($person !== null){
                    // Persoon bestaat en toevoegen aan customer
                    $insertQuery = "
                    INSERT INTO `customers` 
                    (`CustomerID`, `CustomerName`, `BillToCustomerID`, `CustomerCategoryID`, `BuyingGroupID`, `PrimaryContactPersonID`, `AlternateContactPersonID`, `DeliveryMethodID`, `DeliveryCityID`, `PostalCityID`, `CreditLimit`, `AccountOpenedDate`, `StandardDiscountPercentage`, `IsStatementSent`, `IsOnCreditHold`, `PaymentDays`, `PhoneNumber`, `FaxNumber`, `DeliveryRun`, `RunPosition`, `WebsiteURL`, `DeliveryAddressLine1`, `DeliveryAddressLine2`, `DeliveryPostalCode`, `DeliveryLocation`, `PostalAddressLine1`, `PostalAddressLine2`, `PostalPostalCode`, `LastEditedBy`, `ValidFrom`, `ValidTo`) 
                    VALUES 
                    (NULL, ?, '1', '9', NULL, ?, NULL, '8', '1', '1', NULL, '2023-01-01', '0.000', '0', '0', '7', ?, ?, NULL, NULL, ?, ?, NULL, ?, NULL, 'PO Box 1', NULL, '8888PO', '1', '2023-11-30 18:09:47.000000', '2099-11-30 18:09:47.000000');
                    ";

                    $req = $databaseConnection->prepare($insertQuery);

                    $microsoft = "http://www.microsoft.com/" . $fullName . "/";
                    $req->bind_param("sssssss", $fullName, $person['PersonID'], $phoneNumber, $phoneNumber, $microsoft, $address, $postalCode);
                
                    if($req->execute()){
                        // Customer is aangemaakt
                        $customer = getCustomer($databaseConnection, $databaseConnection->insert_id);

                        $result = array();
                        $result['CustomerID'] = $customer['CustomerID'];
                        $result['PersonID'] = $person['PersonID'];
                    }
                } else{
                    // Kon de persoon niet ophalen
                }
            } else {
                // Kon de persoon niet aanmaken
            }
        }

        // alles is goedgegaan dus commit
        $databaseConnection->commit();

    } catch (Exception $e) {
        // er is een error dus rollback
        $databaseConnection->rollback();
        
        $_SESSION['error'] = 'Er is iets fout gegaan in onze database';
    }

    return $result;
}

function createOrder($databaseConnection, $customerId, $personId){
    $result = null;

    // Order aanmaken
    $insertQuery = "
    INSERT INTO `orders` 
    (`OrderID`, `CustomerID`, `SalespersonPersonID`, `PickedByPersonID`, `ContactPersonID`, `BackorderOrderID`, `OrderDate`, `ExpectedDeliveryDate`, `CustomerPurchaseOrderNumber`, `IsUndersupplyBackordered`, `Comments`, `DeliveryInstructions`, `InternalComments`, `PickingCompletedWhen`, `LastEditedBy`, `LastEditedWhen`) 
    VALUES 
    (NULL, ?, '2', '2', ?, NULL, '2013-01-01 00:00:00', '2013-01-01 00:00:00', NULL, '1', NULL, NULL, NULL, '2013-01-01 00:00:00', '1', '2023-11-30 18:34:20.000000');
    ";

    $req = $databaseConnection->prepare($insertQuery);

    $req->bind_param("ss", $customerId, $personId);

    if($req->execute()){
        // Order is aangemaakt
        $order = getOrder($databaseConnection, $databaseConnection->insert_id);

        $result = $order['OrderID'];
    }

    return $result;
}

function createOrderLine($databaseConnection, $orderId, $stockItemId, $quantity){
    $result = null;

    $order = getOrder($databaseConnection, $orderId);
    $stockItem = getStockItemById($databaseConnection, $stockItemId);

    // Orderline aanmaken
    $insertQuery = "
    INSERT INTO `orderlines` 
    (`OrderLineID`, `OrderID`, `StockItemID`, `Description`, `PackageTypeID`, `Quantity`, `UnitPrice`, `TaxRate`, `PickedQuantity`, `PickingCompletedWhen`, `LastEditedBy`, `LastEditedWhen`) 
    VALUES 
    (NULL, ?, ?, ?, ?, ?, ?, ?, ?, '2023-11-30 18:57:43', '1', '2023-11-30 18:57:42.000000');
    ";

    $req = $databaseConnection->prepare($insertQuery);

    $req->bind_param("ssssssss", $orderId, $stockItemId, $stockItem['SearchDetails'], $stockItem['UnitPackageID'], $quantity, $stockItem['UnitPrice'], $stockItem['TaxRate'], $quantity);

    if($req->execute()){
        // Orderline is aangemaakt
        $orderline = getOrderLine($databaseConnection, $databaseConnection->insert_id);

        $result = $orderline['OrderLineID'];
    }

    return $result;
}

function changeQuantity($databaseConnection, $stockItemId, $newQuantity){
    // Update voorraad
    $updateQuery = "
    UPDATE `stockitemholdings` 
    SET `QuantityOnHand` = '" . $newQuantity . "'
    WHERE `stockitemholdings`.`StockItemID` = " . $stockItemId . ";";

    return $databaseConnection->query($updateQuery);
}

function createPayment($databaseConnection, $firstName, $fullName, $phoneNumber, $address, $postalCode, $email){
    $orderId = "";
    $succes = false;

    if(checkQuantityForCart($databaseConnection)){
        // gebruiker aanmaken
        $customer = createCustomer($databaseConnection, $firstName, $fullName, $phoneNumber, $address, $postalCode, $email);

        // als gebruiker is aangemaakt
        if($customer !== null){
            // ophalen van de customer en person
            $customerId = $customer['CustomerID'];
            $personId = $customer['PersonID'];

            // maak een order
            $orderId = createOrder($databaseConnection, $customerId, $personId);

            // als een order is gemaakt
            if($orderId !== null){
                // ophalen van winkelmand
                $cart = getCart();

                // een for loop door de items in de winkelmand 
                foreach($cart as $id => $item){
                    // toevoegen als orderline
                    $orderLineId = createOrderLine($databaseConnection, $orderId, $id, $item['amount']);

                    // als orderline is toegevoegd moet succes true worden
                    if($orderLineId !== null){
                        $succes = true;
                    }
                }
            } else{
                // order kon niet gemaakt worden
            }

        } else{
            // customer kon niet gemaakt worden
        }
    } else{
        // niet genoeg voorraad

    }

    return $orderId;
}

function completePayment($databaseConnection, $orderId){
    $result = true;
    
    // ophalen winkelmand
    $cart = getCart();
    
    if(checkQuantityForCart($databaseConnection)){
        if($cart !== null){
            // een for loop door de items in de winkelmand 
            foreach($cart as $id => $item){
                // ophalen van huidige voorraad
                $quantity = getQuantity($databaseConnection, $id);

                // nieuwe voorraad vast stellen
                $newQuantity = intval($quantity['QuantityOnHand']) - intval($item['amount']);

                // aanpassen van voorraad in database
                $quantityChanged = changeQuantity($databaseConnection, $id, $newQuantity);

                // aanpassen van order om succes te maken
                $orderSuccess = changeOrderSucces($databaseConnection, $orderId);

                // is succesvol gelukt
                if($quantityChanged && $orderSuccess){
                    // quantity is succesvol aangepast
                } else{
                    $result = false;
                }
            }
        }
    } else{
        // is niet genoeg voorraad
        header('Location: order.php?succes=false&opmerking=voorraadcomplete');
    }

    if($result){
        // als alles goed is gegaan moet de winkelmand worden leeg gemaakt
        $_SESSION['cart'] = array();
        $_SESSION['discount'] = null;
    }
}

function checkQuantityForCart($databaseConnection){
    // resultaat is true automatisch maar als er een item niet op voorraad is word ie naar false gezet
    $result = true;

    // get cart
    $cart = getCart();

    // een loop die door alle items gaat in de winkelmand
    foreach($cart as $id => $item){
        // ophalen van huidige voorraad
        $quantity = getQuantity($databaseConnection, $id);

        // kijk of voorraad klopt
        if($quantity >= $item['amount']){
            // er zijn genoeg in voorraad
        } else{
            $result = false;
        }
    }

    return $result;
}

function getPerson($databaseConnection, $id) {
    $Result = null;

    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM people WHERE PersonID = ? LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("i", $id);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getCustomer($databaseConnection, $id) {
    $Result = null;

    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM customers WHERE CustomerID = ? LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("i", $id);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getOrder($databaseConnection, $id){
    $Result = null;
    
    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM orders WHERE OrderID = ? LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("i", $id);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getOrderLine($databaseConnection, $id){
    $Result = null;
    
    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM orderlines WHERE OrderLineID = ? LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("i", $id);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getStockItemById($databaseConnection, $id){
    $Result = null;
    
    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM stockitems WHERE StockItemID = ? LIMIT 1";

    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("i", $id);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getQuantity($databaseConnection, $id){
    $Result = null;
    
    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM stockitemholdings WHERE StockItemID = ? LIMIT 1";

    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("i", $id);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getCustomerByPersonId($databaseConnection, $personId) {
    $Result = null;

    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM customers WHERE PrimaryContactPersonID = ? LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("i", $personId);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getPeopleByEmail($databaseConnection, $email) {
    $Result = null;

    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM people WHERE EmailAddress = ? LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("s", $email);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function getPeopleByLogin($databaseConnection, $email) {
    $Result = null;

    // Prepare and execute the SQL statement
    $sql = "SELECT * FROM people WHERE LogonName = ? AND IsSystemUser = 0 AND IsEmployee = 0 AND IsSalesperson = 0 LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("s", $email);
    $stmt->execute();

    // Get the result
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

function changeOrderSucces($databaseConnection, $orderId){
    // Update order naar success
    $updateQuery = "
    UPDATE orders 
    SET Comments = 'success'
    WHERE OrderID = " . $orderId . ";";

    return $databaseConnection->query($updateQuery);
}

?>