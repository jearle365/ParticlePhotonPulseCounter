//this is the php code that goes into the url that your particle integration is posting to.
//It decodes the json data in order to insert to database
//file should be located in public_html folder. make a seperate folder for your password information that is restricted in the same folder

<?php

require "directory.php";

//Receive the RAW post data.
$content = (file_get_contents("php://input"));

//Attempt to decode the incoming RAW post data from JSON.
$decoded = json_decode($content, true);

//get the actual json details

$coreid = $decoded['coreid'];
$totalizer = $decoded['totalizer'];
$flowRate = $decoded['flowRate'];



//Process the JSON end of json recieving


try {
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    // set the PDO error mode to exception
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $sql = "INSERT INTO `meterReadings` (`Flow`, `Totalizer`, `Time`, `Date`, `Metername`) VALUES ('$flowRate', '$totalizer', CURRENT_TIME(), CURRENT_DATE(), '$coreid')";
    
    $conn->exec($sql);
    echo "New record created successfully";
    }
catch(PDOException $e)
    {
    echo $sql . "<br>" . $e->getMessage();
    }

$conn = null;
?>
