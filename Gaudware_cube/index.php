<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cube avec Zone de Dépôt</title>
    <link rel="stylesheet" href="style.css">
</head>

<?php
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
$TextColor = isDarkColor($randomColor) ? '#FFFFFF' : '#000000';
$buttonColor = isDarkColor($randomColor) ? '#4a6fa5' : '#2c5e8f';
?>
<style>
    :root {
        --primary-color: <?= $randomColor ?>;
        --primary-color-rgb: <?=
            hexdec(substr($randomColor, 1, 2)) . ',' .
            hexdec(substr($randomColor, 3, 2)) . ',' .
            hexdec(substr($randomColor, 5, 2))
        ?>;
        --text-color: <?= $TextColor ?>;
        --button-color: <?= $buttonColor ?>;
        --button-hover: <?= isDarkColor($randomColor) ? '#3a5a8a' : '#1c4e7f' ?>;
    }
</style>
<body>
    <div class="loading">
        <div class="spinner"></div>
        <span>Chargement...</span>
    </div>

    <canvas id="shaderCanvas">
        <div class="fallback-bg"></div>
    </canvas>
    <div id="drop-zone">Déposez le cube ici</div>
    <div id="cube"></div>
    <script src="script.js"></script>
</body>
</html>
