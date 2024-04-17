<?php 
include_once __DIR__ . "/func_header.php";
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <title>NerdyGadgets</title>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>

    <!-- Javascript -->
    <script src="Public/JS/fontawesome.js"></script>
    <script src="Public/JS/jquery.min.js"></script>
    <script src="Public/JS/bootstrap.min.js"></script>
    <script src="Public/JS/popper.min.js"></script>
    <script src="Public/JS/resizer.js"></script>

    <link rel="icon" type="image/x-icon" href="/favicon.ico">

    <meta charset="UTF-8">
    <!-- Style sheets-->
    <link rel="stylesheet" href="Public/CSS/style.css" type="text/css">
    <link rel="stylesheet" href="Public/CSS/bootstrap.min.css" type="text/css">
    <link rel="stylesheet" href="Public/CSS/typekit.css">
</head>
<body>
<div class="Background">
<div style="background-color: #5055ba; width: 100%; height: 30px; position: fixed; z-index: 100;">
    <div style="margin-top: 20px;">
        <?php
            if(isset($_SESSION['token'])){
                $verify = verifyToken($databaseConnection, $_SESSION['token']);

                if($verify['result']){
                    ?>
                        <div style="top: 0; position: absolute; right: 0; margin-right: 100px; margin-top: 2px;">Welkom, <?= htmlspecialchars($verify['person']['PreferredName']) ?></div>
                        <div style="top: 0; position: absolute; right: 0; margin-right: 20px; margin-top: 2px;"><a href="logout.php" style="color: #fff; text-decorations: none;">Uitloggen</a></div>
                    <?php
                } else{
                    ?>
                        <div style="top: 0; position: absolute; right: 0; margin-right: 120px; margin-top: 2px;"><a href="login.php" style="color: #fff; text-decorations: none;">Inloggen</a></div>
                        <div style="top: 0; position: absolute; right: 0; margin-right: 20px; margin-top: 2px;"><a href="register.php" style="color: #fff; text-decorations: none;">Registreren</a></div>
                    <?php
                }
            } else{
                ?>
                    <div style="top: 0; position: absolute; right: 0; margin-right: 120px; margin-top: 2px;"><a href="login.php" style="color: #fff; text-decorations: none;">Inloggen</a></div>
                    <div style="top: 0; position: absolute; right: 0; margin-right: 20px; margin-top: 2px;"><a href="register.php" style="color: #fff; text-decorations: none;">Registreren</a></div>
                <?php
            }
        ?>
        
    </div>
</div>
    <div class="row" id="Header">
        <div class="col-2"><a href="./" id="LogoA">
                <div id="LogoImage"></div>
            </a></div>
        <div class="col-8" id="CategoriesBar">
            <ul id="ul-class">
                <?php
                $HeaderStockGroups = getHeaderStockGroups($databaseConnection);

                foreach ($HeaderStockGroups as $HeaderStockGroup) {
                    ?>
                    <li>
                        <a href="browse.php?category_id=<?php print $HeaderStockGroup['StockGroupID']; ?>"
                           class="HrefDecoration"><?php print $HeaderStockGroup['StockGroupName']; ?></a>
                    </li>
                    <?php
                }
                ?>
                <li>
                    <a href="categories.php" class="HrefDecoration">Alle categorieën</a>
                </li>
            </ul>
        </div>
<!-- code voor US3: zoeken -->

        <ul id="ul-class-navigation" style="margin-right: 40px;">
            <li>
                <a href="browse.php" class="HrefDecoration"><i class="fas fa-search search"></i> Zoeken</a>
            </li>
        </ul>

        <ul id="ul-class-navigation">
            <li>
                <a href="cart.php" class="HrefDecoration"><svg xmlns="http://www.w3.org/2000/svg" height="1em" viewBox="0 0 576 512"><!--! Font Awesome Free 6.4.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license (Commercial License) Copyright 2023 Fonticons, Inc. --><style>svg{fill:#ffffff}</style><path d="M0 24C0 10.7 10.7 0 24 0H69.5c22 0 41.5 12.8 50.6 32h411c26.3 0 45.5 25 38.6 50.4l-41 152.3c-8.5 31.4-37 53.3-69.5 53.3H170.7l5.4 28.5c2.2 11.3 12.1 19.5 23.6 19.5H488c13.3 0 24 10.7 24 24s-10.7 24-24 24H199.7c-34.6 0-64.3-24.6-70.7-58.5L77.4 54.5c-.7-3.8-4-6.5-7.9-6.5H24C10.7 48 0 37.3 0 24zM128 464a48 48 0 1 1 96 0 48 48 0 1 1 -96 0zm336-48a48 48 0 1 1 0 96 48 48 0 1 1 0-96z"/></svg></a>
            </li>
        </ul>

<!-- einde code voor US3 zoeken -->
    </div>
    <div class="row" id="Content">
        <div class="col-12">
            <div id="SubContent">
