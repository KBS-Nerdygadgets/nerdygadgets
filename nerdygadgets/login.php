<?php

include __DIR__ . "/func_header.php";

$error = null;

if(isset($_SESSION['token'])){
    header('Location: categories.php');
}

if(isset($_POST['submit']) && isset($_POST['email']) && isset($_POST['password'])){
    // login request
    if(strlen($_POST['email']) >= 255 || strlen($_POST['password']) >= 255){
        $_SESSION['error'] = "Er zijn teveel karakters ingevoerd op velden, vul het alstublieft opnieuw in";
        $error = true;
    } else{
        $result = login($databaseConnection, $_POST['email'], $_POST['password']);

        if(!$result){ 
            $_SESSION['error'] = "Inloggen mislukt, probeer een andere email en wachtwoord combinatie";
            $error = true;
        } else{ 
            if(isset($_POST['cart'])){
                header('Location: address.php');
            } else{
                header('Location: categories.php');
            }
        }
    }
}

include __DIR__ . "/header.php";

?>

<div class="IndexStyle d-flex justify-content-center" style="margin-top: 30px; width: 40%;">
    <form method="POST" style="width: 100%;">
        <?php if($error && isset($_SESSION['error'])){ ?>
            <div class="alert alert-danger" role="alert">
                <?= $_SESSION['error']; ?>
            </div>
        <?php } ?>
        <div>
            <?php if(isset($_GET['cart'])){ ?>
                <input type="hidden" name="cart" value="<?= $_GET['cart'] ?>">
            <?php } ?>
            <?php if(isset($_POST['cart'])){ ?>
                <input type="hidden" name="cart" value="<?= $_POST['cart'] ?>">
            <?php } ?>
            <label for="email" class="form-label">Email</label>
            <input type="email" class="form-control" id="email" placeholder="Email" maxlength="256" name="email" required>
        </div>   
        <div style="margin-top: 10px;">
            <label for="password" class="form-label">Wachtwoord</label>
            <input type="password" class="form-control" id="password" placeholder="Wachtwoord" minlength="6" maxlength="256" name="password" required>
        </div> 
        <?php echo isset($_GET['cart']) ? '<a style="float: right; margin-top: 1px; color: #fff; text-decoration: none;" href="payment.php">Doorgaan zonder in te loggen</a>' : '' ?>
        <div style="width: 40%; float: right; clear: both; margin-top: 20px;">
            <input type="submit" value="Login" name="submit"/>
            <a style="float: right; margin-top: 10px; color: #fff; text-decoration: none;" href="<?= (isset($_POST['cart']) || isset($_GET['cart'])) ? "register.php?cart=1" : "register.php" ?>">Account aanmaken</a>
        </div>
    </form>
</div>

<?php
include __DIR__ . "/footer.php";
?>

