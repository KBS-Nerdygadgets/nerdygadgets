<?php
include __DIR__ . "/header.php";
include __DIR__ ."/functions/temperature.php";
INCLUDE __DIR__ ."/functions/recordedwhen.php";

$StockItem = getStockItem($_GET['id'], $databaseConnection);
$StockItemImage = getStockItemImage($_GET['id'], $databaseConnection);

if(isset($_POST['amount']) && isset($_GET['id'])){
    addToCart(intval($_GET['id']), intval($_POST['amount']), $StockItem, $StockItemImage);
}

// Reviews database connectie
$itemId = $_GET['id'];
$queryReviews = "SELECT * FROM reviews WHERE ItemID = $itemId";
$resultReviews = mysqli_query($databaseConnection, $queryReviews);

?>
<div id="CenteredContent">
    <?php
    if ($StockItem != null) {
        ?>
        <?php
        if (isset($StockItem['Video'])) {
            ?>
            <div id="VideoFrame">
                <?php print $StockItem['Video']; ?>
            </div>
        <?php }
        ?>


        <div id="ArticleHeader">
            <?php
            if (isset($StockItemImage)) {
                // één plaatje laten zien
                if (count($StockItemImage) == 1) {
                    ?>
                    <div id="ImageFrame"
                         style="background-image: url('Public/StockItemIMG/<?php print $StockItemImage[0]['ImagePath']; ?>'); background-size: 300px; background-repeat: no-repeat; background-position: center;"></div>
                    <?php
                } else if (count($StockItemImage) >= 2) { ?>
                    <!-- meerdere plaatjes laten zien -->
                    <div id="ImageFrame">
                        <div id="ImageCarousel" class="carousel slide" data-interval="false">
                            <!-- Indicators -->
                            <ul class="carousel-indicators">
                                <?php for ($i = 0; $i < count($StockItemImage); $i++) {
                                    ?>
                                    <li data-target="#ImageCarousel"
                                        data-slide-to="<?php print $i ?>" <?php print (($i == 0) ? 'class="active"' : ''); ?>></li>
                                    <?php
                                } ?>
                            </ul>

                            <!-- slideshow -->
                            <div class="carousel-inner">
                                <?php for ($i = 0; $i < count($StockItemImage); $i++) {
                                    ?>
                                    <div class="carousel-item <?php print ($i == 0) ? 'active' : ''; ?>">
                                        <img src="Public/StockItemIMG/<?php print $StockItemImage[$i]['ImagePath'] ?>">
                                    </div>
                                <?php } ?>
                            </div>

                            <!-- knoppen 'vorige' en 'volgende' -->
                            <a class="carousel-control-prev" href="#ImageCarousel" data-slide="prev">
                                <span class="carousel-control-prev-icon"></span>
                            </a>
                            <a class="carousel-control-next" href="#ImageCarousel" data-slide="next">
                                <span class="carousel-control-next-icon"></span>
                            </a>
                        </div>
                    </div>
                    <?php
                }
            } else {
                ?>
                <div id="ImageFrame"
                     style="background-image: url('Public/StockGroupIMG/<?php print $StockItem['BackupImagePath']; ?>'); background-size: cover;"></div>
                <?php
            }
            ?>


            <h1 class="StockItemID">Artikelnummer: <?php print $StockItem["StockItemID"]; ?></h1>
            <h2 class="StockItemNameViewSize StockItemName">
                <?php print $StockItem['StockItemName']; ?>
            </h2>
            <div class="QuantityText"><?php print $StockItem['QuantityOnHand'] . (getItemAmountCart($_GET['id']) !== 0 ? " (" . getItemAmountCart($_GET['id']) . " in winkelmand)" : ""); ?></div>
            <div id="StockItemHeaderLeft">
                <div class="CenterPriceLeft">
                    <div class="CenterPriceLeftChild">
                        <p class="StockItemPriceText"><b><?php print sprintf("€ %.2f", $StockItem['SellPrice']); ?></b></p>
                        <h6 style="margin-top: -20px;"> Inclusief BTW </h6>
                    </div>
                </div>
            </div>
            <form method="POST">
                <input type="submit" style="float: right; margin-top: 120px; width: 220px; height: 30px; z-index: 999 !important; position: relative;" value="Voeg toe aan winkelmand">
                <input style="float: right; margin-top: 120px; z-index: 999 !important; position: relative; width: 70px; height: 30px; margin-right: 10px;" id="amountProducts" type="number" value="1" min="1" max="<?= intval(str_replace("Voorraad: ", "", $StockItem['QuantityOnHand'])) ?>" name="amount">
                <input style="display: none" name="id" value="<?= $_GET['id'] ?>">
            </form>
        </div>
        <div id="StockItemDescription">
            <h3>Artikel beschrijving</h3>
            <p><?php print $StockItem['SearchDetails']; ?></p>
        </div>
        <div id="StockItemSpecifications">
            <h3>Artikel specificaties</h3>
            <?php
            $CustomFields = json_decode($StockItem['CustomFields'], true);
            if (is_array($CustomFields)) { ?>
                <table>
                <thead>
                <th>Naam</th>
                <th>Data</th>
                </thead>
                <?php
                foreach ($CustomFields as $SpecName => $SpecText) { ?>
                    <tr>
                        <td>
                            <?php print $SpecName; ?>
                        </td>
                        <td>
                            <?php
                            if ($SpecName === 'Temperature') {
                                updateTemperature($databaseConnection, 5);
                                ?> <div id="Temperature"></div> <?php
                            } elseif ($SpecName === 'RecordedWhen') {
                                updateRecordedWhen($databaseConnection, 5);
                                ?> <div id="RecordedWhen"></div> <?php
                            } elseif (is_array($SpecText)) {
                                    foreach ($SpecText as $SubText) {
                                        print $SubText . " ";
                                    }
                                } else {
                                    print $SpecText;
                                }
                            ?>
                        </td>
                    </tr>
                <?php } ?>
                </table><?php
            } else { ?>

                <p><?php print $StockItem['CustomFields']; ?>.</p>
                <?php
            }
            ?>
        </div>

        <!-- Review knop -->
        <div>
        <a href="review.php?id=<?= isset($_GET['id']) ? $_GET['id'] : '' ?>">
            <button style="width: 20%; float: right; clear: both; margin-right: 20px; margin-top: 10px;" onclick="">Review achter laten</button>
        </a> 
        </div>

        <!-- Styling van reviews -->
        <style>
            #ProductReviews {
                margin-top: 20px;
                padding: 20px;
            }

            .review-box {
                margin-bottom: 20px;
                padding: 15px;
                border: 1px solid #ddd;
                border-radius: 8px;
                position: relative;
            }

            .review-rating {
                font-size: 24px; 
                font-weight: bold;
                color: white;
                background-color: #333; 
                padding: 5px 10px;
                border: 2px solid #fff; 
                border-radius: 5px;
                position: absolute;
                top: 15px;
                right: 15px;
            }

            .review-text {
                margin-bottom: 10px;
            }

            .review-date {
                font-style: italic;
                color: #666;
            }
        </style>

        <!-- Inhoud van een review box -->
        <div id="ProductReviews">
            <h3>Product Reviews</h3>
            <?php
            while ($rowReview = mysqli_fetch_assoc($resultReviews)) {
                $reviewText = htmlspecialchars($rowReview['Text']);
                $reviewGrade = htmlspecialchars($rowReview['Grade']);
                $reviewDate = htmlspecialchars($rowReview['Date']);

                echo "<div class='review-box'>
                        <p class='review-text'>$reviewText</p>
                        <p class='review-rating'>$reviewGrade/5</p>
                        <p class='review-date'>Date: $reviewDate</p>
                    </div>";
            }
            ?>
        </div>

        <?php
    } else {
        ?><h2 id="ProductNotFound">Het opgevraagde product is niet gevonden.</h2><?php
    } ?>
</div>

<script>

async function getTemperature(){
    var temperature = 0;

    // ophalen webrequest ophalen van temperatuur
    $.get( "api/temperature.php?id=<?= $_GET['id'] ?>", function(data) {
        // op pagina zetten van de temperature (in de DIV die als ID Temperature heeft)
        document.getElementById("Temperature").innerHTML = parseFloat(data).toFixed(2) + "°C";
    });
}

getTemperature();
setInterval(getTemperature, 3000);

</script>

<script>

async function getDateByItem(){
    var date = 0;

    // ophalen webrequest ophalen van de datum
    $.get( "api/recordedwhen.php?id=<?= $_GET['id'] ?>", function(data) {
        // op pagina zetten van de datum (in de DIV die als ID Datum heeft)
        document.getElementById("RecordedWhen").innerHTML = data;
    });
}

getDateByItem();
setInterval(getDateByItem, 3000);

</script>


