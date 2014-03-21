<?php
require 'db_connect.php';
$p_id = $_POST['Name'];
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<body>
<div>
    <h3>Matter details</h3>
    <table>
        <thead>
            <tr>
                <th>Court</th>
                <th>Matter Type</th>
                <th>Filed Date</th>
            </tr>
        </thead>
<?php
        // Prepare statement to display details of matter
        $details = "SELECT mat.court_id, court.name, matter_type, filed_date
                        FROM matter mat
                        INNER JOIN court
                        ON mat.court_id = court.id
                        WHERE mat.id = ?";
        if(!($stmt = $mysqli->prepare($details)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i", $p_id)))
            echo "Bind failed: $mysqli->errno $mysqli->error";
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($court_id, $court_name, $matter_type, $filed_date))
            echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
        while($stmt->fetch())
            echo "<tr><td>$court_name</td><td>$matter_type</td><td>$filed_date</td></tr>";
        $stmt->close();
?>
    </table>
</div>

<div>
    <h3>Court Address</h3>
    <table>
        <thead>
            <tr>
                <th>Street</th>
                <th>City</th>
                <th>State</th>
                <th>Postal Code</th>
            </tr>
        </thead>
<?php
        // Prepare statement to display address of Court associated with matter
        $court_address = "SELECT street, city, state, postal_code FROM court WHERE id = ?";
        if(!($stmt = $mysqli->prepare($court_address)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i", $p_id)))
            echo "Bind failed: $mysqli->errno $mysqli->error";
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($street, $city, $state, $postal_code))
            echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
        while($stmt->fetch())
            echo "<tr><td>$street</td><td>$city</td><td>$state</td><td>$postal_code</td></tr>";
        $stmt->close();
?>
    </table>
<div>

<div>
    <h3>Hearings</h3>
    <table>
        <thead>
            <tr>
                <th>Subject</th>
                <th>Date</th>
                <th>Street</th>
                <th>City</th>
                <th>State</th>
                <th>Postal Code</th>
            </tr>
        </thead>
<?php
        // Prepare statement to display hearings associated with matter
        $hearings = "SELECT subject, hearing_date, street, city, state, postal_code FROM hearing WHERE matter_id = ? ORDER BY hearing_date ASC";
        if(!($stmt = $mysqli->prepare($hearings)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i", $p_id)))
            echo "Bind failed: $mysqli->errno $mysqli->error";
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($subject, $hearing_date, $street, $city, $state, $postal_code))
            echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
        while($stmt->fetch())
            echo "<tr><td>$subject</td><td>$hearing_date</td><td>$street</td><td>$city</td>
                    <td>$state</td><td>$postal_code</td></tr>";
        $stmt->close();
?>
    </table>
<div>

<div>
    <h3>Documents</h3>
    <table>
        <thead>
            <tr>
                <th>Type</th>
                <th>Filed Date</th>
            </tr>
        </thead>
<?php
        // Prepare statement to display documents associated with matter
        $documents = "SELECT doc_type, filed_date FROM document WHERE matter_id = ? ORDER BY filed_date ASC";
        if(!($stmt = $mysqli->prepare($documents)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i", $p_id)))
            echo "Bind failed: $mysqli->errno $mysqli->error";
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($doc_type, $filed_date))
            echo "Bind result failed: $mysqli->errno $mysqli->error", PHP_EOL;
        while($stmt->fetch())
            echo "<tr><td>$doc_type</td><td>$filed_date</td></tr>";
        $stmt->close();
?>
    </table>
<div>
</body>
</html>
