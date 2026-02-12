<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Portfolio GAUDRY</title>
    <!-- Theme color meta tags with dynamic update -->
    <meta name="theme-color" content="<?= $randomColor ?>" id="themeColorMeta">
    <meta name="msapplication-TileColor" content="<?= $randomColor ?>">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
    <meta name="apple-mobile-web-app-title" content="Portfolio GAUDRY">
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
        
    // Liste des dossiers disponibles
    $folders = ['Gaudware_cube', 'Gaudware_fight']; // Ajoutez/modifiez

    // Sélection aléatoire d'un dossier
    $randomFolder = $folders[array_rand($folders)];
    $path = $randomFolder . '/index.php';
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
    <script>
        // Force Chrome to recognize theme color
        document.addEventListener('DOMContentLoaded', function() {
            const meta = document.getElementById('themeColorMeta');
            const newColor = '<?= $randomColor ?>';
            if (meta.content !== newColor) {
                meta.content = newColor;
                document.head.appendChild(meta).remove();
            }
        });
    </script>
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
            <div class="header-container">
                <header>
                    <h1 class="portfolio-title" style="color: <?= $randomColor ?>">ANTOINE GAUDRY</h1>
                </header>
            </div>

            <div class="buttons-grid">
                <button class="pdf-button" onclick="handlePdfView('CV2025.pdf')">
                    <span class="button-label">CV</span>
                </button>
                
                <button class="pdf-button" onclick="handlePdfView('BA510NA CI.pdf')">
                    <span class="button-label">PDF</span>
                </button>

                <!-- bouton pour Gaudware -->
                <a href="<?php echo $path; ?>" class="action-button" target="_blank" rel="noopener noreferrer">
                <span class="button-label">Gaudware</span>
                </a>
                
                <a href="https://github.com/GAUDVIBE" class="action-button" target="_blank" rel="noopener noreferrer">
                    <span class="button-label">GitHub</span>
                </a>
            </div>
            
            <div id="messageDisplay" aria-live="polite"></div>
        </main>
    </div>
    <script>
        // PDF handling function
        function handlePdfView(pdfFile) {
            const isIOS = /iPad|iPhone|iPod/.test(navigator.userAgent);
            
            if (isIOS) {
                window.location.href = pdfFile;
                displayMessage("Ouverture du PDF...", 2000);
                return;
            }
            
            const newWindow = window.open(pdfFile, '_blank');
            
            if (!newWindow || newWindow.closed) {
                displayMessage("Ouvrez le PDF depuis votre gestionnaire de fichiers", 3000);
                const link = document.createElement('a');
                link.href = pdfFile;
                link.download = pdfFile;
                document.body.appendChild(link);
                link.click();
                document.body.removeChild(link);
            }
        }

        function displayMessage(message, duration = 3000) {
            const container = document.getElementById('messageDisplay');
            container.innerHTML = `
                <div class="message">
                    <div class="message-content">${message}</div>
                </div>
            `;
            
            setTimeout(() => {
                container.querySelector('.message').classList.add('fade-out');
                setTimeout(() => container.innerHTML = '', 500);
            }, duration);
        }

        window.addEventListener('load', () => {
            setTimeout(() => {
                document.querySelector('.loading').classList.add('loaded');
                setTimeout(() => document.querySelector('.loading').remove(), 500);
            }, 800);
        });

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
