const express = require('express');
const path = require('path');
const fs = require('fs').promises;

const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.static(path.join(__dirname, 'public')));

// Colors API
app.get('/api/colors', (req, res) => {
    const randomColor = '#' + Math.floor(Math.random() * 16777215).toString(16).padStart(6, '0');
    const r = parseInt(randomColor.substring(1, 3), 16);
    const g = parseInt(randomColor.substring(3, 5), 16);
    const b = parseInt(randomColor.substring(5, 7), 16);
    const isDark = (0.2126 * r + 0.7152 * g + 0.0722 * b) < 128;
    
    res.json({
        primary: randomColor,
        primaryRgb: `${r},${g},${b}`,
        textColor: isDark ? '#FFFFFF' : '#000000'
    });
});

// Random Gaudware
app.get('/api/random-gaudware', (req, res) => {
    const games = ['gaudware-cube', 'gaudware-fight'];
    res.json({ game: games[Math.floor(Math.random() * games.length)] });
});

// PDF serving
app.get('/pdf/:filename', async (req, res) => {
    try {
        const filename = decodeURIComponent(req.params.filename);
        const filepath = path.join(__dirname, 'public', 'pdfs', filename);
        await fs.access(filepath);
        res.setHeader('Content-Type', 'application/pdf');
        res.setHeader('Content-Disposition', `inline; filename="${filename}"`);
        res.sendFile(filepath);
    } catch {
        res.status(404).send('PDF not found');
    }
});

// Routes
app.get('/', (req, res) => res.sendFile(path.join(__dirname, 'public', 'index.html')));
app.get('/pdf-viewer', (req, res) => res.sendFile(path.join(__dirname, 'public', 'pdf-viewer.html')));
app.get('/gaudware-cube', (req, res) => res.sendFile(path.join(__dirname, 'public', 'gaudware-cube.html')));
app.get('/gaudware-fight', (req, res) => res.sendFile(path.join(__dirname, 'public', 'gaudware-fight.html')));

app.listen(PORT, () => console.log(`🚀 Server running on port ${PORT}`));
