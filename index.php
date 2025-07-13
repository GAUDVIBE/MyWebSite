<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>Portfolio GAUDRY</title>
    <link rel="stylesheet" href="style.css">
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
</head>
<body>
    <div class="loading">
        <div class="spinner"></div>
        <span>Chargement...</span>
    </div>
    
    <canvas id="shaderCanvas">
        <div class="fallback-bg"></div>
    </canvas>
    
    <div class="content-container">
        <main class="main-content">
            <!-- Header Container -->
            <div class="header-container">
                <header>
                    <h1>ANTOINE GAUDRY</h1>
                </header>
            </div>

            <!-- Buttons Grid -->
            <div class="buttons-grid">
                <button class="pdf-button" onclick="handlePdfView('CV2025.pdf')">
                    <span class="label">CV</span>
                </button>
                
                <button class="pdf-button" onclick="handlePdfView('BA510NA CI.pdf')">
                    <span class="label">PDF</span>
                </button>
                
                <a href="https://github.com/GAUDVIBE" class="action-button" target="_blank">
                    <span class="label">GitHub</span>
                </a>
            </div>
            
            <div id="messageDisplay" aria-live="polite"></div>
        </main>
    </div>

    <!-- Rest of your JavaScript remains the same -->
    <script>
        // Gestion PDF améliorée
        function handlePdfView(pdfFile) {
            const isIOS = /iPad|iPhone|iPod/.test(navigator.userAgent);
            const isMobile = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);
            
            // Solution pour iOS/Safari
            if (isIOS) {
                window.location.href = pdfFile;
                displayMessage("Ouverture du PDF pour mobile...", 2000);
                return;
            }
            
            // Solution standard
            const newWindow = window.open(pdfFile, '_blank');
            
            // Fallback si bloqué
            if (!newWindow || newWindow.closed) {
                displayMessage("Ouvrez le PDF depuis votre gestionnaire de fichiers", 3000);
                
                // Créer un lien invisible pour le téléchargement
                const link = document.createElement('a');
                link.href = pdfFile;
                link.download = pdfFile;
                document.body.appendChild(link);
                link.click();
                document.body.removeChild(link);
            }
        }

        // Affichage des messages
        function displayMessage(message, duration = 3000) {
            const messageElement = document.createElement('div');
            messageElement.className = 'message';
            messageElement.innerHTML = `
                <div class="message-content">
                    ${message}
                </div>
            `;
            
            const container = document.getElementById('messageDisplay');
            container.innerHTML = '';
            container.appendChild(messageElement);
            
            setTimeout(() => {
                messageElement.classList.add('fade-out');
                setTimeout(() => messageElement.remove(), 500);
            }, duration);
        }

        // Gestion du chargement
        window.addEventListener('load', () => {
            setTimeout(() => {
                document.querySelector('.loading').classList.add('loaded');
                setTimeout(() => {
                    document.querySelector('.loading').remove();
                }, 500);
            }, 800);
        });

        // Fallback si WebGL échoue
        document.addEventListener('DOMContentLoaded', () => {
            setTimeout(() => {
                if (!document.querySelector('canvas').getContext('webgl')) {
                    document.querySelector('.fallback-bg').style.display = 'block';
                }
            }, 1000);
        });
    </script>
    
    <script src="shader.js"></script>
</body>
</html>
