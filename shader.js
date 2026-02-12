document.addEventListener('DOMContentLoaded', function() {
    // Remove loading indicator when WebGL is ready
    const loadingElement = document.querySelector('.loading');
    
    const canvas = document.getElementById('shaderCanvas');
    const gl = canvas.getContext('webgl');
    
    if (!gl) {
        console.warn('WebGL not supported - using fallback background');
        loadingElement.remove();
        return;
    }

    // Generate random parameters for each page load
    const randomParams = {
        shapeType: Math.floor(Math.random() * 6), // 0-5 for different shapes
        colorSeed: Math.random() * 100,
        speed: 0.5 + Math.random() * 1.5,
        density: 2 + Math.random() * 3,
        pattern: Math.floor(Math.random() * 3),
        lineWidth: 0.01 + Math.random() * 0.03,
        distortionAmount: 0.1 + Math.random() * 0.3,
        distortionSpeed: 0.5 + Math.random() * 2.0,
        distortionScale: 3 + Math.random() * 5
    };

    // Get the primary color from CSS
    const primaryColor = getComputedStyle(document.documentElement)
        .getPropertyValue('--primary-color').trim();
    
    // Convert hex color to RGB
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

    // Fragment shader with enhanced distortion
    const fragmentShaderSource = `
        precision highp float;
        
        uniform vec2 iResolution;
        uniform float iTime;
        uniform vec3 uRandomParams;
        
        // Distance functions
        float sdCircle(vec2 p, float r) {
            return length(p) - r;
        }
        
        float sdBox(vec2 p, vec2 b) {
            vec2 d = abs(p) - b;
            return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
        }
        
        float sdTriangle(vec2 p) {
            const float k = sqrt(3.0);
            p.x = abs(p.x) - 1.0;
            p.y = p.y + 1.0/k;
            if(p.x + k*p.y > 0.0) {
                p = vec2(p.x - k*p.y, -k*p.x - p.y)/2.0;
            }
            p.x -= clamp(p.x, -2.0, 0.0);
            return -length(p) * sign(p.y);
        }
        
        float sdHexagon(vec2 p, float r) {
            const vec3 k = vec3(-0.866025404,0.5,0.577350269);
            p = abs(p);
            p -= 2.0*min(dot(k.xy,p),0.0)*k.xy;
            p -= vec2(clamp(p.x, -k.z*r, k.z*r), r);
            return length(p)*sign(p.y);
        }
        
        float sdVerticalLine(vec2 p, float width) {
            return abs(p.x) - width;
        }
        
        float sdHorizontalLine(vec2 p, float width) {
            return abs(p.y) - width;
        }
        
        // Color palette generator
        vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
            return a + b*cos(6.28318*(c*t+d));
        }
        
        // Distortion function
        vec2 distort(vec2 uv, float time, float amount, float speed, float scale) {
            // Multi-frequency distortion
            float offset1 = amount * sin(scale * uv.y + speed * time);
            float offset2 = amount * 0.5 * sin(scale * 2.0 * uv.x + speed * 1.3 * time);
            float offset3 = amount * 0.3 * cos(scale * 0.7 * (uv.x + uv.y) + speed * 0.7 * time);
            
            uv.x += offset1 + offset2;
            uv.y += offset3;
            return uv;
        }
        
        void main() {
            vec2 uv = (gl_FragCoord.xy/iResolution - 0.5) * 2.0;
            uv.x *= iResolution.x / iResolution.y;
            
            // Apply distortion effect before other transformations
            uv = distort(
                uv,
                iTime,
                ${randomParams.distortionAmount.toFixed(2)},
                ${randomParams.distortionSpeed.toFixed(2)},
                ${randomParams.distortionScale.toFixed(2)}
            );
            
            // Apply random speed to movement
            uv += iTime * 0.1 * uRandomParams.z;
            
            // Create repeating pattern
            uv = fract(uv * ${randomParams.density.toFixed(1)}) - 0.5;
            
            // Select shape based on random parameter
            float d;
            float width = ${randomParams.lineWidth.toFixed(3)};
            
            if (uRandomParams.x < 0.5) {
                d = sdCircle(uv, 0.4);
            } else if (uRandomParams.x < 1.5) {
                d = sdBox(uv, vec2(0.3, 0.3));
            } else if (uRandomParams.x < 2.5) {
                d = sdTriangle(uv * 1.2);
            } else if (uRandomParams.x < 3.5) {
                d = sdHexagon(uv, 0.3);
            } else if (uRandomParams.x < 4.5) {
                d = sdVerticalLine(uv, width);
            } else {
                d = sdHorizontalLine(uv, width);
            }
            
            // Generate color based on position and time
            vec3 col;
            if (${randomParams.pattern} == 0) {
                // Radial color pattern using primary color
                float r = length(uv) + iTime * 0.3 * uRandomParams.z;
                vec3 a = vec3(${primaryRgb.r}, ${primaryRgb.g}, ${primaryRgb.b});
                vec3 b = vec3(0.3, 0.3, 0.3);
                vec3 c = vec3(1.0, 1.0, 1.0);
                vec3 d = vec3(uRandomParams.y, uRandomParams.y + 0.33, uRandomParams.y + 0.67);
                col = palette(r, a, b, c, d);
            } else if (${randomParams.pattern} == 1) {
                // Horizontal stripes
                float p = uv.x * 5.0 + iTime * 0.5 * uRandomParams.z;
                vec3 a = vec3(${primaryRgb.r}, ${primaryRgb.g}, ${primaryRgb.b});
                vec3 b = vec3(0.2, 0.2, 0.2);
                vec3 c = vec3(1.0, 1.0, 1.0);
                vec3 d = vec3(0.0, 0.10, 0.20) + uRandomParams.y;
                col = palette(p, a, b, c, d);
            } else {
                // Diagonal waves
                float p = (uv.x + uv.y) * 3.0 + iTime * 0.4 * uRandomParams.z;
                vec3 a = vec3(${primaryRgb.r}, ${primaryRgb.g}, ${primaryRgb.b});
                vec3 b = vec3(0.3, 0.3, 0.3);
                vec3 c = vec3(1.0, 1.0, 0.5);
                vec3 d = vec3(0.8, 0.9, 0.3) + uRandomParams.y;
                col = palette(p, a, b, c, d);
            }
            
            // Draw shape with anti-aliasing
            col = mix(col, vec3(0.0), smoothstep(width, width+0.01, abs(d)));
            
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
        
        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            console.error('Shader error:', gl.getShaderInfoLog(shader));
            return null;
        }
        return shader;
    }

    const vertexShader = compileShader(vertexShaderSource, gl.VERTEX_SHADER);
    const fragmentShader = compileShader(fragmentShaderSource, gl.FRAGMENT_SHADER);
    
    if (!vertexShader || !fragmentShader) {
        loadingElement.remove();
        return;
    }

    const program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        console.error('Program error:', gl.getProgramInfoLog(program));
        loadingElement.remove();
        return;
    }
    
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
    
    // Pass random parameters to shader
    gl.uniform3f(
        randomParamsLocation,
        randomParams.shapeType,
        randomParams.colorSeed,
        randomParams.speed
    );

    let lastTime = 0;
    function animate(currentTime) {
        currentTime *= 0.001; // Convert to seconds
        const deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        if (timeLocation) {
            gl.uniform1f(timeLocation, currentTime);
        }
        
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        
        const fps = document.hidden ? 15 : 60;
        setTimeout(() => {
            requestAnimationFrame(animate);
        }, 1000 / fps);
    }
    
    window.addEventListener('resize', function() {
        resizeCanvas();
        gl.uniform2f(resolutionLocation, canvas.width, canvas.height);
    });
    
    // Remove loading indicator when everything is ready
    loadingElement.style.opacity = '0';
    setTimeout(() => {
        loadingElement.remove();
    }, 500);
    
    requestAnimationFrame(animate);
});
