#include <iostream>
#include <fstream>
#include <string>

// Create a simple HTML file for testing the web interface
int main() {
    std::ofstream htmlFile("/home/ubuntu/ElectricVehicleSimulation/ui/index.html");
    
    if (!htmlFile.is_open()) {
        std::cerr << "Failed to create index.html file" << std::endl;
        return 1;
    }
    
    htmlFile << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Electric Vehicle Dashboard</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f5f5f5;
        }
        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 20px;
        }
        .card {
            background-color: white;
            border-radius: 8px;
            padding: 15px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        .card h2 {
            margin-top: 0;
            color: #333;
            border-bottom: 1px solid #eee;
            padding-bottom: 10px;
        }
        .data-item {
            display: flex;
            justify-content: space-between;
            margin-bottom: 8px;
        }
        .data-value {
            font-weight: bold;
        }
        .controls {
            background-color: white;
            border-radius: 8px;
            padding: 15px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            margin-bottom: 20px;
        }
        button {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 10px 15px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 4px;
        }
        button.stop {
            background-color: #f44336;
        }
        button.mode {
            background-color: #2196F3;
        }
        .warning {
            color: #f44336;
            font-weight: bold;
        }
    </style>
</head>
<body>
    <h1>Electric Vehicle Dashboard</h1>
    
    <div class="controls">
        <h2>Controls</h2>
        <button id="startEngine">Start Engine</button>
        <button id="stopEngine" class="stop">Stop Engine</button>
        <button id="modeNormal" class="mode">Normal Mode</button>
        <button id="modeSport" class="mode">Sport Mode</button>
        <button id="modeEco" class="mode">Eco Mode</button>
        <button id="modeSnow" class="mode">Snow/Off-road Mode</button>
    </div>
    
    <div class="dashboard">
        <div class="card">
            <h2>Vehicle Status</h2>
            <div class="data-item">
                <span>Speed:</span>
                <span id="speed" class="data-value">0 km/h</span>
            </div>
            <div class="data-item">
                <span>Mode:</span>
                <span id="mode" class="data-value">Normal</span>
            </div>
            <div class="data-item">
                <span>Engine:</span>
                <span id="engine" class="data-value">Stopped</span>
            </div>
            <div class="data-item">
                <span>Gear:</span>
                <span id="gear" class="data-value">P</span>
            </div>
            <div class="data-item">
                <span>Distance Traveled:</span>
                <span id="distance-traveled" class="data-value">0 km</span>
            </div>
        </div>
        
        <div class="card">
            <h2>Battery</h2>
            <div class="data-item">
                <span>Charge:</span>
                <span id="battery" class="data-value">100%</span>
            </div>
            <div class="data-item">
                <span>Temperature:</span>
                <span id="battery-temp" class="data-value">25°C</span>
            </div>
            <div class="data-item">
                <span>Estimated Range:</span>
                <span id="estimated-distance" class="data-value">0 km</span>
            </div>
            <div class="data-item">
                <span>Charging:</span>
                <span id="plug-in" class="data-value">No</span>
            </div>
        </div>
        
        <div class="card">
            <h2>Safety</h2>
            <div class="data-item">
                <span>Door Lock:</span>
                <span id="door-lock" class="data-value">Unlocked</span>
            </div>
            <div class="data-item">
                <span>Seat Belt:</span>
                <span id="seat-belt" class="data-value">Unfastened</span>
            </div>
            <div class="data-item">
                <span>Brake:</span>
                <span id="brake" class="data-value">Released</span>
            </div>
            <div class="data-item">
                <span>Accelerator:</span>
                <span id="gas" class="data-value">Released</span>
            </div>
            <div class="data-item">
                <span>Warning Level:</span>
                <span id="warning" class="data-value warning">0</span>
            </div>
        </div>
        
        <div class="card">
            <h2>Environment</h2>
            <div class="data-item">
                <span>Temperature:</span>
                <span id="air-condition" class="data-value">20°C</span>
            </div>
            <div class="data-item">
                <span>Wind Speed:</span>
                <span id="wind" class="data-value">0 km/h</span>
            </div>
            <div class="data-item">
                <span>Left Signal:</span>
                <span id="signal-left" class="data-value">Off</span>
            </div>
            <div class="data-item">
                <span>Right Signal:</span>
                <span id="signal-right" class="data-value">Off</span>
            </div>
        </div>
    </div>
    
    <script>
        // Function to update dashboard with vehicle data
        function updateDashboard() {
            fetch('/api/vehicle')
                .then(response => response.json())
                .then(data => {
                    // Update vehicle status
                    document.getElementById('speed').textContent = data.speed + ' km/h';
                    document.getElementById('mode').textContent = data.mode;
                    document.getElementById('engine').textContent = (data.speed > 0 || data.gear !== 'P') ? 'Running' : 'Stopped';
                    document.getElementById('gear').textContent = data.gear;
                    document.getElementById('distance-traveled').textContent = data['distance-traveled'] + ' km';
                    
                    // Update battery
                    document.getElementById('battery').textContent = data.battery + '%';
                    document.getElementById('battery-temp').textContent = data['battery-temp'] + '°C';
                    document.getElementById('estimated-distance').textContent = data['estimated-distance'] + ' km';
                    document.getElementById('plug-in').textContent = data['plug-in'] == 1 ? 'Yes' : 'No';
                    
                    // Update safety
                    document.getElementById('door-lock').textContent = data['door-lock'] == 1 ? 'Locked' : 'Unlocked';
                    document.getElementById('seat-belt').textContent = data['seat-belt'] == 1 ? 'Fastened' : 'Unfastened';
                    document.getElementById('brake').textContent = data.brake == 1 ? 'Applied' : 'Released';
                    document.getElementById('gas').textContent = data.gas == 1 ? 'Applied' : 'Released';
                    document.getElementById('warning').textContent = data.warning;
                    
                    // Update environment
                    document.getElementById('air-condition').textContent = data['air-condition'] + '°C';
                    document.getElementById('wind').textContent = data.wind + ' km/h';
                    document.getElementById('signal-left').textContent = data['signal-left'] == 1 ? 'On' : 'Off';
                    document.getElementById('signal-right').textContent = data['signal-right'] == 1 ? 'On' : 'Off';
                })
                .catch(error => console.error('Error fetching vehicle data:', error));
        }
        
        // Update dashboard every 1 second
        setInterval(updateDashboard, 1000);
        
        // Initial update
        updateDashboard();
        
        // Engine control
        document.getElementById('startEngine').addEventListener('click', () => {
            fetch('/api/engine', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ action: 'start' }),
            })
            .then(response => response.text())
            .then(data => console.log(data))
            .catch(error => console.error('Error starting engine:', error));
        });
        
        document.getElementById('stopEngine').addEventListener('click', () => {
            fetch('/api/engine', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ action: 'stop' }),
            })
            .then(response => response.text())
            .then(data => console.log(data))
            .catch(error => console.error('Error stopping engine:', error));
        });
        
        // Mode control
        document.getElementById('modeNormal').addEventListener('click', () => {
            fetch('/api/mode', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ mode: 'normal' }),
            })
            .then(response => response.text())
            .then(data => console.log(data))
            .catch(error => console.error('Error changing mode:', error));
        });
        
        document.getElementById('modeSport').addEventListener('click', () => {
            fetch('/api/mode', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ mode: 'sport' }),
            })
            .then(response => response.text())
            .then(data => console.log(data))
            .catch(error => console.error('Error changing mode:', error));
        });
        
        document.getElementById('modeEco').addEventListener('click', () => {
            fetch('/api/mode', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ mode: 'eco' }),
            })
            .then(response => response.text())
            .then(data => console.log(data))
            .catch(error => console.error('Error changing mode:', error));
        });
        
        document.getElementById('modeSnow').addEventListener('click', () => {
            fetch('/api/mode', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ mode: 'snow' }),
            })
            .then(response => response.text())
            .then(data => console.log(data))
            .catch(error => console.error('Error changing mode:', error));
        });
    </script>
</body>
</html>)";
    
    htmlFile.close();
    std::cout << "Created index.html file for web interface" << std::endl;
    
    return 0;
}
