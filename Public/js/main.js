// ============ DYNAMIC COLOR MANAGEMENT ============
async function loadDynamicColors() {
    try {
        const response = await fetch('/api/colors');
        const colors = await response.json();
        
        // Set CSS variables
        document.documentElement.style.setProperty('--primary-color', colors.primary);
        document.documentElement.style.setProperty('--primary-color-rgb', colors.primaryRgb);
        document.documentElement.style.setProperty('--text-color', colors.textColor);
        document.documentElement.style.setProperty('--button-color', colors.buttonColor);
        document.documentElement.style.setProperty('--button-hover', colors.buttonHover);
        
        // Update meta tags
        const themeColorMeta = document.getElementById('themeColorMeta');
        const msTileColorMeta = document.getElementById('msTileColorMeta');
        
        if (themeColorMeta) themeColorMeta.content = colors.primary;
        if (msTileColorMeta) msTileColorMeta.content = colors.primary;
        
        // Update title color
        const title = document.getElementById('portfolioTitle');
        if (title) title.style.color = colors.primary;
        
        return colors;
    } catch (error) {
        console.error('Failed to load colors:', error);
        // Fallback colors
        document.documentElement.style.setProperty('--primary-color', '#3498db');
        document.documentElement.style.setProperty('--primary-color-rgb', '52,152,219');
        document.documentElement.style.setProperty('--text-color', '#000000');
    }
}

// ============ BUTTONS GENERATION ============
async function generateButtons() {
    const buttonsGrid = document.getElementById('buttonsGrid');
    if (!buttonsGrid) return;
    
    try {
        // Get random Gaudware game
        const gaudwareResponse = await fetch('/api/random-gaudware');
        const gaudwareData = await gaudwareResponse.json();
        
        const buttons = [
            {
                type: 'pdf',
                label: 'CV',
                file: 'CV2025.pdf',
                class: 'pdf-button'
            },
            {
                type: 'pdf',
                label: 'PDF',
                file: 'BA510NA CI.pdf',
                class: 'pdf-button'
            },
            {
                type: 'link',
                label: 'Gaudware',
                url: `/${gaudwareData.game}`,
                class: 'action-button',
                target: '_blank'
            },
            {
                type: 'link',
                label: 'GitHub',
                url: 'https://github.com/GAUDVIBE',
                class: 'action-button',
                target: '_blank'
            }
        ];
        
        buttonsGrid.innerHTML = buttons.map(button => {
            if (button.type === 'pdf') {
                return `<button class="${button.class}" onclick="handlePdfView('${button.file}')">
                    <span class="button-label">${button.label}</span>
                </button>`;
            } else {
                return `<a href="${button.url}" class="${button.class}" target="${button.target}" rel="noopener noreferrer">
                    <span class="button-label">${button.label}</span>
                </a>`;
            }
        }).join('');
        
    } catch (error) {
        console.error('Failed to generate buttons:', error);
    }
}

// ============ PDF HANDLING ============
window.handlePdfView = function(pdfFile) {
    const isIOS = /iPad|iPhone|iPod/.test(navigator.userAgent);
    const pdfUrl = `/pdf/${encodeURIComponent(pdfFile)}`;
    
    if (isIOS) {
        window.location.href = pdfUrl;
        displayMessage("Ouverture du PDF...", 2000);
        return;
    }
    
    const newWindow = window.open(pdfUrl, '_blank');
    
    if (!newWindow || newWindow.closed || typeof newWindow.closed === 'undefined') {
        displayMessage("Ouvrez le PDF depuis votre gestionnaire de fichiers", 3000);
        const link = document.createElement('a');
        link.href = pdfUrl;
        link.download = pdfFile;
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);
    }
}

// ============ MESSAGE DISPLAY ============
window.displayMessage = function(message, duration = 3000) {
    const container = document.getElementById('messageDisplay');
    if (!container) return;
    
    container.innerHTML = `
        <div class="message">
            <div class="message-content">${message}</div>
        </div>
    `;
    
    setTimeout(() => {
        const messageElement = container.querySelector('.message');
        if (messageElement) {
            messageElement.classList.add('fade-out');
            setTimeout(() => container.innerHTML = '', 500);
        }
    }, duration);
}

// ============ LOADING MANAGEMENT ============
function hideLoading() {
    const loading = document.getElementById('loading');
    if (loading) {
        loading.classList.add('loaded');
        setTimeout(() => {
            if (loading.parentNode) loading.remove();
        }, 500);
    }
}

// ============ INITIALIZATION ============
document.addEventListener('DOMContentLoaded', async () => {
    // Load dynamic colors
    await loadDynamicColors();
    
    // Generate buttons
    await generateButtons();
    
    // Check WebGL support
    setTimeout(() => {
        const canvas = document.getElementById('shaderCanvas');
        if (canvas && !canvas.getContext('webgl')) {
            const fallback = document.querySelector('.fallback-bg');
            if (fallback) fallback.style.display = 'block';
        }
    }, 1000);
});

window.addEventListener('load', () => {
    setTimeout(hideLoading, 800);
});
