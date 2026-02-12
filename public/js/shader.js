document.addEventListener('DOMContentLoaded', function() {
    const loading = document.querySelector('.loading');
    const canvas = document.getElementById('shaderCanvas');
    
    if (!canvas) {
        console.error('Canvas not found');
        if (loading) loading.remove();
        return;
    }
    
    const gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl');
    
    if (!gl) {
        console.warn('WebGL not supported');
        document.body.style.background = getComputedStyle(document.documentElement)
            .getPropertyValue('--primary-color') || '#3498db';
        if (loading) loading.remove();
        return;
    }

    // Get colors from CSS
    const primaryColor = getComputedStyle(document.documentElement)
        .getPropertyValue('--primary-color').trim() || '#3498db';
    
    // Convert hex to RGB
    const hexToRgb = (hex) => {
        const r = parseInt(hex.substring(1, 3), 16) / 255;
        const g = parseInt(hex.substring(3, 5), 16) / 255;
        const b = parseInt(hex.substring(5, 7), 16) / 255;
        return {r, g, b};
    };
    
    const rgb = hexToRgb(primaryColor);

    // Vertex shader
    const vsSource = `
        attribute vec4 aVertexPosition;
        void main() {
            gl_Position = aVertexPosition;
        }
    `;

    // Fragment shader with animated background
    const fsSource = `
        precision highp float;
        uniform vec2 uResolution;
        uniform float uTime;
        
        void main() {
            vec2 uv = gl_FragCoord.xy / uResolution.xy;
            
            // Animated waves
            float wave1 = sin(uv.x * 10.0 + uTime) * 0.1;
            float wave2 = cos(uv.y * 8.0 + uTime * 0.8) * 0.1;
            float wave3 = sin((uv.x + uv.y) * 12.0 + uTime * 0.5) * 0.1;
            
            // Colors based on your theme
            vec3 color1 = vec3(${rgb.r}, ${rgb.g}, ${rgb.b});
            vec3 color2 = vec3(${rgb.r * 0.7}, ${rgb.g * 0.7}, ${rgb.b * 0.7});
            vec3 color3 = vec3(${rgb.r * 0.4}, ${rgb.g * 0.4}, ${rgb.b * 0.4});
            
            // Mix colors based on position and waves
            vec3 final = mix(color1, color2, uv.x + wave1);
            final = mix(final, color3, uv.y + wave2);
            final += wave3 * 0.2;
            
            gl_FragColor = vec4(final, 1.0);
        }
    `;

    // Compile shaders
    function createShader(gl, source, type) {
        const shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);
        
        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            console.error('Shader compile error:', gl.getShaderInfoLog(shader));
            gl.deleteShader(shader);
            return null;
        }
        return shader;
    }

    const vertexShader = createShader(gl, vsSource, gl.VERTEX_SHADER);
    const fragmentShader = createShader(gl, fsSource, gl.FRAGMENT_SHADER);
    
    if (!vertexShader || !fragmentShader) {
        if (loading) loading.remove();
        return;
    }

    // Create program
    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        console.error('Program link error');
        if (loading) loading.remove();
        return;
    }
    
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
    }

    window.addEventListener('resize', resizeCanvas);
    resizeCanvas();

    // Animation
    let startTime = Date.now();
    
    function animate() {
        const time = (Date.now() - startTime) * 0.001; // seconds
        gl.uniform1f(timeLocation, time);
        
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        
        requestAnimationFrame(animate);
    }

    // Remove loading indicator
    if (loading) {
        loading.style.opacity = '0';
        setTimeout(() => {
            if (loading.parentNode) loading.remove();
        }, 500);
    }

    animate();
});
