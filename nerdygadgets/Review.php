<?php
include __DIR__ . "/header.php";

$itemid = $_GET['id'];
?>

<!DOCTYPE html>
<html lang="nl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Plaats een Review</title>
    <!-- Styling review pagina -->
    <style>
        body {
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
        }

        h1 {
            text-align: center;
        }

        form {
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        .rating {
            display: flex;
            flex-direction: row-reverse; 
            font-size: 40px; 
        }

        .rating input {
            display: none;
        }

        .rating label {
            cursor: pointer;
            margin-right: 10px; 
        }

        .rating label:before {
            content: '\2605'; 
            display: inline-block;
            color: #ccc; 
        }

        .rating input:checked + label:before,
        .rating input:checked ~ input + label:before {
            color: gold; 
        }

        label {
            margin-top: 10px;
            text-align: center;
        }

        textarea {
            width: 400px; 
            height: 150px;
            resize: none; 
        }

        button {
            margin-top: 10px;
        }
    </style>
</head>

<body>
    <h1>Plaats een Review</h1>
    
    <form action="review-submit.php" method="post">
        <div class="rating">
            <!-- Sterren input -->
            <input type="radio" id="rating1" name="rating" value="5" required>
            <label for="rating1"></label>

            <input type="radio" id="rating2" name="rating" value="4">
            <label for="rating2"></label>

            <input type="radio" id="rating3" name="rating" value="3">
            <label for="rating3"></label>

            <input type="radio" id="rating4" name="rating" value="2">
            <label for="rating4"></label>

            <input type="radio" id="rating5" name="rating" value="1">
            <label for="rating5"></label>
        </div>

        <!-- Geschreven review -->
        <label for="reviewText">Review:</label>
        <textarea id="reviewText" name="reviewText" rows="4"></textarea>

        <!-- Stuur het ItemID door naar de db pagina -->
        <input type="hidden" id="idkeeper" name="idkeeper" value='<?=$itemid?>'>

        <!-- Knop verstuurt data naar review-submit.php -->
        <button type="submit">Verstuur Review</button>
    </form>
</body>
</html>

<?php
include __DIR__ . "/footer.php";
?>