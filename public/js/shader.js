// ============ SHADER.JS - DEBUG VERSION ============
console.log('🔥🔥🔥 SHADER.JS IS LOADED! 🔥🔥🔥');
console.log('Timestamp:', new Date().toISOString());

document.addEventListener('DOMContentLoaded', function() {
    console.log('✅ DOMContentLoaded fired');
    
    const loading = document.querySelector('.loading');
    const canvas = document.getElementById('shaderCanvas');
    
    console.log('🔍 Canvas element:', canvas);
    
    if (!canvas) {
        console.error('❌ Canvas not found!');
        if (loading) loading.remove();
        
        // Create a fallback colored background
        document.body.style.background = 'linear-gradient(45deg, #3498db, #9b59b6)';
        return;
    }
    
    const gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl');
    console.log('🔍 WebGL context:', gl ? '✅ Created' : '❌ Failed');
    
    if (!gl) {
        console.warn('❌ WebGL not supported');
        document.body.style.background = 'linear-gradient(45deg, #e74c3c, #f39c12)';
        if (loading) loading.remove();
        return;
    }

    // Get colors from CSS
    const primaryColor = getComputedStyle(document.documentElement)
        .getPropertyValue('--primary-color').trim() || '#3498db';
    console.log('🎨 Primary color:', primaryColor);
    
    // Convert hex to RGB
    const hexToRgb = (hex) => {
        const r = parseInt(hex.substring(1, 3), 16) / 255;
        const g = parseInt(hex.substring(3, 5), 16) / 255;
        const b = parseInt(hex.substring(5, 7), 16) / 255;
        return {r, g, b};
    };
    
    const rgb = hexToRgb(primaryColor);
    console.log('🎨 RGB values:', rgb);

    // Vertex shader
    const vsSource = `
        attribute vec4 aVertexPosition;
        void main() {
            gl_Position = aVertexPosition;
        }
    `;

    // Fragment shader with VERY VISIBLE animation
    const fsSource = `
        precision highp float;
        uniform vec2 uResolution;
        uniform float uTime;
        
        void main() {
            vec2 uv = gl_FragCoord.xy / uResolution.xy;
            
            // VERY VISIBLE animation - bright colors!
            float r = sin(uv.x * 10.0 + uTime) * 0.5 + 0.5;
            float g = cos(uv.y * 8.0 + uTime * 0.7) * 0.5 + 0.5;
            float b = sin((uv.x + uv.y) * 12.0 + uTime * 0.5) * 0.5 + 0.5;
            
            // Use theme color with high visibility
            vec3 theme = vec3(${rgb.r}, ${rgb.g}, ${rgb.b});
            
            // Mix theme with bright colors
            vec3 final = mix(theme, vec3(r, g, b), 0.7);
            
            gl_FragColor = vec4(final, 1.0);
        }
    `;

    // Compile shaders
    function createShader(gl, source, type) {
        console.log('🔧 Compiling shader...');
        const shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);
        
        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            console.error('❌ Shader compile error:', gl.getShaderInfoLog(shader));
            return null;
        }
        console.log('✅ Shader compiled successfully');
        return shader;
    }

    const vertexShader = createShader(gl, vsSource, gl.VERTEX_SHADER);
    const fragmentShader = createShader(gl, fsSource, gl.FRAGMENT_SHADER);
    
    if (!vertexShader || !fragmentShader) {
        console.error('❌ Failed to compile shaders');
        if (loading) loading.remove();
        return;
    }

    // Create program
    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        console.error('❌ Program link error');
        if (loading) loading.remove();
        return;
    }
    
    console.log('✅ Program linked successfully');
    gl.useProgram(program);

    // Set up geometry
    const vertices = new Float32Array([
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    ]);
    
    const vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
    
    const positionLocation = gl.getAttribLocation(program, 'aVertexPosition');
    gl.enableVertexAttribArray(positionLocation);
    gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);

    // Get uniform locations
    const resolutionLocation = gl.getUniformLocation(program, 'uResolution');
    const timeLocation = gl.getUniformLocation(program, 'uTime');

    // Resize function
    function resizeCanvas() {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
        gl.viewport(0, 0, canvas.width, canvas.height);
        gl.uniform2f(resolutionLocation, canvas.width, canvas.height);
        console.log('📐 Canvas resized:', canvas.width, 'x', canvas.height);
    }

    window.addEventListener('resize', resizeCanvas);
    resizeCanvas();

    // Animation
    let startTime = Date.now();
    let frameCount = 0;
    
    function animate() {
        const time = (Date.now() - startTime) * 0.001;
        gl.uniform1f(timeLocation, time);
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        
        frameCount++;
        if (frameCount % 60 === 0) {
            console.log('🎬 Animation frame', frameCount, 'time:', time.toFixed(2));
        }
        
        requestAnimationFrame(animate);
    }

    // Remove loading indicator
    if (loading) {
        loading.style.opacity = '0';
        setTimeout(() => {
            if (loading.parentNode) loading.remove();
        }, 500);
    }

    console.log('🚀 Starting animation...');
    animate();
    console.log('✅ Shader initialization complete!');
});

console.log('📦 shader.js execution finished');
