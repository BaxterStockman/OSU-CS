<?php
//Turn on error reporting
ini_set('display_errors', 'On');

// Set variables
$DB_NAME = 'schreibm-db';
$DB_HOST = 'oniddb.cws.oregonstate.edu';
$DB_USER = 'schreibm-db';
$DB_PASS = 'lJZQkchPXbxVt6Af';

// Connect to the database
$mysqli = new mysqli("$DB_HOST","$DB_USER","$DB_PASS","$DB_NAME")
    or die("Connection error: $mysqli->connect_errno $mysqli->connect_error");

// From user Alix Axel answer on
// http://stackoverflow.com/questions/768431/how-to-make-a-redirect-in-php
function Redirect($url, $permanent = false) {
    if(headers_sent() === false)
        header("Location: $url", true, ($permanent === true) ? 301 : 302);
    exit();
}

// Encapsulates the operations involved in
// writing information to the database
function runQuery($db, $query) {
    $args = array_slice(func_get_args(), 2);
    $arg_len = count($args);
    for($i = 0; $i < $arg_len; $i++)
        $args[$i] = &$args[$i];
    // Prepare statement
    if(!($stmt = $db->prepare($query)))
        echo "Prepare failed: $db->errno $db->error", PHP_EOL;
    // Bind parameters to '?' placedholders
    if(!(call_user_func_array(array($stmt, "bind_param"), $args)))
        echo "Bind failed: $db->errno $db->error", PHP_EOL;
    // Execute statement
    if(!$stmt->execute())
        echo "Execute failed: $db->errno $db->error", PHP_EOL;
    // Print success message
    else
        echo "Changed $stmt->affected_rows rows successfully.\n", PHP_EOL;
    $stmt->close();
}

// Couldn't get this to work
function bindVars($db, &$stmt, $query) {
    $args = array_slice(func_get_args(), 3);
    $arg_len = count($args);
    for($i = 0; $i < $arg_len; $i++)
        $args[$i] = &$args[$i];
    // Prepare statement
    if(!($stmt = $db->prepare($query)))
        echo "Prepare failed: $db->errno $db->error", PHP_EOL;
    // Execute statement
    if(!$stmt->execute())
        echo "Execute failed: $db->errno $db->error", PHP_EOL;
    // Bind parameters to variables passed in
    if(!(call_user_func_array(array($stmt, "bind_result"), $args)))
        echo "Bind failed: $db->errno $db->error", PHP_EOL;
}

?>
