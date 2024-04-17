<?php
include __DIR__ . "/func_header.php";

$error = false;

if(isset($_POST['submit']) && isset($_POST['firstname']) && isset($_POST['lastname']) && isset($_POST['email']) && isset($_POST['password'])){
    // register request
    if(strlen($_POST['firstname']) >= 256 || strlen($_POST['lastname']) >= 256 || strlen($_POST['email']) >= 256 || strlen($_POST['password']) >= 256 || strlen($_POST['firstname']) <= 0 || strlen($_POST['lastname']) <= 0 || strlen($_POST['email']) <= 0 || strlen($_POST['password']) <= 0){
        $_SESSION['error'] = "Er zijn teveel karakters ingevoerd op velden, vul het alstublieft opnieuw in";
        $error = true;
    } else{
        $createUser = register($databaseConnection, $_POST['firstname'], $_POST['lastname'], $_POST['email'], $_POST['password']);

        if($createUser !== null){
            // kon de gebruiker maken en is geingelogd
            if(isset($_POST['cart'])){
                header('Location: address.php');
            } else{
                header('Location: categories.php');
            }
        } else{
            // kon gebruiken niet maken dus error
            $error = true;
        }
    }
}

include __DIR__ . "/header.php";

?>

<div class="IndexStyle d-flex justify-content-center" style="margin-top: 30px; width: 40%;">
    <form method="POST" style="width: 100%;">
        <?php if($error && isset($_SESSION['error'])){ ?>
            <div class="alert alert-danger" role="alert">
                <?= $_SESSION['error'] ?>
            </div>
        <?php } ?>
        <?php if(isset($_GET['cart'])){ ?>
                <input type="hidden" name="cart" value="<?= $_GET['cart'] ?>">
        <?php } ?>
        <?php if(isset($_POST['cart'])){ ?>
            <input type="hidden" name="cart" value="<?= $_POST['cart'] ?>">
        <?php } ?>
        <div>
            <label for="firstname" class="form-label">Voornaam</label>
            <input type="text" class="form-control" id="firstname" placeholder="Voornaam" maxlength="256" name="firstname" required>
        </div> 
        <div style="margin-top: 10px;">
            <label for="lastname" class="form-label">Achternaam</label>
            <input type="text" class="form-control" id="lastname" placeholder="Achternaam" maxlength="256" name="lastname" required>
        </div> 
        <div style="margin-top: 10px;">
            <label for="email" class="form-label">Email</label>
            <input type="email" class="form-control" id="email" placeholder="Email" maxlength="256" name="email" required>
        </div>   
        <div style="margin-top: 10px;">
            <label for="password" class="form-label">Wachtwoord</label>
            <input type="password" class="form-control" id="password" placeholder="Wachtwoord" minlength="6" maxlength="256" name="password" required>
        </div> 
        <div style="width: 40%; float: right; clear: both; margin-top: 20px;">
            <input type="submit" value="Registreren" name="submit"/>
            <a style="float: right; margin-top: 10px; color: #fff; text-decoration: none;" href=".">Terug naar winkel</a>
        </div>
    </form>
</div>
<?php
include __DIR__ . "/footer.php";
?>

