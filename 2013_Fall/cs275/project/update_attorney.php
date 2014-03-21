<?php
// Connect to database
require 'db_connect.php';

// Create variables
$checkbox = isset($_POST['checkbox']) ? $_POST['checkbox'] : NULL; // contains check boxes
$check_count = count($checkbox);   // contains count of checked boxes
$p_id = $_POST['Name'];
$p_bar_number = isset($_POST['BarNumber']) ? $_POST['BarNumber'] : NULL;

// If user set bar number, update it
// This is here to prevent overwriting the bar number if the user doesn't enter anything
if($p_bar_number) {
    $update = "UPDATE attorney
                SET bar_number = ?
                WHERE id = ?";
    runQuery($mysqli, $update, "si", $p_bar_number, $p_id);
}

// For all cases the user selected, add link to 'attorney_matter' table
if($checkbox) {
    for($i = 0; $i < $check_count; $i++) {
        $add_id = $checkbox[$i];
        $query = "INSERT INTO attorney_matter(attorney_id, matter_id) VALUES (?, ?)";
        runQuery($mysqli, $query, "ii", $p_id, $add_id);
    }
}

// Pause briefly, then return to view_attorney page
sleep(3);
Redirect('http://web.engr.oregonstate.edu/~schreibm/classes/cs275/project/view_attorney.php', false);
die();
?>
