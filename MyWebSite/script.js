document.addEventListener('DOMContentLoaded', function() {
    const terminal = document.getElementById('terminal');
    let currentInput = null;
    let commandHistory = [];
    let historyIndex = -1;
    
    // Clear terminal and add initial messages
    terminal.innerHTML = '';
    addLine('WARNING: This executes real commands on the server. Use with caution!', 'warning');
    addLine('Web Terminal - Type \'help\' for available commands');

    // Client-side commands
    const localCommands = {
        help: () => 'Available commands: help, clear\nAll other commands will be executed on server',
        clear: () => {
            terminal.innerHTML = '';
            addLine('WARNING: This executes real commands on the server. Use with caution!', 'warning');
            addLine('Web Terminal - Type \'help\' for available commands');
            createPrompt();
            return '';
        }
    };

    function addLine(text, className = '') {
        const line = document.createElement('div');
        line.className = `terminal-line ${className}`;
        line.textContent = text;
        terminal.appendChild(line);
        terminal.scrollTop = terminal.scrollHeight;
    }

    function createPrompt() {
        // Only create prompt if one doesn't already exist
        if (document.querySelector('.terminal-input')) return;

        const promptLine = document.createElement('div');
        promptLine.className = 'terminal-line';
        
        const promptSpan = document.createElement('span');
        promptSpan.className = 'prompt';
        promptSpan.textContent = '$';
        
        const input = document.createElement('input');
        input.type = 'text';
        input.className = 'terminal-input';
        
        promptLine.appendChild(promptSpan);
        promptLine.appendChild(input);
        terminal.appendChild(promptLine);
        
        currentInput = input;
        currentInput.focus();
        
        currentInput.addEventListener('keydown', function(e) {
            if (e.key === 'Enter') {
                const command = e.target.value.trim();
                if (command) {
                    commandHistory.push(command);
                    historyIndex = commandHistory.length;
                }
                addLine('$ ' + command);
                
                // Handle local commands
                const cmdParts = command.split(' ');
                if (localCommands[cmdParts[0]]) {
                    const result = localCommands[cmdParts[0]](cmdParts.slice(1));
                    if (result) addLine(result);
                }
                // Send other commands to server
                else if (command) {
                    executeCommand(command);
                }
                
                // Clear the input and create new prompt
                currentInput.removeEventListener('keydown', arguments.callee);
                promptLine.remove();
                createPrompt();
            }
            // Command history navigation
            else if (e.key === 'ArrowUp') {
                e.preventDefault();
                if (historyIndex > 0) {
                    historyIndex--;
                    currentInput.value = commandHistory[historyIndex];
                }
            }
            else if (e.key === 'ArrowDown') {
                e.preventDefault();
                if (historyIndex < commandHistory.length - 1) {
                    historyIndex++;
                    currentInput.value = commandHistory[historyIndex];
                } else {
                    historyIndex = commandHistory.length;
                    currentInput.value = '';
                }
            }
        });
    }

    function executeCommand(command) {
        fetch('execute.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: `command=${encodeURIComponent(command)}`
        })
        .then(response => response.text())
        .then(output => {
            addLine(output);
        })
        .catch(error => {
            addLine('Error executing command: ' + error, 'warning');
        });
    }

    // Initialize the terminal with a single prompt
    createPrompt();
});
