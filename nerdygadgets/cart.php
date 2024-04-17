<?php
include __DIR__ . "/header.php";

if(isset($_POST['id']) && isset($_POST['amount'])){
    changeCart($_POST['id'], $_POST['amount']);
} elseif(isset($_POST['id'])){
    removeFromCart($_POST['id']);
}

if (isset($_POST["discountCode"])){
    $discountCodeFromTextbox = $_POST["discountCode"]; 
    $discountedPrice = connectDatabaseDiscount($databaseConnection, $discountCodeFromTextbox);
    
    if ($discountedPrice == null) {
        $_SESSION['discount'] = null;
    }
}

$cart = getCart();
$total = getTotalAmountCart();
$subtotal = getTotalPriceCart();
$subtotalNoDiscount = getTotalPriceCart(false);

if(count($cart) > 0){
?>

<div id="CartSideText">
    <h2 class="FilterText">
       Winkelmand 
    </h2>
    <div>
        Aantal items in winkelmand: <?= $total ?>
    </div>
    <div>
        <h4 class="FilterText">
            </br>
            Kortingscode invoeren
        </h4>
    </div>

    <form action="cart.php" method="POST">
        <div class="col-20">
                <label for="Kortingscode" class="form-label">Type hier uw kortingscode</label>
                <input type="text" name="discountCode" value="<?php echo isset($_SESSION['discount']['code']) ? $_SESSION['discount']['code'] : "" ?>">
                <input type="submit" value="Toevoegen">
        </div>
    </form>   
    
    <?php 
    if (isset($_POST["discountCode"])){
        if ($discountedPrice !== null) {
            echo "Kortingscode geaccepteerd";
        } else {
            $_SESSION['discount'] = null;
            echo "Geen geldigde kortingscode gevonden";
        }
    }

    ?>
</div>        


<?php 
}
?>

<div id="ResultsArea" class="Browse" style="margin-bottom: 110px;">
<?php
foreach($cart as $name => $item){
$left = 250;
?>

    <div id="ProductFrame">
        <?php
        if (isset($item['img'])) { ?>
        <a class="ListItem" href='view.php?id=<?php print $name; ?>'>
            <div class="ImgFrame"
                    style="background-image: url('<?php print "Public/StockItemIMG/" . $item['img']; ?>'); background-size: 230px; background-repeat: no-repeat; background-position: center;"></div>
        </a>
        <?php } else{
            $left = 0;
        }
        ?>
        <form method="POST" id="deleteItemFromCart">
            <input style="display: none" name="id" value="<?= $name ?>">
            <button style="float: right; margin-top: 215px; margin-right: 10px; z-index: 1001 !important; position: relative; padding: 0; border: none; background: none;"><svg xmlns="http://www.w3.org/2000/svg" height="1em" viewBox="0 0 448 512"><!--! Font Awesome Free 6.4.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license (Commercial License) Copyright 2023 Fonticons, Inc. --><style>svg{fill:#ffffff}</style><path d="M135.2 17.7L128 32H32C14.3 32 0 46.3 0 64S14.3 96 32 96H416c17.7 0 32-14.3 32-32s-14.3-32-32-32H320l-7.2-14.3C307.4 6.8 296.3 0 284.2 0H163.8c-12.1 0-23.2 6.8-28.6 17.7zM416 128H32L53.2 467c1.6 25.3 22.6 45 47.9 45H346.9c25.3 0 46.3-19.7 47.9-45L416 128z"/></svg></button>
        </form>
        <form method="POST" id="changeBasketAmount">
            <input onchange="this.form.submit()" style="float: right; margin-top: 215px; z-index: 1000 !important; position: relative; width: 70px; height: 30px; margin-right: 10px;" id="amountProducts" type="number" value="<?= $item['amount'] ?>" min="1" max="<?= $item['quantity'] ?>" name="amount">
            <input style="display: none" name="id" value="<?= $name ?>">
        </form>
        <div id="StockItemFrameRight">
            <div class="CenterPriceLeftChild">
                <h1 class="StockItemPriceText"><?php print "€ " . number_format($item['price'] * $item['amount'], 2); ?></h1>
                <h6>Inclusief BTW </h6>
                
            </div>
            
        </div>
        <h1 class="StockItemID">Artikelnummer: <?php print $name; ?></h1>
        <a class="ListItem" href='view.php?id=<?php print $name; ?>'><p class="StockItemName"><?php print $item["name"]; ?></p></a>
        <h4 class="ItemQuantity" style="left: <?= $left ?> !important"><?php print "Aantal in winkelmand: " . $item['amount']; ?></h4>
        
    </div>

<?php

}

?>

</div>

<?php 
if(count($cart) > 0){
?>
<div style="width: 100%; height: 200px; background-color: #23232F; position: fixed; top: 100%; left: 50%; transform: translate(-50%, -50%); z-index: 2;">
    <div style="float: right; margin-right: 20px; font-size: 15px;">Subtotaal</div>
    <h1 class="StockItemPriceText" style="float: right; clear: both; margin-right: 20px; margin-top: 2px;"><?php print "€ " . number_format($subtotal, 2); ?></h1>
    <?php 
    if(isset($_SESSION['discount']['code'])){
    ?>
        <h3 class="StockItemPriceText" style="margin-top: -28px; float: right; clear: both; margin-right: 183px; font-size: 15px; color: #ff0033;"><s><?php print "€ " . number_format($subtotalNoDiscount, 2); ?></s></h3>
    <?php
    }
    ?>
    <a href="address.php">
        <button style="width: 20%; float: right; clear: both; margin-right: 20px;" onclick="">Naar afrekenen</button>
    </a>
    <a href="categories.php" style="color: white; text-decoration: none; float: left; margin-left: 20px;">&lt; Verder winkelen</a>
</div>
<?php 
}
?>



<?php 

if(count($cart) <= 0){
    print '<h2 style="text-align: center; margin-top: 10px;">Er zijn geen producten toegevoegd in de winkelmand</h2>';
}

include __DIR__ . "/footer.php";
?>







