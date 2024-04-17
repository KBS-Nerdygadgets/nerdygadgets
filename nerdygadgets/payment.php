<?php
include __DIR__ . "/func_header.php";

$Countries = getCountries($databaseConnection);
$subtotal = getTotalPriceCart();

if(isset($_POST['postcode'])){
    // postcode ready maken voor de database
    $_POST['postcode'] = str_replace(" ", "", $_POST['postcode']);
    $_POST['postcode'] = preg_replace('/([0-9]+)([a-zA-Z]+)/', '$1 $2', $_POST['postcode']);
}

if(isset($_POST['submit']) && isset($_POST['voornaam']) && isset($_POST['achternaam']) && isset($_POST['email']) && isset($_POST['telefoonnummer']) && isset($_POST['adres']) && isset($_POST['huisnummer']) && isset($_POST['postcode']) && isset($_POST['provincie']) && isset($_POST['stad']) && isset($_POST['land'])){
    $fullName = "";
    if(isset($_POST['tv'])){
        $fullName = $_POST['voornaam'] . " " . $_POST['tv'] . " " . $_POST['achternaam'];
    } else{
        $fullName = $_POST['voornaam'] . " " . $_POST['achternaam'];
    }

    $adres = $_POST['adres'] . ' ' . $_POST['huisnummer'];

    $paymentSuccess = createPayment($databaseConnection, $_POST['voornaam'], $fullName, $_POST['telefoonnummer'], $adres, $_POST['postcode'], $_POST['email']);

    if($paymentSuccess){
        // payment is succesvol gemaakt
        header('Location: ideal.php?orderid=' . $paymentSuccess . '&price=' . str_replace(",", "", number_format($subtotal, 2)));
    } else{
        // payment is niet gelukt om aan te maken
    }
}

if(isset($_SESSION['token']) && isset($_POST['submit']) && isset($_POST['adres']) && isset($_POST['huisnummer']) && isset($_POST['postcode']) && isset($_POST['provincie']) && isset($_POST['stad']) && isset($_POST['land'])){
    $verifyTok = verifyToken($databaseConnection, $_SESSION['token']);
    
    $adres = $_POST['adres'] . ' ' . $_POST['huisnummer'];
    
    if($verifyTok['result']){
        $paymentSuccess = createPayment($databaseConnection, '', '', '', $adres, $_POST['postcode'], $verifyTok['person']['LogonName']);

        if($paymentSuccess){
            // payment is succesvol gemaakt
            header('Location: ideal.php?orderid=' . $paymentSuccess . '&price=' . str_replace(",", "", number_format($subtotal, 2)));
        } else{
            // payment is niet gelukt om aan te maken
        }
    } else{
        $_SESSION['error'] = 'Token is ongeldig';
    }
}

if(isset($_SESSION['token']) && isset($_GET['pay'])){
    $verifyToken = verifyToken($databaseConnection, $_SESSION['token']);

    if($verifyToken['result'] && $_GET['pay'] == '1'){
        $paymentSuccess = createPayment($databaseConnection, '', '', '', '', '', $verifyToken['person']['LogonName']);

        if($paymentSuccess){
            // payment is succesvol gemaakt
            header('Location: ideal.php?orderid=' . $paymentSuccess . '&price=' . str_replace(",", "", number_format($subtotal, 2)));
        } else{
            // payment is niet gelukt om aan te maken
        }
    } else{
        $_SESSION['error'] = 'Token is ongeldig';
    }
}

include __DIR__ . "/header.php";

?>
<div class="IndexStyle" style="margin-top: 30px;">
    <form method="POST">
        <?php if(!isset($_SESSION['token'])){ ?>
        <div class="row">
            <div class="col-5">
                <label for="voornaam" class="form-label">Voornaam*</label>
                <input type="text" class="form-control" id="voornaam" placeholder="Voornaam" maxlength="256" name="voornaam" required>
            </div>
            <div class="col-2">
                <label for="tussenvoegsel" class="form-label">Tussenvoegsel</label>
                <input type="text" class="form-control" id="tussenvoegsel" maxlength="100" name="tv" placeholder="Tussenvoegsel (optioneel)">
            </div>
            <div class="col-5">
                <label for="achternaam" class="form-label">Achternaam*</label>
                <input type="text" class="form-control" id="achternaam" placeholder="Achternaam" name="achternaam" maxlength="256" required>
            </div>
            <div class="col-4">
                <label for="email" class="form-label">Email*</label>
                <input type="email" class="form-control" id="email" placeholder="Email" maxlength="256" name="email" required>
            </div>
            <div class="col-4">
                <label for="telefoonnummer" class="form-label">Telefoonnummer*</label>
                <input type="text" class="form-control" id="telefoonnummer" placeholder="Telefoonnummer" name="telefoonnummer" pattern="^[0-9\s\-()+]+" maxlength="256" required>
            </div>
        </div>
        <?php } ?>
        <div class="row" style="<?= !isset($_SESSION['token']) ? 'margin-top: 80px;' : 'margin-top: 0px;' ?>">
            <div class="col-6">
                <label for="Adres" class="form-label">Adres*</label>
                <input type="text" class="form-control" id="Adres" placeholder="Adres" maxlength="1000" name="adres" required>
            </div>
            <div class="col-3">
                <label for="Huisnummer" class="form-label">Huisnummer*</label>
                <input type="text" class="form-control" id="Huisnummer" placeholder="Huisnummer" pattern="^[1-9][0-9]{0,4} ?[A-Za-z]{0,3}$" name="huisnummer" maxlength="500" required>
            </div>
            <div class="col-3">
                <label for="Postcode" class="form-label">Postcode*</label>
                <input type="text" class="form-control" id="Postcode" placeholder="Postcode (bijv. 9999AB)" name="postcode" pattern="[1-9][0-9]{3}\s?[a-zA-Z]{2}" maxlength="500" required>
            </div>
            <div class="col-3">
                <label for="Provincie" class="form-label">Provincie*</label>
                <input type="text" class="form-control" id="Provincie" placeholder="Provincie" name="provincie" maxlength="1000" required>
            </div>
            <div class="col-3">
                <label for="Stad" class="form-label">Stad*</label>
                <input type="text" class="form-control" id="Stad" placeholder="Stad" maxlength="1000" name="stad" required>
            </div>
            <div class="col-4">
                <label for="Land" class="form-label">Land*</label>
                <select name="land" class="form-control" required>
                    <?php 

                        foreach($Countries as $Country){
                            if($Country['CountryName'] == "Netherlands"){
                                echo "<option selected value='" . $Country['CountryName'] . "'>" . $Country['CountryName'] . "</option>";
                            } else{
                                // echo "<option value='" . $Country['CountryID'] . "'>" . $Country['CountryName'] . "</option>";
                            }
                        }
                
                    ?>
                </select>
            </div>
        </div>
        <div>
            <div style="float: right; font-size: 15px; margin-top: 50px;">Subtotaal</div>
            <h1 class="StockItemPriceText" style="float: right; clear: both; margin-top: 5px"><?php print "€ " . number_format($subtotal, 2); ?></h1>
            <input type="submit" value="Bestelling plaatsen" name="submit" style="width: 40%; float: right; clear: both; margin-top: 10px;"/>
        </div>
    </form>
</div>
<?php
include __DIR__ . "/footer.php";
?>

