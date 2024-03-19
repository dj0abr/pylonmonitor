<?php
// Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Prepare an array to store the incoming data
    $data = array(
        "ssid" => $_POST['ssid'],
        "password" => $_POST['password'],
        "mqttBrokerIP" => $_POST['mqttBrokerIP'],
        "brokerusername" => $_POST['brokerusername'],
        "brokerpassword" => $_POST['brokerpassword'],
        "publishTopic" => $_POST['publishTopic'],
        "responseTopic" => $_POST['responseTopic'],
        "locationTopic" => $_POST['locationTopic'],
        "deviceTopic" => $_POST['deviceTopic']
    );

    // Encode the data as JSON
    $jsonData = json_encode($data, JSON_PRETTY_PRINT);

    // Specify the file path where the data will be saved
    $filePath = __DIR__ . '/wxdata/configData.json';

    // Write the JSON data to the file, creating it if it doesn't exist, or overwriting it if it does
    if (file_put_contents($filePath, $jsonData) !== false) {
        echo "Data successfully saved.";
    } else {
        // If writing the file fails, return an error message
        http_response_code(500);
        echo "Error saving data to " . $filePath;
    }
} else {
    // If the request method is not POST, return an error
    http_response_code(405); // Method Not Allowed
    echo "Invalid request method. Only POST requests are allowed.";
}
?>
