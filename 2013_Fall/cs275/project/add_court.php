<?php
// Connect to database
require 'db_connect.php';

// Create variables -- if no new data was entered, set them to NULL.
$p_name = isset($_POST['Name']) ? $_POST['Name'] : NULL;
$p_street = isset($_POST['Street']) ? $_POST['Street'] : NULL;
$p_city = isset($_POST['City']) ? $_POST['City'] : NULL;
$p_state = isset($_POST['State']) ? $_POST['State'] : NULL;
$p_postal_code = isset($_POST['PostalCode']) ? $_POST['PostalCode'] : NULL;

// Insert values into 'court' table
$update = "INSERT INTO court(name, street, city, state, postal_code) VALUES (?, ?, ?, ?, ?)";
runQuery($mysqli, $update, "ssssi", $p_name, $p_street, $p_city, $p_state, $p_postal_code);

// Pause briefly, then return to view_matter page
sleep(3);
Redirect('http://web.engr.oregonstate.edu/~schreibm/classes/cs275/project/view_matter.php', false);
die();
?>
