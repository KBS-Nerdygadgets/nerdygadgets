<?php
include __DIR__ . "/header.php";

if(isset($_GET['succes']) && isset($_GET['orderid']) && $_GET['succes'] == "true"){
    completePayment($databaseConnection, $_GET['orderid']);
}

?>
<div class="IndexStyle" style="margin-top: 10px;">

    <?php if(isset($_GET['succes']) && isset($_GET['orderid']) && $_GET['succes'] == "true"){ ?>
    <div class="alert alert-success" role="alert">
        De bestelling is succesvol afgerond
        <br>
        Uw bestellingsnummer is <?= $_GET['orderid'] ?>
    </div>
    <?php } ?>

    <?php if(isset($_GET['succes']) && isset($_GET['opmerking']) && $_GET['succes'] == "false" && $_GET['opmerking'] == "voorraadcomplete"){ ?>
    <div class="alert alert-danger" role="alert">
        Er is niet genoeg voorraad voor dit product, het bedrag zou zo snel mogelijk terug gestuurd worden naar uw bankrekening.
    </div>
    <?php } ?>

    <?php if(isset($_GET['succes']) && isset($_GET['opmerking']) && $_GET['succes'] == "false" && $_GET['opmerking'] == "voorraad"){ ?>
    <div class="alert alert-danger" role="alert">
        Er is niet genoeg voorraad voor dit product.
    </div>
    <?php } ?>

    <?php if(isset($_GET['succes']) && isset($_GET['opmerking']) && $_GET['succes'] == "false" && $_GET['opmerking'] == "cancel"){ ?>
    <div class="alert alert-danger" role="alert">
        De bestelling is geannuleerd.
    </div>
    <?php } ?>

    <?php if(isset($_GET['succes']) && isset($_GET['opmerking']) && $_GET['succes'] == "true" && $_GET['opmerking'] == "pending"){ ?>
    <div class="alert alert-warning" role="alert">
        De bestelling is nog in afhandeling, u ontvangt een mail zodra deze afgerond is.
    </div>
    <?php } ?>

    <?php if(isset($_GET['succes']) && !isset($_GET['opmerking']) && $_GET['succes'] == "false"){ ?>
    <div class="alert alert-danger" role="alert">
        Er is iets fout gegaan tijdens het bestellen van de producten.
    </div>
    <?php } ?>
</div>
<?php
include __DIR__ . "/footer.php";
?>

