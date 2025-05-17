<!DOCTYPE html>
<html lang="fr">

<!DOCTYPE html>
<html lang="fr">

<?php
// Configuration initiale
ini_set('display_errors', 1);
error_reporting(E_ALL);

// Configuration des couleurs
$TextColor = '#000000';

function generateRandomColor() {
    return sprintf('#%06X', mt_rand(0, 0xFFFFFF));
}

function isDarkColor($hexColor) {
    $hexColor = ltrim($hexColor, '#');
    $r = hexdec(substr($hexColor, 0, 2));
    $g = hexdec(substr($hexColor, 2, 2));
    $b = hexdec(substr($hexColor, 4, 2));
    return (0.2126 * $r + 0.7152 * $g + 0.0722 * $b) < 128;
}

$randomColor = generateRandomColor();
if (isDarkColor($randomColor)) {
    $TextColor = '#FFFFFF';
}
$buttonColor = isDarkColor($randomColor) ? '#29262e' : '#ebeacc' ;
?>

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Web Site</title>
    <style type="text/css">
        body {
            background-color: <?= $randomColor ?>;
            color: <?= $TextColor ?>;
            font-family: Arial, sans-serif;
            margin: 20px;
            transition: all 0.3s ease;
        }
        button {
            padding: 10px 15px;
            margin: 10px 5px;
            border-radius: 5px;
            cursor: pointer;
            background-color: <?= $buttonColor ?>;
            color: <?= $TextColor ?>;
            border: none;
            font-size: 16px;
            transition: transform 0.2s, box-shadow 0.2s;
        }
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 8px rgba(0,0,0,0.2);
        }
        #terminal {
            background-color: rgba(0,0,0,0.2);
            border-radius: 5px;
            padding: 15px;
            margin: 20px auto;
            max-width: 800px;
            font-family: monospace;
            height: 400px;
            overflow-y: auto;
        }
        #terminal-input {
            background-color: transparent;
            border: none;
            color: <?= $TextColor ?>;
            font-family: monospace;
            width: 80%;
            outline: none;
            caret-color: #000;
        }
        .prompt {
            color: <?= $buttonColor ?>;
        }
        .terminal-line {
            margin-bottom: 5px;
        }
        .warning {
            color: #ffcc00;
            font-weight: bold;
        }
    </style>
</head>
<body>
    <h1>My web site !</h1>

    <?php if (file_exists('CV 2025 engl.pdf')): ?>
        <button onclick="window.open('CV 2025 engl.pdf', 'pdfPopup', 'width=800,height=600')"> CV.pdf </button>
    <?php endif; ?>

    <a href="https://github.com" target="_blank" style="color: <?=$buttonColor ?>;">Mon GitHub</a>

    <div id="terminal">
        <div class="terminal-line warning">WARNING: This executes real commands on the server. Use with caution!</div>
        <div class="terminal-line">Web Terminal - Type 'help' for available commands</div>
    </div>

    <script src="script.js"></script>
</body>
</html>
