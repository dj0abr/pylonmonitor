<?php
// Specify the path to the JSON file
$jsonFilePath = __DIR__ . '/wxdata/configData.json';

// Check if the file exists
if (file_exists($jsonFilePath)) {
    // Set the Content-Type header to application/json
    header('Content-Type: application/json');

    // Read the file contents and echo them
    $jsonData = file_get_contents($jsonFilePath);
    echo $jsonData;
} else {
    // If the file does not exist, return an error
    http_response_code(404);
    echo json_encode(array("error" => "File not found."));
}
?>
