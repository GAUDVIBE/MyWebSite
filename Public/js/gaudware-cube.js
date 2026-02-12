// ============ LOAD DYNAMIC COLORS ============
async function loadGameColors() {
    try {
        const response = await fetch('/api/colors');
        const colors = await response.json();
        
        document.documentElement.style.setProperty('--primary-color', colors.primary);
        document.documentElement.style.setProperty('--primary-color-rgb', colors.primaryRgb);
        
        const cube = document.getElementById('cube');
        if (cube) {
            cube.style.backgroundColor = colors.primary;
        }
        
        return colors;
    } catch (error) {
        console.error('Failed to load colors:', error);
        document.documentElement.style.setProperty('--primary-color', '#3498db');
    }
}

// ============ CUBE DRAG AND DROP ============
const cube = document.getElementById('cube');
const dropZone = document.getElementById('drop-zone');

let isDragging = false;
let offsetX, offsetY;

function updateCubePosition(clientX, clientY) {
    if (!cube) return;
    
    const x = Math.max(0, Math.min(
        clientX - offsetX,
        window.innerWidth - cube.offsetWidth
    ));
    const y = Math.max(0, Math.min(
        clientY - offsetY,
        window.innerHeight - cube.offsetHeight
    ));

    cube.style.left = `${x}px`;
    cube.style.top = `${y}px`;

    // Collision detection
    if (dropZone) {
        const cubeRect = cube.getBoundingClientRect();
        const dropRect = dropZone.getBoundingClientRect();

        const isOverlapping = (
            cubeRect.right > dropRect.left &&
            cubeRect.left < dropRect.right &&
            cubeRect.bottom > dropRect.top &&
            cubeRect.top < dropRect.bottom
        );

        dropZone.classList.toggle('highlight', isOverlapping);
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

    const cubeRect = cube.getBoundingClientRect();
    const dropRect = dropZone.getBoundingClientRect();

    if (checkCollision(cubeRect, dropRect)) {
        cube.style.transform = 'scale(0.5) rotate(180deg)';
        dropZone.style.backgroundColor = '#e74c3c';
        
        setTimeout(() => {
            try {
                window.close();
                if (!window.closed) {
                    window.location.href = 'about:blank';
                }
            } catch (e) {
                console.error("Impossible de fermer l'onglet :", e);
            }
        }, 500);
    }
}

function checkCollision(rect1, rect2) {
    return (
        rect1.right > rect2.left &&
        rect1.left < rect2.right &&
        rect1.bottom > rect2.top &&
        rect1.top < rect2.bottom
    );
}

// ============ EVENT LISTENERS ============
if (cube) {
    // Mouse events
    cube.addEventListener('mousedown', (e) => {
        e.preventDefault();
        startDrag(e.clientX, e.clientY);
    });

    // Touch events
    cube.addEventListener('touchstart', (e) => {
        e.preventDefault();
        startDrag(e.touches[0].clientX, e.touches[0].clientY);
    });
}

document.addEventListener('mousemove', (e) => {
    if (!isDragging) return;
    e.preventDefault();
    updateCubePosition(e.clientX, e.clientY);
});

document.addEventListener('touchmove', (e) => {
    if (!isDragging) return;
    e.preventDefault();
    updateCubePosition(e.touches[0].clientX, e.touches[0].clientY);
}, { passive: false });

document.addEventListener('mouseup', stopDrag);
document.addEventListener('touchend', stopDrag);

// ============ WEBGL SHADER ============
document.addEventListener('DOMContentLoaded', async () => {
    await loadGameColors();
    
    const loading = document.getElementById('loading');
    const canvas = document.getElementById('shaderCanvas');
    
    if (!canvas) return;
    
    const gl = canvas.getContext('webgl');
    
    if (!gl) {
        console.warn('WebGL not supported');
        if (loading) loading.remove();
        return;
    }

    // Generate random parameters
    const randomParams = {
        shapeType: Math.floor(Math.random() * 6),
        colorSeed: Math.random() * 100,
        speed: 0.5 + Math.random() * 1.5,
        density: 2 + Math.random() * 3,
        pattern: Math.floor(Math.random() * 3),
        lineWidth: 0.01 + Math.random() * 0.03,
        distortionAmount: 0.1 + Math.random() * 0.3,
        distortionSpeed: 0.5 + Math.random() * 2.0,
        distortionScale: 3 + Math.random() * 5
    };

    // Get primary color
    const primaryColor = getComputedStyle(document.documentElement)
        .getPropertyValue('--primary-color').trim() || '#3498db';
    
    const hexToRgb = (hex) => {
        const r = parseInt(hex.substring(1, 3), 16) / 255;
        const g = parseInt(hex.substring(3, 5), 16) / 255;
        const b = parseInt(hex.substring(5, 7), 16) / 255;
        return {r, g, b};
    };
    
    const primaryRgb = hexToRgb(primaryColor);

    // Vertex shader
    const vertexShaderSource = `
        attribute vec2 aPosition;
        void main() {
            gl_Position = vec4(aPosition, 0.0, 1.0);
        }
    `;

    // Fragment shader
    const fragmentShaderSource = `
        precision highp float;
        
        uniform vec2 iResolution;
        uniform float iTime;
        uniform vec3 uRandomParams;
        
        float sdCircle(vec2 p, float r) { return length(p) - r; }
        float sdBox(vec2 p, vec2 b) { vec2 d = abs(p) - b; return length(max(d,0.0)) + min(max(d.x,d.y),0.0); }
        
        vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
            return a + b*cos(6.28318*(c*t+d));
        }
        
        vec2 distort(vec2 uv, float time, float amount, float speed, float scale) {
            uv.x += amount * sin(scale * uv.y + speed * time);
            uv.x += amount * 0.5 * sin(scale * 2.0 * uv.x + speed * 1.3 * time);
            uv.y += amount * 0.3 * cos(scale * 0.7 * (uv.x + uv.y) + speed * 0.7 * time);
            return uv;
        }
        
        void main() {
            vec2 uv = (gl_FragCoord.xy/iResolution - 0.5) * 2.0;
            uv.x *= iResolution.x / iResolution.y;
            
            uv = distort(uv, iTime, ${randomParams.distortionAmount.toFixed(2)}, 
                         ${randomParams.distortionSpeed.toFixed(2)}, ${randomParams.distortionScale.toFixed(2)});
            uv += iTime * 0.1 * uRandomParams.z;
            uv = fract(uv * ${randomParams.density.toFixed(1)}) - 0.5;
            
            float d;
            if (uRandomParams.x < 0.5) d = sdCircle(uv, 0.4);
            else d = sdBox(uv, vec2(0.3, 0.3));
            
            vec3 a = vec3(${primaryRgb.r}, ${primaryRgb.g}, ${primaryRgb.b});
            vec3 b = vec3(0.3, 0.3, 0.3);
            vec3 c = vec3(1.0, 1.0, 1.0);
            vec3 d_vec = vec3(uRandomParams.y, uRandomParams.y + 0.33, uRandomParams.y + 0.67);
            
            vec3 col = palette(length(uv) + iTime * 0.3 * uRandomParams.z, a, b, c, d_vec);
            col = mix(col, vec3(0.0), smoothstep(${randomParams.lineWidth.toFixed(3)}, 
                        ${(randomParams.lineWidth + 0.01).toFixed(3)}, abs(d)));
            
            gl_FragColor = vec4(col, 1.0);
        }
    `;

    function resizeCanvas() {
        canvas.width = Math.min(window.innerWidth, 1920);
        canvas.height = Math.min(window.innerHeight, 1080);
        gl.viewport(0, 0, canvas.width, canvas.height);
    }

    function compileShader(source, type) {
        const shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);
        return shader;
    }

    const vertexShader = compileShader(vertexShaderSource, gl.VERTEX_SHADER);
    const fragmentShader = compileShader(fragmentShaderSource, gl.FRAGMENT_SHADER);
    
    if (!vertexShader || !fragmentShader) {
        if (loading) loading.remove();
        return;
    }

    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    gl.useProgram(program);

    const vertices = new Float32Array([-1, -1, 1, -1, -1, 1, 1, 1]);
    const vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
    
    const positionLocation = gl.getAttribLocation(program, 'aPosition');
    gl.enableVertexAttribArray(positionLocation);
    gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);

    const resolutionLocation = gl.getUniformLocation(program, 'iResolution');
    const timeLocation = gl.getUniformLocation(program, 'iTime');
    const randomParamsLocation = gl.getUniformLocation(program, 'uRandomParams');
    
    resizeCanvas();
    gl.uniform2f(resolutionLocation, canvas.width, canvas.height);
    gl.uniform3f(randomParamsLocation, randomParams.shapeType, randomParams.colorSeed, randomParams.speed);

    function animate(time) {
        time *= 0.001;
        if (timeLocation) gl.uniform1f(timeLocation, time);
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        requestAnimationFrame(animate);
    }
    
    window.addEventListener('resize', () => {
        resizeCanvas();
        gl.uniform2f(resolutionLocation, canvas.width, canvas.height);
    });
    
    if (loading) {
        loading.style.opacity = '0';
        setTimeout(() => loading.remove(), 500);
    }
    
    requestAnimationFrame(animate);
});

// Hide loading on window load
window.addEventListener('load', () => {
    const loading = document.getElementById('loading');
    if (loading) {
        loading.classList.add('hidden');
        setTimeout(() => loading.remove(), 500);
    }
});
