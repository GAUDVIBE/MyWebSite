<?php
header('Content-Type: text/plain');

// SECURITY WARNING: This is extremely dangerous if exposed to the internet
// In a real implementation, you would need:
// 1. Authentication
// 2. Command whitelisting
// 3. Proper input validation
// 4. Limited permissions

// Simple security check - remove this in production!
if (!isset($_SERVER['HTTP_REFERER']) || parse_url($_SERVER['HTTP_REFERER'], PHP_URL_HOST) !== $_SERVER['HTTP_HOST']) {
    die("Unauthorized access");
}

// Very basic command filtering (inadequate for production)
$command = $_POST['command'] ?? '';
$allowedCommands = ['ls', 'pwd', 'whoami', 'date']; // Example whitelist

    if (!in_array(explode(' ', $command)[0], $allowedCommands)) {
        die("Command not allowed");
    }
$disallowed = ['rm', 'mv', 'dd', '>', '|', '&', ';', 'sudo', 'su'];

foreach ($disallowed as $dangerous) {
    if (stripos($command, $dangerous) !== false) {
        die("Command contains potentially dangerous operation: $dangerous");
    }
}

// Execute command
if (!empty($command)) {
    try {
        $output = shell_exec($command . ' 2>&1');
        echo $output ?: "(No output)";
    } catch (Exception $e) {
        echo "Error executing command: " . $e->getMessage();
    }
} else {
    echo "No command provided";
}
