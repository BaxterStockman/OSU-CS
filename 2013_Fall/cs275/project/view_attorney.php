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
    <h3>Current Attorneys</h3>
    <table>
        <thead>
            <tr>
                <th>Name</th>
                <th>Bar Number</th>
            </tr>
        </thead>
        <?php
        // Prepare statement
        $attorneys = "SELECT fname, lname, bar_number FROM attorney ORDER BY lname, fname ASC";
        $stmt = $mysqli->prepare($attorneys);
        if(!$stmt)
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        // Execute statement
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
        // Bind result
        if(!$stmt->bind_result($fname, $lname, $bar_number))
            echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
        // Fetch data and echo to screen
        while($stmt->fetch())
         echo "<tr><td>$lname,  $fname</td><td>$bar_number</td></tr>", PHP_EOL;
        // Close statment
        $stmt->close();
        ?>
    </table>
</div>

<div>
    <form method="post" action="filter_attorney.php">
        <fieldset>
            <legend>Filter by Case</legend>
                <select name="Case">
                    <option value="">Select...</option>
                    <?php
                    $cases = "SELECT id, style FROM matter ORDER BY style ASC";
                    if(!($stmt = $mysqli->prepare($cases)))
                        echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->errno $mysqli->connect_error", PHP_EOL;
                    if(!$stmt->bind_result($id, $style))
                        echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
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
    <h3>Add an Attorney</h3>
    <form method="post" action="add_attorney.php">

        <fieldset>
            <legend>Name</legend>
            <p>First Name: <input type="text" name="FirstName" /></p>
            <p>Last Name: <input type="text" name="LastName" /></p>
        </fieldset>

        <fieldset>
            <legend>Bar Number</legend>
            <p>Bar Number: <input type="text" name="BarNumber" /></p>
        </fieldset>

        <fieldset>
            <legend>Case Assignment</legend>
                <table>
                    <?php
                    // Prepare statement getting information about cases
                    $cases = "SELECT style, id FROM matter ORDER BY style ASC";
                    if(!($stmt = $mysqli->prepare($cases)))
                        echo "Prepare failed: $stmt->errno $stmt->error", PHP_EOL;
                    if(!$stmt->execute())
                        echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    if(!$stmt->bind_result($style, $id))
                        echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                    while($stmt->fetch())
                        echo "<tr><td><input type='checkbox' name='checkbox[]' id='checkbox' class='check' value=$id></td><td>$style</td></tr>", PHP_EOL;
                    $stmt->close();
                    ?>
                </table>
        </fieldset>
        <p><input id='add_attorney' type="submit" class='button' name='add_attorney' value='Add attorney'/></p>
    </form>
</div>

<div>
    <h3>Update an Attorney</h3>
    <form method="post" action="update_attorney.php">

        <fieldset>
            <legend>Name</legend>
            <select name="Name">
                <option value="">Select...</option>
                <?php
                // Prepare statement for getting names of attorneys
                $attorney_names = "SELECT id, bar_number, lname, fname FROM attorney ORDER BY lname, fname ASC";
                if(!($stmt = $mysqli->prepare($attorney_names)))
                    echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->execute())
                    echo "Execute failed: $mysqli->errno $mysqli->error", PHP_EOL;
                if(!$stmt->bind_result($id, $bar_number, $lname, $fname))
                    echo "Bind failed: $mysqli->errno $mysqli->error", PHP_EOL;
                while($stmt->fetch())
                    echo "<option value=$id>$lname, $fname</option>\n", PHP_EOL;
                $stmt->close();
                ?>
            </select>
        </fieldset>

        <fieldset>
            <legend>Bar Number</legend>
            <p>Bar Number:<input type="text" name="BarNumber" /></p>
        </fieldset>

        <fieldset>
            <legend>Case Assignment</legend>
            <table>
                <?php
                // Prepare statement for getting case names
                if(!($stmt = $mysqli->prepare("SELECT style, id FROM matter ORDER BY style ASC")))
                    echo "Prepare failed: $stmt->errno $stmt->error", PHP_EOL;
                if(!$stmt->execute())
                    echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                if(!$stmt->bind_result($style, $id))
                    echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
                while($stmt->fetch())
                    echo "<tr><td><input type='checkbox' name='checkbox[]' id='checkbox' class='check' value=$id></td><td>$style</td></tr>", PHP_EOL;
                $stmt->close();
                ?>
            </table>
        </fieldset>
        <p><input id='update_attorney' type="submit" class='button' name='update_attorney' value='Update attorney'/></p>
    </form>
</div>

</body>
</html>
