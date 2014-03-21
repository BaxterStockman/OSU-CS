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
    <h3>Current matters</h3>
    <table>
        <?php
        // Prepare statement for getting case names
        $matters = "SELECT style FROM matter ORDER BY style ASC";
        if(!($stmt = $mysqli->prepare($matters)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!$stmt->bind_result($style))
            echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
        while($stmt->fetch())
            echo "<tr><td>$style</td></tr>", PHP_EOL;
        $stmt->close();
        ?>
    </table>
</div>

<div>
    <form method="post" action="details_matter.php">
        <fieldset>
            <legend>Get Details on a Matter</legend>
            <select name="Name">
                <option value="">Select...</option>
                <?php
                // Prepare statement for getting case names
                $matters = "SELECT id, style FROM matter ORDER BY style ASC";
                if(!($stmt = $mysqli->prepare($matters)))
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
        <p><input type="submit" value="Get Details" /></p>
    </form>
</div>

<div>
    <h3>Add a Matter</h3>
    <form method="post" action="add_matter.php">

        <fieldset>
            <legend>Matter Name</legend>
            <p>Matter Name: <input type="text" name="MatterName" /></p>
        </fieldset>

        <fieldset>
            <legend>Court</legend>
                <select name="Court">
                    <option value="">Select...</option>
                    <?php
                    // Prepare statement for getting court namesj
                    $courts = "SELECT id, name FROM court ORDER BY name ASC";
                    if(!($stmt = $mysqli->prepare($courts)))
                        echo "Prepare failed: $stmt->errno $stmt->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    if(!$stmt->bind_result($court_id, $court_name))
                        echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    while($stmt->fetch())
                        echo "<option value=$court_id>$court_name</option>\n", PHP_EOL;
                    $stmt->close();
                    ?>
                </select>
        </fieldset>

        <fieldset>
            <legend>Filed Date</legend>
            <p>Date (YYYY-MM-DD): <input type="date" name="FiledDate" /></p>
        </fieldset>

        <fieldset>
            <legend>Matter Type</legend>
            <p>Type: <input type="text" name="MatterType" /></p>
        </fieldset>

        <fieldset>
            <legend>Attorney(s)</legend>
                <table>
                <?php
                    // Prepare statement for getting attorney names
                    if(!($stmt = $mysqli->prepare("SELECT id, lname, fname FROM attorney ORDER BY lname, fname ASC")))
                        echo "Prepare failed: $stmt->errno $stmt->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    if(!$stmt->bind_result($att_id, $att_lname, $att_fname))
                        echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    while($stmt->fetch())
                        echo "<tr><td><input type='checkbox' name='att_checkbox[]' id='att_checkbox' class='check' value=$att_id /></td><td>$att_lname, $att_fname</td></tr>", PHP_EOL;
                    $stmt->close();
                    ?>
                </table>
        </fieldset>

        <fieldset>
            <legend>Client(s)</legend>
                <table>
                    <?php
                    // Prepare statement for getting client names
                    if(!($stmt = $mysqli->prepare("SELECT id, lname, fname FROM client ORDER BY lname, fname  ASC")))
                        echo "Prepare failed: $stmt->errno $stmt->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    if(!$stmt->bind_result($cli_id, $cli_lname, $cli_fname))
                        echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    while($stmt->fetch())
                        echo "<tr><td><input type='checkbox' name='cli_checkbox[]' id='cli_checkbox' class='check' value=$cli_id /></td><td>$cli_lname, $cli_fname</td></tr>", PHP_EOL;
                    $stmt->close();
                    ?>
                </table>
        </fieldset>
        <p><input id='add_matter' type="submit" class='button' name='add_matter' value='Add matter'/></p>
    </form>
</div>

<div>
    <h3>Update a matter</h3>
    <form method="post" action="update_matter.php">
        <fieldset>
            <legend>Matter</legend>
            <select name="Name">
                <option value="">Select...</option>
                <?php
                // Prepare statement for getting matter names
                $matters = "SELECT id, style FROM matter ORDER BY style ASC";
                if(!($stmt = $mysqli->prepare($matters)))
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
            <legend>Add a Document</legend>
            <p>Type: <input type="text" name="DocType" /></p>
            <p>Date Filed (YYYY-MM-DD): <input type="date" name="FiledDate" /></p>
        </fieldset>

        <fieldset>
            <legend>Add a Hearing</legend>
            <p>Type: <input type="text" name="HearingType" /></p>
            <p>Date (YYYY-MM-DD): <input type="date" name="HearingDate" /></p>
            <p>Street: <input type="text" name="Street" /></p>
            <p>City: <input type="text" name="City" /></p>
            <p>State: <input type="text" name="State" /></p>
            <p>Postal Code: <input type="text" name="PostalCode" /></p>
            <select name="Court">
                <option value="">Select...</option>
                <?php
                // Prepare statement for getting court names
                $courts = "SELECT id, name FROM court ORDER BY name ASC";
                if(!($stmt = $mysqli->prepare($courts)))
                    echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->execute())
                    echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->bind_result($court_id, $court_name))
                    echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
                while($stmt->fetch())
                    echo "<option value=$court_id>$court_name</option>\n", PHP_EOL;
                $stmt->close();
                ?>
            </select>
        </fieldset>
        <p><input id='update_matter' type="submit" class='button' name='update_matter' value='Update matter'/></p>
    </form>
</div>

<div>
    <h3>Add a Court</h3>
    <form method="post" action="add_court.php">
        <fieldset>
            <legend>Court</legend>
            <p>Name: <input type="text" name="Name" /></p>
            <p>Street: <input type="text" name="Street" /></p>
            <p>City: <input type="text" name="City" /></p>
            <p>State: <input type="text" name="State" /></p>
            <p>Postal Code: <input type="text" name="PostalCode" /></p>
        </fieldset>
        <p><input id='update_matter' type="submit" class='button' name='add_court' value='Add Court'/></p>
    </form>
</div>
</body>
</html>
