<!-- Connect to database containing content for webpage -->
<?php
    require 'db_connect.php';
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<body>
<div>
    <table>
        <tr>
            <td><a href="view_attorney.php">Attorneys</a></td>
            <td><a href="view_client.php">Clients</a></td>
            <td><a href="view_matter.php">Matters</a></td>
        </tr>
    </table>
</div>
<div>
    <h3>Current clients</h3>
    <table>
        <?php
        // Prepare statement for getting client names
        $clients = "SELECT fname, lname FROM client ORDER BY lname, fname ASC";
        $stmt = $mysqli->prepare($clients);
        if(!$stmt)
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($fname, $lname))
            echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
        while($stmt->fetch())
            echo "<tr><td>$lname,  $fname</td></tr>", PHP_EOL;
        $stmt->close();
        ?>
    </table>
</div>

<div>
    <form method="post" action="details_client.php">
        <fieldset>
            <legend>Get Details on a Client</legend>
            <select name="Name">
                <option value="">Select...</option>
                <?php
                // Prepare statement for getting client names
                $clients = "SELECT id, fname, lname FROM client ORDER BY lname, fname ASC";
                if(!($stmt = $mysqli->prepare($clients)))
                    echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->execute())
                    echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->bind_result($id, $fname, $lname))
                    echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
                while($stmt->fetch())
                    echo "<option value=$id>$lname, $fname</option>\n", PHP_EOL;
                $stmt->close();
                ?>
            </select>
        </fieldset>
        <p><input type="submit" value="Get Details" /></p>
    </form>
</div>

<div>
    <form method="post" action="filter_client.php">
        <fieldset>
            <legend>Filter by Case</legend>
                <select name="Case">
                    <option value="">Select...</option>
                    <?php
                    // Prepare statement for getting case names
                    $cases = "SELECT id, style FROM matter ORDER BY style ASC";
                    if(!($stmt = $mysqli->prepare($cases)))
                        echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    if(!$stmt->bind_result($id, $style))
                        echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    while($stmt->fetch())
                     echo "<option value=$id>$style</option>\n", PHP_EOL;
                    $stmt->close();
                    ?>
                </select>
        </fieldset>
        <p><input type="submit" value="Run Filter" /></p>
    </form>
</div>

<div>
    <h3>Add a client</h3>
    <form method="post" action="add_client.php">
        <fieldset>
            <legend>Name</legend>
            <p>First Name: <input type="text" name="FirstName" /></p>
            <p>Last Name: <input type="text" name="LastName" /></p>
        </fieldset>

        <fieldset>
            <legend>Address</legend>
            <p>Street: <input type="text" name="Street" /></p>
            <p>City: <input type="text" name="City" /></p>
            <p>State: <input type="text" name="State" /></p>
            <p>Postal Code: <input type="text" name="PostalCode" /></p>
        </fieldset>

        <fieldset>
            <legend>Birthdate</legend>
            <p>Birthdate (YYYY-MM-DD): <input type="date" name="BirthDate" /></p>
        </fieldset>

        <fieldset>
            <legend>Case Assignment</legend>
                <select name="Matter">
                    <option value="">Select...</option>
                    <?php
                    // Prepare statement for getting case names
                    $cases = "SELECT id, style FROM matter ORDER BY style ASC";
                    if(!($stmt = $mysqli->prepare($cases)))
                        echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    if(!$stmt->bind_result($matter_id, $style))
                        echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    while($stmt->fetch())
                        echo "<option value=$matter_id>$style</option>\n", PHP_EOL;
                    $stmt->close();
                    ?>
                </select>
        </fieldset>
        <p><input id='add_client' type="submit" class='button' name='add_client' value='Add client'/></p>
    </form>
</div>

<div>
    <h3>Update a client</h3>
    <form method="post" action="update_client.php">
        <fieldset>
            <legend>Name</legend>
            <select name="Name">
                <option value="">Select...</option>
                <?php
                // Prepare statement for getting client names
                $clients = "SELECT id, fname, lname FROM client ORDER BY lname, fname ASC";
                if(!($stmt = $mysqli->prepare($clients)))
                    echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->execute())
                    echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->bind_result($id, $fname, $lname))
                    echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
                while($stmt->fetch())
                    echo "<option value=$id>$lname, $fname</option>\n", PHP_EOL;
                $stmt->close();
                ?>
            </select>
        </fieldset>

        <fieldset>
            <legend>Address</legend>
            <p>Street: <input type="text" name="Street" /></p>
            <p>City: <input type="text" name="City" /></p>
            <p>State: <input type="text" name="State" /></p>
            <p>Postal Code: <input type="text" name="PostalCode" /></p>
        </fieldset>

        <fieldset>
            <legend>Birthdate</legend>
            <p>Birthdate (YYYY-MM-DD): <input type="date" name="BirthDate" /></p>
        </fieldset>

        <fieldset>
            <legend>Case Assignment</legend>
                <select name="Matter">
                    <option value="">Select...</option>
                    <?php
                    // Prepare statement for getting case names
                    $cases = "SELECT id, style FROM matter ORDER BY style ASC";
                    if(!($stmt = $mysqli->prepare($cases)))
                        echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    if(!$stmt->bind_result($id, $style))
                        echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    while($stmt->fetch())
                        echo "<option value=$id>$style</option>\n", PHP_EOL;
                    $stmt->close();
                    ?>
                </select>
        </fieldset>

        <fieldset>
            <legend>Bill</legend>
            <p>Amount: <input type="text" name="Amount" /></p>
            <p>Due Date: <input type="text" name="DueDate" /></p>
        </fieldset>

        <p><input id='update_client' type="submit" class='button' name='update_client' value='Update client'/></p>
    </form>
</div>

</body>
</html>
