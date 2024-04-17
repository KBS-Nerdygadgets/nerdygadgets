<?php

function addToCart($id, $amount, $StockItem, $StockItemImage){
    // add everything in the session (cart) with the specific id
    $_SESSION['cart'][$id]['name'] = $StockItem['StockItemName'];
    $_SESSION['cart'][$id]['img'] = isset($StockItemImage[0]['ImagePath']) ? $StockItemImage[0]['ImagePath'] : '';
    $_SESSION['cart'][$id]['price'] = $StockItem['SellPrice'];
    $_SESSION['cart'][$id]['quantity'] = intval(str_replace("Voorraad: ", "", $StockItem['QuantityOnHand']));

    // check if the amount is already defined, if so add new amount to cart otherwise make amount of item in cart the amount
    if(isset($_SESSION['cart'][$id]['amount'])){
        $_SESSION['cart'][$id]['amount'] = validationAmount($_SESSION['cart'][$id]['amount'] + $amount, $_SESSION['cart'][$id]['quantity']);
    } else{
        $_SESSION['cart'][$id]['amount'] = validationAmount($amount, $_SESSION['cart'][$id]['quantity']);
    }
}

function changeCart($id, $amount){
    // change amount of cart
    if(isset($_SESSION['cart'][$id]['amount'])){
        $_SESSION['cart'][$id]['amount'] = validationAmount($amount, $_SESSION['cart'][$id]['quantity']);
    }
}

function removeFromCart($id){
    // remove item from cart by using unset (removes array part)
    if(isset($_SESSION['cart'][$id])){
        unset($_SESSION['cart'][$id]);
    }
}

function validationAmount($amount, $quantity){
    // if amount lower than quantity it will stay amount otherwise it will change to the quantity cause the amount cant be higher than the items that are in stock
    if($amount < $quantity){
        $amount = $amount;
    } else{
        $amount = $quantity;
    }

    // if amount is higher than 0 it will stay amount otherwise its lower than 0 so a minus amount and that is not allowed in the application
    if($amount > 0){
        $amount = $amount;
    } else{
        $amount = 0;
    }

    // return updated amount
    return $amount;
}

function getCart(){
    // return the complete cart filtering all amount 0 items out
    if(isset($_SESSION['cart'])){
        $_SESSION['cart'] = array_filter($_SESSION['cart'], function($item) { return $item['amount'] != 0; });
    }
    
    return isset($_SESSION['cart']) ? $_SESSION['cart'] : array();
}

function getItemAmountCart($id){
    // return the amount of the specific item in the cart
    return isset($_SESSION['cart'][$id]['amount']) ? $_SESSION['cart'][$id]['amount'] : 0;
}

function getTotalPriceCart($discounted = true){
    // calculate total price of the cart
    return $discounted ? getDiscounted() : array_sum(array_map(function($item) { return $item['amount'] * $item['price']; }, getCart()));
}

function getTotalAmountCart(){
    // calculate total amount of the cart
    return array_sum(array_column(getCart(), 'amount'));
}

function getDiscounted(){
    $price = array_sum(array_map(function($item) { return $item['amount'] * $item['price']; }, getCart()));

    if(isset($_SESSION['discount'])){
        $discountAmount = $_SESSION['discount']['discountamount'];
        $discountPercentage = $_SESSION['discount']['discountpercentage'];

        if ($discountPercentage > 0) {
            $discount = $discountPercentage;
            $price = $price * (1 - $discountPercentage);
        } else {
            $discount = $discountAmount;
            $price = $price - $discount;
        }
    }

    if($price <= 0){
        $price = 0;
    }

    return $price;
}

function connectDatabaseDiscount($databaseConnection, $discountCode) {
    if ($databaseConnection->connect_error) {
        die("Connection failed: " . $databaseConnection->connect_error);
    }

    $sqlSelect = "SELECT discountCode, discountAmount, discountPercentage, startDate, endDate FROM discountcodes WHERE discountCode = ?";
    $stmt = $databaseConnection->prepare($sqlSelect);

    $stmt->bind_param("s", $discountCode);

    $stmt->execute();

    $result = $stmt->get_result();

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $discountAmount = $row["discountAmount"];
        $discountPercentage = $row["discountPercentage"];
        $discountedprice = 0;
        $startDate = $row["startDate"];
        $endDate = $row["endDate"];
        $currentdate = date("Y-m-d");
        
        if ($currentdate >= $startDate && $currentdate <= $endDate){

        $_SESSION['discount'] = array();
        $_SESSION['discount']['code'] = $discountCode;
        $_SESSION['discount']['discountpercentage'] = $discountPercentage;
        $_SESSION['discount']['discountamount'] = $discountAmount;

        if ($discountPercentage > 0) {
            $discount = $discountPercentage;
            $discountedprice = getTotalPriceCart() * (1 - $discountPercentage);
        } else {
            $discount = $discountAmount;
            $discountedprice = getTotalPriceCart() - $discount;
        }

        return round($discountedprice, 2);
    }
    } else {
        return null;
    }
}
?>  