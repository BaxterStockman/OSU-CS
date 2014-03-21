<?php
// Connect to database
require 'db_connect.php';

// Create variables -- if no new data was entered, set them to NULL.
$p_court = isset($_POST['Court']) ? $_POST['Court'] : NULL;
$p_style = isset($_POST['MatterName']) ? $_POST['MatterName'] : NULL;
$p_type = isset($_POST['MatterType']) ? $_POST['MatterType'] : NULL;
$p_date = isset($_POST['FiledDate']) ? $_POST['FiledDate'] : NULL;
$att_checkbox = isset($_POST['att_checkbox']) ? $_POST['att_checkbox'] : NULL;
$att_check_count = count($att_checkbox);   // contains count of checked boxes
$cli_checkbox = isset($_POST['cli_checkbox']) ? $_POST['cli_checkbox'] : NULL;
$cli_check_count = count($cli_checkbox);   // contains count of checked boxes

// Make sure that the user enters at least some basic information
if(!($p_style || $p_att_checkbox || $p_cli_checkbox))
    die("Missing essential matter information; please try again.");

// Insert values into 'matter' table
$insert = "INSERT INTO matter(court_id, style, matter_type, filed_date) VALUES (?, ?, ?, ?)";
runQuery($mysqli, $insert, "isss", $p_court, $p_style, $p_type, $p_date);

// Get ID of last insertion
$p_id = $mysqli->insert_id;

// Loop through checked boxes, adding links for cases that were checked
for($i = 0; $i < $att_check_count; $i++) {
    $add_id = $att_checkbox[$i];
    echo $add_id;
    $query = "INSERT INTO attorney_matter(attorney_id, matter_id) VALUES (?, ?)";
    runQuery($mysqli, $query, "ii", $add_id, $p_id);
}

// Loop through checked boxes, adding links for cases that were checked
for($i = 0; $i < $cli_check_count; $i++) {
    $add_id = $cli_checkbox[$i];
    $query = "INSERT INTO client_matter(client_id, matter_id) VALUES (?, ?)";
    runQuery($mysqli, $query, "ii", $add_id, $p_id);
}

// Pause briefly, then return to view_matter page
sleep(3);
Redirect('http://web.engr.oregonstate.edu/~schreibm/classes/cs275/project/view_matter.php', false);
die();
?>
