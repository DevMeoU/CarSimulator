// Main JavaScript file to integrate all components
document.addEventListener('DOMContentLoaded', function() {
    // Show loading screen
    const loadingOverlay = document.getElementById('loading-overlay');
    
    // Hide modern dashboard initially and show hybrid dashboard
    const modernDashboard = document.querySelector('.modern-dashboard');
    const hybridDashboard = document.querySelector('.hybrid-dashboard');
    
    if (modernDashboard) {
        modernDashboard.style.display = 'none';
    }
    
    if (hybridDashboard) {
        hybridDashboard.style.display = 'flex';
    }
    
    // Update time display
    function updateTime() {
        const timeDisplay = document.getElementById('time');
        if (timeDisplay) {
            const now = new Date();
            const hours = now.getHours().toString().padStart(2, '0');
            const minutes = now.getMinutes().toString().padStart(2, '0');
            timeDisplay.textContent = `${hours}:${minutes}`;
        }
        
        // Update every minute
        setTimeout(updateTime, 60000);
    }
    
    // Update author display
    function updateAuthor() {
        const authorDisplay = document.getElementById('author');
        if (authorDisplay) {
            authorDisplay.textContent = 'Người dùng';
        }
    }
    
    // Initialize dashboard
    function initDashboard() {
        // Update time
        updateTime();
        
        // Update author
        updateAuthor();
        
        // Hide loading screen after 2 seconds
        setTimeout(() => {
            if (loadingOverlay) {
                loadingOverlay.classList.add('hidden');
            }
        }, 2000);
        
        // Add CSS to ensure author is not overwritten
        const style = document.createElement('style');
        style.textContent = `
            #author {
                font-weight: bold;
                color: white;
                text-shadow: 0 0 5px rgba(255, 255, 255, 0.5);
            }
        `;
        document.head.appendChild(style);
        
        // Add keyboard shortcut to toggle between dashboards
        document.addEventListener('keydown', function(event) {
            if (event.key === 'Tab' && event.shiftKey) {
                event.preventDefault();
                toggleDashboard();
            }
        });
        
        // Add responsive design adjustments
        window.addEventListener('resize', adjustLayout);
        adjustLayout();
    }
    
    // Toggle between modern and hybrid dashboards
    function toggleDashboard() {
        if (modernDashboard && hybridDashboard) {
            if (modernDashboard.style.display === 'none') {
                modernDashboard.style.display = 'flex';
                hybridDashboard.style.display = 'none';
                showNotification('GIAO DIỆN', 'HIỆN ĐẠI', '#007aff');
            } else {
                modernDashboard.style.display = 'none';
                hybridDashboard.style.display = 'flex';
                showNotification('GIAO DIỆN', 'THỂ THAO-CỔ ĐIỂN', '#ff9500');
            }
        }
    }
    
    // Adjust layout based on screen size
    function adjustLayout() {
        const width = window.innerWidth;
        const height = window.innerHeight;
        
        // Calculate optimal scale
        const scaleX = width / 1200; // Base width
        const scaleY = height / 800; // Base height
        const scale = Math.min(scaleX, scaleY);
        
        // Apply scale transform to container
        const container = document.querySelector('.container');
        if (container) {
            container.style.transform = `scale(${scale})`;
            container.style.transformOrigin = 'center center';
        }
        
        // Log screen dimensions and scale
        console.log(`Screen dimensions: ${width}x${height}, Scale: ${scale.toFixed(2)}`);
        
        // Apply specific adjustments for different screen sizes
        if (width < 768) {
            // Mobile adjustments
            applyMobileLayout();
        } else if (width < 1024) {
            // Tablet adjustments
            applyTabletLayout();
        } else {
            // Desktop adjustments
            applyDesktopLayout();
        }
    }
    
    // Apply mobile layout adjustments
    function applyMobileLayout() {
        console.log('Applying mobile layout');
        
        // Adjust speedometer size
        const speedometer = document.querySelector('.speedometer');
        if (speedometer) {
            speedometer.style.width = '200px';
            speedometer.style.height = '200px';
        }
        
        // Adjust battery display
        const batteryDisplay = document.querySelector('.battery-display');
        if (batteryDisplay) {
            batteryDisplay.style.padding = '10px';
        }
        
        // Adjust temperature display
        const temperatureDisplay = document.querySelector('.temperature-display');
        if (temperatureDisplay) {
            temperatureDisplay.style.padding = '10px';
        }
        
        // Adjust warning indicators
        const warningIndicators = document.querySelector('.warning-indicators');
        if (warningIndicators) {
            warningIndicators.style.flexWrap = 'wrap';
        }
    }
    
    // Apply tablet layout adjustments
    function applyTabletLayout() {
        console.log('Applying tablet layout');
        
        // Adjust speedometer size
        const speedometer = document.querySelector('.speedometer');
        if (speedometer) {
            speedometer.style.width = '250px';
            speedometer.style.height = '250px';
        }
        
        // Reset other adjustments
        const batteryDisplay = document.querySelector('.battery-display');
        if (batteryDisplay) {
            batteryDisplay.style.padding = '15px';
        }
        
        const temperatureDisplay = document.querySelector('.temperature-display');
        if (temperatureDisplay) {
            temperatureDisplay.style.padding = '15px';
        }
    }
    
    // Apply desktop layout adjustments
    function applyDesktopLayout() {
        console.log('Applying desktop layout');
        
        // Reset all adjustments
        const speedometer = document.querySelector('.speedometer');
        if (speedometer) {
            speedometer.style.width = '300px';
            speedometer.style.height = '300px';
        }
        
        const batteryDisplay = document.querySelector('.battery-display');
        if (batteryDisplay) {
            batteryDisplay.style.padding = '15px';
        }
        
        const temperatureDisplay = document.querySelector('.temperature-display');
        if (temperatureDisplay) {
            temperatureDisplay.style.padding = '15px';
        }
        
        const warningIndicators = document.querySelector('.warning-indicators');
        if (warningIndicators) {
            warningIndicators.style.flexWrap = 'nowrap';
        }
    }
    
    // Show notification
    function showNotification(title, message, color) {
        // Create notification element if it doesn't exist
        let notification = document.querySelector('.button-press');
        if (!notification) {
            notification = document.createElement('div');
            notification.className = 'button-press';
            document.body.appendChild(notification);
            
            const iconDiv = document.createElement('div');
            iconDiv.className = 'button-press-icon';
            notification.appendChild(iconDiv);
            
            const textDiv = document.createElement('div');
            textDiv.className = 'button-press-text';
            notification.appendChild(textDiv);
        }
        
        // Update content
        const iconDiv = notification.querySelector('.button-press-icon');
        const textDiv = notification.querySelector('.button-press-text');
        
        iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
            <path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm4.59-12.42L10 14.17l-2.59-2.58L6 13l4 4 8-8z" fill="currentColor"/>
        </svg>`;
        
        textDiv.textContent = `${title}: ${message}`;
        
        // Set color
        notification.style.borderLeft = `4px solid ${color}`;
        
        // Show and animate
        notification.classList.remove('visible');
        void notification.offsetWidth; // Trigger reflow
        notification.classList.add('visible');
        
        // Remove after animation
        setTimeout(() => {
            notification.classList.remove('visible');
        }, 2000);
    }
    
    // Initialize dashboard
    initDashboard();
    
    // Add link to all script files
    function loadScript(src) {
        return new Promise((resolve, reject) => {
            const script = document.createElement('script');
            script.src = src;
            script.onload = resolve;
            script.onerror = reject;
            document.head.appendChild(script);
        });
    }
    
    // Load all scripts
    Promise.all([
        loadScript('/js/driving-modes.js'),
        loadScript('/js/climate-battery.js'),
        loadScript('/js/warning-indicators.js')
    ]).then(() => {
        console.log('All scripts loaded successfully');
    }).catch(error => {
        console.error('Error loading scripts:', error);
    });
});
