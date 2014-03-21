<?php
require 'db_connect.php';
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<body>
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
        $attorneys = "SELECT fname, lname, bar_number
                        FROM attorney att
                        INNER JOIN attorney_matter att_mat
                        ON att.id = att_mat.attorney_id
                        INNER JOIN matter mat
                        ON att_mat.matter_id = mat.id
                        WHERE mat.id = ?
                        ORDER BY lname, fname ASC";
        $p_matter_id = $_POST['Case'];

        // Prepare query to display attorneys filtered by case
        if(!($stmt = $mysqli->prepare($attorneys)))
            echo "Prepare failed: $mysqli->errno $mysqli->error", PHP_EOL;
        if(!($stmt->bind_param("i",$p_matter_id)))
            echo "Bind failed: "  . $stmt->errno . " " . $stmt->error;
        if(!$stmt->execute())
            echo "Execute failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
        if(!$stmt->bind_result($fname, $lname, $bar_number))
            echo "Bind failed: $mysqli->connect_errno $mysqli->connect_error", PHP_EOL;
        while($stmt->fetch())
         echo "<tr>\n<td>\n $lname,  $fname \n</td>\n<td>\n $bar_number\n</td>\n</tr>", PHP_EOL;
        $stmt->close();
        ?>
    </table>
</div>

</body>
</html>
