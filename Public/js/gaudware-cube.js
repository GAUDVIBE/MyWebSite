// Colors
fetch('/api/colors').then(r => r.json()).then(colors => {
    document.documentElement.style.setProperty('--primary-color', colors.primary);
    const cube = document.getElementById('cube');
    if (cube) cube.style.backgroundColor = colors.primary;
});

// Drag and drop
const cube = document.getElementById('cube');
const dropZone = document.getElementById('drop-zone');
let isDragging = false, offsetX, offsetY;

function updatePosition(clientX, clientY) {
    if (!cube) return;
    const x = Math.max(0, Math.min(clientX - offsetX, window.innerWidth - cube.offsetWidth));
    const y = Math.max(0, Math.min(clientY - offsetY, window.innerHeight - cube.offsetHeight));
    cube.style.left = `${x}px`;
    cube.style.top = `${y}px`;
    
    if (dropZone) {
        const cr = cube.getBoundingClientRect();
        const dr = dropZone.getBoundingClientRect();
        dropZone.classList.toggle('highlight', 
            cr.right > dr.left && cr.left < dr.right && cr.bottom > dr.top && cr.top < dr.bottom);
    }
}

function startDrag(clientX, clientY) {
    if (!cube) return;
    isDragging = true;
    const rect = cube.getBoundingClientRect();
    offsetX = clientX - rect.left;
    offsetY = clientY - rect.top;
    cube.style.cursor = 'grabbing';
    cube.style.transition = 'none';
}

function stopDrag() {
    if (!isDragging || !cube || !dropZone) return;
    isDragging = false;
    cube.style.cursor = 'pointer';
    cube.style.transition = 'transform 0.2s ease';
    
    const cr = cube.getBoundingClientRect();
    const dr = dropZone.getBoundingClientRect();
    
    if (cr.right > dr.left && cr.left < dr.right && cr.bottom > dr.top && cr.top < dr.bottom) {
        cube.style.transform = 'scale(0.5) rotate(180deg)';
        dropZone.style.backgroundColor = '#e74c3c';
        setTimeout(() => window.close(), 500);
    }
}

cube?.addEventListener('mousedown', (e) => startDrag(e.clientX, e.clientY));
cube?.addEventListener('touchstart', (e) => startDrag(e.touches[0].clientX, e.touches[0].clientY));
document.addEventListener('mousemove', (e) => isDragging && updatePosition(e.clientX, e.clientY));
document.addEventListener('touchmove', (e) => isDragging && updatePosition(e.touches[0].clientX, e.touches[0].clientY));
document.addEventListener('mouseup', stopDrag);
document.addEventListener('touchend', stopDrag);

// Hide loading
window.addEventListener('load', () => {
    const loading = document.getElementById('loading');
    if (loading) {
        loading.classList.add('hidden');
        setTimeout(() => loading.remove(), 500);
    }
});
