<?php

$hostname = "localhost";
$username = "root";
$password = "mysql_remy";
$database = "testdatabase";

$conn = mysqli_connect($hostname, $username, $password, $database, 4306);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}


$sql = "select * from sensorergebnisse";
$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) > 0) {
    // output data of each row
    while($row = mysqli_fetch_assoc($result)) {
      echo "time:" . $row["datetime"]. " - temperature: " . $row["Temperature"]."\n";
    }
} else {
    echo "0 results";
}

mysqli_close($conn);
?>