<?php

function login($databaseConnection, $email, $password){
    // laat de klant inloggen
    $result = false;

    // wachtwoord hashen om te testen met database
    $password = strtoupper(hash('sha256', $password));

    // login checken of het klopt
    $login = getPersonLogin($databaseConnection, $email, $password);

    if($login != null){
        // aanmaken van de token
        $token = createToken($login['PreferredName'], $login['FullName'], $email, $password);

        // het toevoegen van de token in de sessie
        $_SESSION['token'] = $token;

        // login is gelukt en cookie is gezet, dus kan er weer worden gegeven dat er is ingelogd
        $result = true;
    } else{
        // login is niet gelukt dus fout geven
        $result = false;
    }

    return $result;
}

function register($databaseConnection, $firstName, $lastName, $email, $password){
    // laat de bezoeker een klanten account aanmaken
    $result = null;

    // gebruiker aanmaken
    $createUser = createCustomer($databaseConnection, $firstName, $firstName . ' ' . $lastName, '', '', '0000 AM', $email, $password);

    if($createUser !== null){
        $result = login($databaseConnection, $email, $password);
    }

    return $result;
}

function createToken($firstName, $fullName, $email, $password){
    // maken van token, we doen dit in volgorde van params
    // dit encoden we in base64 om nog een beetje beveiliging te hebben
    // het wachtwoord word altijd gestored als hash zodat dit veiliger is!
    $token = '';

    $part = base64_encode($firstName) . ':' . base64_encode($fullName) . ':' . base64_encode($email) . ':' . base64_encode($password);
    $token = base64_encode($part);

    return $token;
}

function verifyToken($databaseConnection, $token){
    // als er een token is gemaakt moet er bevestiging komen of de token klopt bij de gebruiker
    // hiervoor gebruiken we de opgeslagen token en kijken we of deze klopt en aan een account vast zit, anders loggen we de gebruiker uit
    $result = array();
    $result['result'] = false;
    $result['person'] = null;

    // token decoden
    $token = base64_decode($token);

    // token weer terug opsplitsen in alle delen zoals die zit
    $splitToken = explode(':', $token);

    // kijk of de token nog klopt of dat er een andere verkeerde token in staat
    if(isset($splitToken[0]) && isset($splitToken[1]) && isset($splitToken[2]) && isset($splitToken[3])){
        // alles weer decoden
        $firstName = base64_decode($splitToken[0]);
        $fullName = base64_decode($splitToken[1]);
        $email = base64_decode($splitToken[2]);
        $password = base64_decode($splitToken[3]);

        // kijken of de login klopt
        $person = getPersonLogin($databaseConnection, $email, $password);

        // persoon in resultaat zetten zodat eventuele data kan worden gebruikt
        $result['person'] = $person;
        $result['result'] = true;
    }

    return $result;
}

function getPersonLogin($databaseConnection, $email, $password) {
    $Result = null;

    // Bereid het sql statement voor op de uitvoering
    $sql = "SELECT * FROM people WHERE LogonName = ? AND HashedPassword = ? AND LogonName != 'NO LOGON' AND IsSystemUser = 0 AND IsEmployee = 0 AND IsSalesperson = 0 LIMIT 1";
    $stmt = $databaseConnection->prepare($sql);
    $stmt->bind_param("ss", $email, $password);
    $stmt->execute();

    // Krijg het resultaat
    $result = $stmt->get_result();

    // Fetch the row
    $row = $result->fetch_assoc();

    if($row){
        $Result = $row;
    }

    return $Result;
}

?>