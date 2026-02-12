// Dynamic colors from API
async function loadDynamicColors() {
    try {
        const response = await fetch('/api/colors');
        const colors = await response.json();
        
        document.documentElement.style.setProperty('--primary-color', colors.primary);
        document.documentElement.style.setProperty('--primary-color-rgb', colors.primaryRgb);
        document.documentElement.style.setProperty('--text-color', colors.textColor);
        
        const title = document.getElementById('portfolioTitle');
        if (title) title.style.color = colors.primary;
        
        const themeColorMeta = document.getElementById('themeColorMeta');
        if (themeColorMeta) themeColorMeta.content = colors.primary;
        
        return colors;
    } catch (error) {
        console.error('Failed to load colors:', error);
    }
}

// Generate buttons
async function generateButtons() {
    try {
        const gaudwareRes = await fetch('/api/random-gaudware');
        const gaudwareData = await gaudwareRes.json();
        
        const buttons = [
            { type: 'pdf', label: 'CV', file: 'CV2025.pdf' },
            { type: 'pdf', label: 'PDF', file: 'BA510NA CI.pdf' },
            { type: 'link', label: 'Gaudware', url: `/${gaudwareData.game}` },
            { type: 'link', label: 'GitHub', url: 'https://github.com/GAUDVIBE' }
        ];
        
        const grid = document.getElementById('buttonsGrid');
        if (!grid) return;
        
        grid.innerHTML = buttons.map(btn => {
            if (btn.type === 'pdf') {
                return `<button class="pdf-button" onclick="handlePdfView('${btn.file}')">
                    <span class="button-label">${btn.label}</span>
                </button>`;
            } else {
                return `<a href="${btn.url}" class="action-button" target="_blank" rel="noopener noreferrer">
                    <span class="button-label">${btn.label}</span>
                </a>`;
            }
        }).join('');
    } catch (error) {
        console.error('Failed to generate buttons:', error);
    }
}

// PDF handler
window.handlePdfView = function(pdfFile) {
    const pdfUrl = `/pdf/${encodeURIComponent(pdfFile)}`;
    window.open(pdfUrl, '_blank');
}

// Loading
window.addEventListener('load', () => {
    setTimeout(() => {
        const loading = document.getElementById('loading');
        if (loading) {
            loading.classList.add('loaded');
            setTimeout(() => loading.remove(), 500);
        }
    }, 800);
});

// Initialize
document.addEventListener('DOMContentLoaded', async () => {
    await loadDynamicColors();
    await generateButtons();
});
