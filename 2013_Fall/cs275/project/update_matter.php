<?php
// Connect to database
require 'db_connect.php';

// Create variables -- if no new data was entered, set them to NULL.
$p_id = isset($_POST['Name']) ? $_POST['Name'] : NULL;
$p_doc_type = isset($_POST['DocType']) ? $_POST['DocType'] : NULL;
$p_filed_date = isset($_POST['FiledDate']) ? $_POST['FiledDate'] : NULL;
$p_hearing_type = isset($_POST['HearingType']) ? $_POST['HearingType'] : NULL;
$p_hearing_date = isset($_POST['HearingDate']) ? $_POST['HearingDate'] : NULL;
$p_street = isset($_POST['Street']) ? $_POST['Street'] : NULL;
$p_city = isset($_POST['City']) ? $_POST['City'] : NULL;
$p_state = isset($_POST['State']) ? $_POST['State'] : NULL;
$p_postal_code = isset($_POST['PostalCode']) ? $_POST['PostalCode'] : NULL;
$p_court = isset($_POST['Court']) ? $_POST['Court'] : NULL;

// If the user entered a document, insert values into 'document' table
if($p_doc_type) {
    $update = "INSERT INTO document(matter_id, doc_type, filed_date) VALUES(?, ?, ?)";
    runQuery($mysqli, $update, "iss", $p_id, $p_doc_type, $p_filed_date);
}

// If user chose to associate hearing with a court, update court_id value along with other values.
// Otherwise ignore the court_id value.
if($p_court) {
    if($p_hearing_type) {
        $update = "INSERT INTO hearing(matter_id, subject, hearing_date, street, city, state, postal_code, court_id)
                    VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
        runQuery($mysqli, $update, "isssssii", $p_id, $p_hearing_type, $p_hearing_date, $p_street, $p_city, $p_state, $p_postal_code, $p_court);
    }
} else {
    if($p_hearing_type) {
        $update = "INSERT INTO hearing(matter_id, subject, hearing_date, street, city, state, postal_code)
                    VALUES(?, ?, ?, ?, ?, ?, ?)";
        runQuery($mysqli, $update, "isssssi", $p_id, $p_hearing_type, $p_hearing_date, $p_street, $p_city, $p_state, $p_postal_code);
    }
}

// Pause briefly, then return to view_matter page
sleep(3);
Redirect('http://web.engr.oregonstate.edu/~schreibm/classes/cs275/project/view_matter.php', false);
die();
?>
