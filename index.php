<!DOCTYPE html>
<html lang="fr">

<?php
$TextColor = '#000000';

function generateRandomColor() {
    return sprintf('#%06X', mt_rand(0, 0xFFFFFF));
}
function isDarkColor($hexColor) {
    $hexColor = ltrim($hexColor, '#');
    $r = hexdec(substr($hexColor, 0, 2));
    $g = hexdec(substr($hexColor, 2, 2));
    $b = hexdec(substr($hexColor, 4, 2));

    $luminance = 0.2126 * $r + 0.7152 * $g + 0.0722 * $b;

    return $luminance < 128;
}

$randomColor = generateRandomColor();

if (isDarkColor($randomColor)) {
    $TextColor = '#FFFFFF';
    $ButtonColor = 'blue';
}

echo sprintf('
    <style type="text/css">
        body {
            background-color: %s;
            color: %s; /* Appliquer la couleur du texte */
        }
    </style>
', $randomColor, $TextColor);

echo $randomColor; 
?>



<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Mon site web</title>

    <!-- Link to style.css file -->
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h1>Mon site web</h1>

    <?php
    $pdfFile = 'BA510NA CI.pdf';
    $message1 = "Vous avez cliqué sur PDF 1 !";
    if (file_exists($pdfFile)) {
        echo '<button class="pdf-button" onclick="openPdfPopup(\'' . $pdfFile . '\', \'' . $message1 . '\')">PDF 1</button>';
    } else {
        echo '<p>Le fichier PDF n\'existe pas.</p>';
    }
    ?>

    <?php
    $pdfFile2 = 'Fiche OT en cours.pdf';
    $message2 = "Vous avez cliqué sur PDF 2 !";
    if (file_exists($pdfFile2)) {
        echo '<button class="pdf-button" onclick="openPdfPopup(\'' . $pdfFile2 . '\', \'' . $message2 . '\')">PDF 2</button>';
    } else {
        echo '<p>Le fichier PDF n\'existe pas.</p>';
    }
    ?>

    <a class="github-link" href="https://github.com" target="_blank">Mon GitHub</a>

    <div id="messageDisplay"></div>
 
    <script>
        // Fonction to display message
        function displayMessage(message) {
            // Creat <p> element
            const messageElement = document.createElement('p');
            messageElement.classList.add('message'); // Add a class 
            messageElement.textContent = message;  // Define text
            const messageContainer = document.getElementById('messageDisplay');
            messageContainer.innerHTML = ''; // Remove previous element
            messageContainer.appendChild(messageElement);  // Add new message
        }

        function openPdfPopup(pdfUrl, message) {

            const width = 800; 
            const height = 600; 
            const left = (window.innerWidth - width) / 2; 
            const top = (window.innerHeight - height) / 2;

            window.open(
                pdfUrl,
                'pdfPopup',
                `width=${width},height=${height},left=${left},top=${top},resizable=yes,scrollbars=yes`
            );

            displayMessage(message);
        }
    </script>
</body>
</html>
