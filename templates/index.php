<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>APIcall</title>
</head>
<body>
  <p>----Centraline----</p>
  <p>|---Centrale---|</p>
<?php

$servername = "localhost";
$username = "root";
$password = ""; // anche per linux non serve utilizzare la psw (almeno con lampp)
$dbname = "centralina";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
  die("Connessione fallita: " . $conn->connect_error);
}

$sql = "SELECT id, temperatura, umidita, pm FROM centrale";
$result = $conn -> query($sql);

if ($result->num_rows > 0) {
  echo "<table><tr><th>ID</th><th>Temp</th><th>Umi</th><th>Pm</th></tr>";

  while($row = $result->fetch_assoc()) {
    echo "<tr><td>".$row["id"]."</td><td>".$row["temperatura"]."</td><td>".$row["umidita"]."</td><td>".$row["pm"]."</td></tr>";
  }


  echo "</table>";
} else {
  echo "0 risultati";
}
$conn->close();
?>
</body>
</html>