<?php
require 'db_connect.php';
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<body>
<div>
    <h3>Client details</h3>
    <table>
        <thead>
            <tr>
                <th>Name</th>
                <th>Gender</th>
                <th>Birthdate</th>
                <th>Address</th>
                <th>Matter Name</th>
            </tr>
        </thead>
<?php
        $p_id = $_POST['Name'];
        $client = "SELECT fname, lname, gender, birth_date, street, city, state, postal_code, mat.style
                        FROM client cli
                        INNER JOIN client_matter cli_mat
                        ON cli.id = cli_mat.client_id
                        INNER JOIN matter mat
                        ON cli_mat.matter_id = mat.id
                        WHERE cli.id = ?";
        // Prepare statement to display client details
        if(!($stmt = $mysqli->prepare($client)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i", $p_id)))
            echo "Bind failed: $mysqli->errno $mysqli->error";
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($fname, $lname, $gender, $birth_date, $street, $city, $state, $postal_code, $style))
            echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
        $address = "$street\n$city, $state $postal_code";
        while($stmt->fetch())
            echo "<tr><td>$lname,  $fname</td><td>$gender</td><td>$birth_date</td>" .
                    "<td>$address</td><td>$style</td></tr>", PHP_EOL;
        $stmt->close();
?>
    </table>
</div>

<div>
    <h3>Client bill</h3>
    <table>
        <thead>
            <tr>
                <th>Amount</th>
                <th>Due Date</th>
            </tr>
        </thead>
<?php
        // Prepare statement to display client's bill
        $bill = "SELECT amount, due_date FROM bill WHERE client_id = ?";
        if(!($stmt = $mysqli->prepare($bill)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i", $p_id)))
            echo "Bind failed: $mysqli->errno $mysqli->error";
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($amount, $due_date))
            echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
        while($stmt->fetch())
            echo "<tr><td>$amount</td><td>$due_date</td></tr>";
        $stmt->close();
?>
    </table>
<div>
</body>
</html>
