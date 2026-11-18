<?php
/*
 * scandevices.php
 * Returns a JSON array of all available /dev/ttyUSB* devices with their
 * udevadm serial string and idVendor so the setup UI can present them
 * for one-click selection.
 *
 * Example response:
 * [
 *   { "device": "/dev/ttyUSB0", "serial": "AB0KOTJ8", "idVendor": "0403" },
 *   { "device": "/dev/ttyUSB1", "serial": "",          "idVendor": "1a86" }
 * ]
 */

header('Content-Type: application/json');

$devices = [];

// Find all ttyUSB* nodes
$output = [];
exec('ls -1 /dev/ttyUSB* 2>/dev/null', $output);

foreach ($output as $dev) {
    $dev = trim($dev);
    if ($dev === '') continue;

    $serial   = '';
    $idVendor = '';

    // Read serial attribute
    $cmd = 'udevadm info -a -p $(udevadm info -q path -n ' . escapeshellarg($dev) . ') 2>/dev/null'
         . ' | grep \'{serial}\' | cut -d \'"\' -f2 | head -n 1';
    $serial = trim(shell_exec($cmd) ?? '');

    // Read idVendor attribute
    $cmd = 'udevadm info -a -p $(udevadm info -q path -n ' . escapeshellarg($dev) . ') 2>/dev/null'
         . ' | grep \'{idVendor}\' | cut -d \'"\' -f2 | head -n 1';
    $idVendor = trim(shell_exec($cmd) ?? '');

    $devices[] = [
        'device'   => $dev,
        'serial'   => $serial,
        'idVendor' => $idVendor,
    ];
}

echo json_encode($devices, JSON_PRETTY_PRINT);
?>
