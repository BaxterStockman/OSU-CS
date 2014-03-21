<?php
// Connect to database
require 'db_connect.php';

// Create variables -- if no new data was entered, set them to NULL.
$p_id = isset($_POST['Name']) ? $_POST['Name'] : NULL;
$p_street = isset($_POST['Street']) ? $_POST['Street'] : NULL;
$p_city = isset($_POST['City']) ? $_POST['City'] : NULL;
$p_state = isset($_POST['State']) ? $_POST['State'] : NULL;
$p_postal_code = isset($_POST['PostalCode']) ? $_POST['PostalCode'] : NULL;
$p_birth_date = isset($_POST['BirthDate']) ? $_POST['BirthDate'] : NULL;
$p_amount = isset($_POST['Amount']) ? $_POST['Amount'] : NULL;
$p_due_date = isset($_POST['DueDate']) ? $_POST['DueDate'] : NULL;
$p_matter = isset($_POST['Matter']) ? $_POST['Matter'] : NULL;

// Check whether client has existing bill
// Create bill query
$get_bill = "SELECT id FROM bill WHERE client_id = ?";
if(!($stmt = $mysqli->prepare($get_bill)))
    echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
if(!($stmt->bind_param("i", $p_id)))
    echo "Bind failed: $mysqli->errno $mysqli->error";
// Execute statement
if(!$stmt->execute())
    echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
// Bind result
if(!$stmt->bind_result($bill_id))
    echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
// Fetch data and echo to screen
// Close statment
$stmt->close();

// All of the following code is conditional, since we don't want it
// to change unless the user entered new values
if($p_street) {
    $update = "UPDATE client
                SET street = ?
                WHERE id = ?";
    runQuery($mysqli, $update, "si", $p_street, $p_id);
}

if($p_city) {
    $update = "UPDATE client
                SET city = ?
                WHERE id = ?";
    runQuery($mysqli, $update, "si", $p_city, $p_id);
}

if($p_state) {
    $update = "UPDATE client
                SET state = ?
                WHERE id = ?";
    runQuery($mysqli, $update, "si", $p_state, $p_id);
}

if($p_postal_code) {
    $update = "UPDATE client
                SET postal_code = ?
                WHERE id = ?";
    runQuery($mysqli, $update, "si", $p_postal_code, $p_id);
}

if($p_birth_date) {
    $update = "UPDATE client
                SET birth_date = ?
                WHERE id = ?";
    runQuery($mysqli, $update, "si", $p_birth_date, $p_id);
}

if($bill_id) {
    if($p_amount) {
        $update = "UPDATE bill
                    SET amount = ?
                    WHERE client_id = ?";
        runQuery($mysqli, $update, "di", $p_amount, $p_id);
    }

    if($p_due_date) {
        $update = "UPDATE bill
                    SET due_date = ?
                    WHERE client_id = ?";
        runQuery($mysqli, $update, "si", $p_due_date, $p_id);
    }
} else {
    if($p_amount) {
        $update = "INSERT INTO bill(client_id, amount, due_date) VALUES (?, ?, ?)";
        runQuery($mysqli, $update, "ids", $p_id, $p_amount, $p_due_date);
    }
}

if($p_matter) {
    $update = "INSERT INTO client_matter(client_id, matter_id) VALUES (?, ?)";
    runQuery($mysqli, $update, "ii", $p_id, $p_matter);
}

// Pause briefly, then return to view_client page
sleep(3);
Redirect('http://web.engr.oregonstate.edu/~schreibm/classes/cs275/project/view_client.php', false);
die();
?>
