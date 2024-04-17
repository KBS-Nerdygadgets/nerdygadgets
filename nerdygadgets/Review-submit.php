<?php
include __DIR__ . "/header.php";

    // Haalt info van de review op
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $rating = $_POST['rating'];
    $text = $_POST['reviewText'];
    $itid = $_POST['idkeeper'];

    // Database connectie
    $db = new mysqli("localhost", "root", "", "nerdygadgets");

    if ($db->connect_error) {
        die("Connection failed: " . $db->connect_error);
    }

    // Versie 1
        // $query = "INSERT INTO reviews (Text, Grade, ItemID) VALUES ('$text', '$rating', '$itid')";

        // if ($db->query($query) === TRUE) {
        //     echo "Bedankt voor uw review!";
        // } else {
        //     echo "Fout: " . $query . "<br>" . $db->error;
        // }

    // Versie 2
    $query = $db->prepare("INSERT INTO reviews (Text, Grade, ItemID) VALUES (?, ?, ?)");

    // Prepared statement voor het vorkomen van SQL injections
    if ($query) {
        $query->bind_param("ssi", $text, $rating, $itid);

        if ($query->execute()) {
            echo "<div style='text-align: center; margin-top: 20px;'>";
            echo "<h2>Bedankt voor uw review!</h2>";
            echo "<meta http-equiv='refresh' content='2;url=view.php?id=$itid'>";
            echo "</div>";
        } else {
            echo "<div style='text-align: center; margin-top: 20px; color: red;'>";
            echo "<p>Fout: " . $query->error . "</p>";
            echo "<meta http-equiv='refresh' content='2;url=view.php?id=$itid'>";
            echo "</div>";
        }

        $query->close();
    } else {
        echo "<div style='text-align: center; margin-top: 20px; color: red;'>";
        echo "<p>Fout bij het voorbereiden van de query.</p>";
        echo "<meta http-equiv='refresh' content='2;url=view.php?id=$itid'>";
        echo "</div>";
    }

    $db->close();
} else {
    echo "<div style='text-align: center; margin-top: 20px; color: red;'>";
    echo "<p>Ongeldige aanvraag!</p>";
    echo "<meta http-equiv='refresh' content='2;url=view.php?id=$itid'>";
    echo "</div>";
}

include __DIR__ . "/footer.php";
?>
