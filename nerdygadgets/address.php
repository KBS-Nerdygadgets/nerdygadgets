<?php
include __DIR__ . "/func_header.php";

if(!isset($_SESSION['token'])){
    header('Location: login.php?cart=1');
} else{
    // user has a token
    $verify = verifyToken($databaseConnection, $_SESSION['token']);

    if($verify['result']){
        // gebruiker bestaat
        $customer = getCustomerByPersonId($databaseConnection, $verify['person']['PersonID']);

        if($customer['DeliveryAddressLine1'] == '' || $customer['DeliveryPostalCode'] == ''){
            // heeft geen adres, redirect 
            header('Location: payment.php');
        } else{
            // heeft adres, dus toon scherm
        }
    } else{
        // kon niet verifyen
        header('Location: payment.php');
    }
}

include __DIR__ . "/header.php";
?>
<div class="IndexStyle d-flex justify-content-center" style="margin-top: 30px; width: 40%;">
    <div style="width: 100%;">
        <div style="font-size: 30px; margin-bottom: 0px;">Adresgegevens</div>
        <div style="font-size: 20px; margin-bottom: 20px;">We hebben opgeslagen adresgegevens gevonden bij uw klanten account</div>
        <div style="border: 5px solid #5055ba; padding: 15px;">
            <div style="font-size: 20px;"><?= $customer['CustomerName'] ?></div>
            <div style="font-size: 20px;"><?= $customer['DeliveryAddressLine1'] ?>, <?= $customer['DeliveryPostalCode'] ?></div>
        </div>
        <div style="margin-top: 20px; font-size: 20px; margin-bottom: 10px;">Wilt u deze adresgegevens gebruiken, of wilt u het handmatig invullen?</div>
        <a href="payment.php?pay=1">
            <button style="width: 40%; height: 20%; margin-right: 20px;" onclick="">Afrekenen</button>
        </a>
        <a href="payment.php">
            <button style="width: 40%; height: 20%; margin-right: 20px;" onclick="">Adresgegevens aanpassen</button>
        </a>
    </div>
</div>
<?php
include __DIR__ . "/footer.php";
?>

