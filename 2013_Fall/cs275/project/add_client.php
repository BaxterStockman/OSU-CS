<?php
// Connect to database
require 'db_connect.php';

// Create variables -- if no new data was entered, set them to NULL.
$p_fname = isset($_POST['FirstName']) ? $_POST['FirstName'] : NULL;
$p_lname = isset($_POST['LastName']) ? $_POST['LastName'] : NULL;
$p_street = isset($_POST['Street']) ? $_POST['Street'] : NULL;
$p_city = isset($_POST['City']) ? $_POST['City'] : NULL;
$p_state = isset($_POST['State']) ? $_POST['State'] : NULL;
$p_postal_code = isset($_POST['PostalCode']) ? $_POST['PostalCode'] : NULL;
$p_birth_date = isset($_POST['BirthDate']) ? $_POST['BirthDate'] : NULL;
$p_matter = isset($_POST['Matter']) ? $_POST['Matter'] : NULL;

// Make sure user enters at least some basic information
if(!($p_fname || $p_lname))
    die("Missing essential client information: name");

// Insert values into 'client' table
$insert = "INSERT INTO client(fname, lname, birth_date, street, city, state, postal_code) VALUES (?, ?, ?, ?, ?, ?, ?)";
runQuery($mysqli, $insert, "ssssssi", $p_fname, $p_lname, $p_birth_date, $p_street, $p_city, $p_state, $p_postal_code);

// Get ID of last insertion
$p_id = $mysqli->insert_id;

// If user chose to associate client to a matter, add link to client_matter table
if($p_matter) {
    $insert = "INSERT INTO client_matter(client_id, matter_id) VALUES (?, ?)";
    runQuery($mysqli, $insert, "ii", $p_id, $p_matter);
}

// Pause briefly, then return to view_client page
sleep(3);
Redirect('http://web.engr.oregonstate.edu/~schreibm/classes/cs275/project/view_client.php', false);
die();
?>
