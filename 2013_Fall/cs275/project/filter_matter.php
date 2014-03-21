<?php
require 'db_connect.php';
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<body>
<div>
<?php
    // Set variables
    $p_matter_id = $_POST['Case'];

    // Prepare statement to print case name
    $matter = "SELECT style FROM matter WHERE id = $p_matter_id";
    if(!($stmt = $mysqli->prepare($matter)))
        echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
    if(!$stmt->execute())
        echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
    if(!$stmt->bind_result($style))
        echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
    while($stmt->fetch())
     echo "<h3>Clients associated with $style</h3>", PHP_EOL;
    $stmt->close();
?>
</div>

<div>
    <table>
<?php
        $clients = "SELECT fname, lname
                        FROM client cli
                        INNER JOIN client_matter cli_mat
                        ON cli.id = cli_mat.client_id
                        INNER JOIN matter mat
                        ON cli_mat.matter_id = mat.id
                        WHERE mat.id = ?
                        ORDER BY lname, fname ASC";

        // Prepare query to display client associated with matter
        if(!($stmt = $mysqli->prepare($clients)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i",$p_matter_id)))
            echo "Bind failed: "  . $stmt->errno . " " . $stmt->error;
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
        if(!$stmt->bind_result($fname, $lname))
            echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
        while($stmt->fetch())
         echo "<tr><td>$lname, $fname</td><td></tr>", PHP_EOL;
        $stmt->close();
        ?>
    </table>
</div>

</body>
</html>
