<?php
// Connect to database
require 'db_connect.php';

// Create variables
$checkbox = $_POST['checkbox'];             // contains check boxes
$check_count = count($_POST['checkbox']);   // contains count of checked boxes
$p_bar_number = $_POST['BarNumber'];
$p_fname = $_POST['FirstName'];
$p_lname = $_POST['LastName'];

// Prepare statement to check whether attorney exists
$check = "SELECT bar_number FROM attorney WHERE bar_number = '$p_bar_number'";
if(!($stmt = $mysqli->prepare($check)))
    echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;

// Execute statement
if(!$stmt->execute())
    echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;

// Bind results
if(!$stmt->bind_result($exists_bar_number))
    echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;

$stmt->close();

// Check to see whether attorney already exists; if not, create attributes.
if(!$exists_bar_number) {
    $insert = "INSERT INTO attorney(fname, lname, bar_number) VALUES (?, ?, ?)";
    runQuery($mysqli, $insert, "sss", $p_fname,$p_lname,$p_bar_number);

    $p_id = $mysqli->insert_id;

    // Loop through checked boxes, adding links for cases that were checked
    for($i = 0; $i < $check_count; $i++) {
        $add_id = $checkbox[$i];
        $query = "INSERT INTO attorney_matter(attorney_id, matter_id) VALUES (?, ?)";
        runQuery($mysqli, $query, "is", $p_id, $add_id);
    }
} else {
// If attorney already exists, tell user to use 'Update' instead
die ("$p_fname $p_lname already exists in database; please use 'Update' function instead.");
}

// Pause briefly, then return to view_attorney page
sleep(3);
Redirect('http://web.engr.oregonstate.edu/~schreibm/classes/cs275/project/view_attorney.php', false);
die();
?>
