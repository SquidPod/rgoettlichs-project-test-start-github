<?php

$hostname = "localhost";
$username = "root";
$password = "mysql_remy";
$database = "testdatabase";

$conn = mysqli_connect($hostname, $username, $password, $database, 4306);

if (!$conn) {
    die("connection failed: ".mysqli_connect_error());
}

echo "database connection is ok";


$t = $_GET["temperature"];


$sql = "insert into sensorergebnisse (temperature) values ($t)";

if(mysqli_query($conn, $sql)) {

        echo "\nnew record created";

}  else {

    echo "error: ".$sql."<br>".mysqli_error($conn);

}

mysqli_close($conn);

?>