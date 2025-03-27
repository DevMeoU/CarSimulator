// Responsive testing script
document.addEventListener('DOMContentLoaded', function() {
    // Test interface responsiveness
    testResponsiveness();
    
    // Add window resize listener
    window.addEventListener('resize', function() {
        testResponsiveness();
    });
});

// Test responsiveness
function testResponsiveness() {
    // Get window dimensions
    const windowWidth = window.innerWidth;
    const windowHeight = window.innerHeight;
    
    console.log(`Testing responsiveness at ${windowWidth}x${windowHeight}`);
    
    // Calculate optimal scale
    const scaleX = windowWidth / 1200; // Base width
    const scaleY = windowHeight / 800; // Base height
    const scale = Math.min(scaleX, scaleY);
    
    // Apply scale transform to container
    const container = document.querySelector('.container');
    if (container) {
        container.style.transform = `scale(${scale})`;
        container.style.width = `${1200}px`;
        container.style.height = `${800}px`;
        container.style.position = 'absolute';
        container.style.left = '50%';
        container.style.top = '50%';
        container.style.marginLeft = `-${1200 / 2}px`;
        container.style.marginTop = `-${800 / 2}px`;
        container.style.transformOrigin = 'center center';
        
        console.log(`Applied scale: ${scale.toFixed(2)}`);
    }
    
    // Adjust elements for different screen sizes
    if (windowWidth < 768) {
        // Mobile adjustments
        applyMobileAdjustments();
    } else if (windowWidth < 1024) {
        // Tablet adjustments
        applyTabletAdjustments();
    } else {
        // Desktop adjustments
        applyDesktopAdjustments();
    }
    
    // Test visibility of all important elements
    testElementVisibility();
}

// Apply mobile adjustments
function applyMobileAdjustments() {
    console.log('Applying mobile adjustments');
    
    // Reduce size of gauges
    const gauges = document.querySelectorAll('.gauge');
    gauges.forEach(gauge => {
        gauge.style.width = '200px';
        gauge.style.height = '200px';
    });
    
    // Adjust digital speedometer
    const digitalSpeedometer = document.querySelector('.digital-speedometer');
    if (digitalSpeedometer) {
        digitalSpeedometer.style.transform = 'scale(0.8)';
    }
    
    // Stack drive modes vertically
    const driveModes = document.querySelector('.drive-modes');
    if (driveModes) {
        driveModes.style.flexDirection = 'column';
    }
    
    // Adjust info panels
    const infoPanels = document.querySelectorAll('.info-panel');
    infoPanels.forEach(panel => {
        panel.style.width = '45%';
        panel.style.margin = '5px';
    });
}

// Apply tablet adjustments
function applyTabletAdjustments() {
    console.log('Applying tablet adjustments');
    
    // Adjust gauge size
    const gauges = document.querySelectorAll('.gauge');
    gauges.forEach(gauge => {
        gauge.style.width = '250px';
        gauge.style.height = '250px';
    });
    
    // Adjust info panels
    const infoPanels = document.querySelector('.info-panels');
    if (infoPanels) {
        infoPanels.style.flexWrap = 'wrap';
    }
}

// Apply desktop adjustments
function applyDesktopAdjustments() {
    console.log('Applying desktop adjustments');
    
    // Reset gauge size
    const gauges = document.querySelectorAll('.gauge');
    gauges.forEach(gauge => {
        gauge.style.width = '300px';
        gauge.style.height = '300px';
    });
    
    // Reset digital speedometer
    const digitalSpeedometer = document.querySelector('.digital-speedometer');
    if (digitalSpeedometer) {
        digitalSpeedometer.style.transform = 'scale(1)';
    }
    
    // Reset drive modes
    const driveModes = document.querySelector('.drive-modes');
    if (driveModes) {
        driveModes.style.flexDirection = 'column';
    }
    
    // Reset info panels
    const infoPanels = document.querySelectorAll('.info-panel');
    infoPanels.forEach(panel => {
        panel.style.width = '18%';
        panel.style.margin = '0';
    });
}

// Test element visibility
function testElementVisibility() {
    // List of critical elements to test
    const criticalElements = [
        '.top-panel',
        '.analog-gauges',
        '.digital-dashboard',
        '.battery-display',
        '.info-panels',
        '.controls-area',
        '.bottom-panel',
        '.gear-display'
    ];
    
    // Check each element
    criticalElements.forEach(selector => {
        const element = document.querySelector(selector);
        if (element) {
            const rect = element.getBoundingClientRect();
            const isVisible = (
                rect.top >= 0 &&
                rect.left >= 0 &&
                rect.bottom <= window.innerHeight &&
                rect.right <= window.innerWidth
            );
            
            console.log(`Element ${selector}: ${isVisible ? 'Visible' : 'Not fully visible'}`);
            
            // Highlight elements that are not fully visible
            if (!isVisible) {
                element.style.border = '2px solid red';
            } else {
                element.style.border = '';
            }
        } else {
            console.warn(`Element ${selector} not found`);
        }
    });
}

// Test keyboard controls
function testKeyboardControls() {
    console.log('Testing keyboard controls');
    
    // List of keys to test
    const keysToTest = [
        'w', 'a', 's', 'd',
        'ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight',
        'p', 'r', 'n', 'd',
        'e', 't', 'c',
        'h', 'l', ' ',
        'f1', 'Escape'
    ];
    
    // Simulate key presses
    keysToTest.forEach(key => {
        console.log(`Testing key: ${key}`);
        
        // Create keydown event
        const keydownEvent = new KeyboardEvent('keydown', {
            key: key,
            bubbles: true
        });
        
        // Dispatch event
        document.dispatchEvent(keydownEvent);
        
        // For keys that need keyup events
        if (['w', 's', 'ArrowUp', 'ArrowDown'].includes(key)) {
            // Create keyup event
            const keyupEvent = new KeyboardEvent('keyup', {
                key: key,
                bubbles: true
            });
            
            // Dispatch event after a short delay
            setTimeout(() => {
                document.dispatchEvent(keyupEvent);
            }, 300);
        }
    });
}

// Test touch controls
function testTouchControls() {
    console.log('Testing touch controls');
    
    // List of elements to test clicks on
    const elementsToClick = [
        '#gear-p',
        '#gear-r',
        '#gear-n',
        '#gear-d',
        '#mode-sport',
        '#mode-classic',
        '#mode-eco',
        '#handbrake',
        '#door-lock'
    ];
    
    // Simulate clicks
    elementsToClick.forEach(selector => {
        const element = document.querySelector(selector);
        if (element) {
            console.log(`Testing click on: ${selector}`);
            
            // Create click event
            const clickEvent = new MouseEvent('click', {
                bubbles: true,
                cancelable: true,
                view: window
            });
            
            // Dispatch event
            element.dispatchEvent(clickEvent);
        } else {
            console.warn(`Element ${selector} not found`);
        }
    });
}

// Run all tests
function runAllTests() {
    console.log('Running all responsiveness tests');
    
    // Test responsiveness
    testResponsiveness();
    
    // Test keyboard controls
    setTimeout(() => {
        testKeyboardControls();
    }, 1000);
    
    // Test touch controls
    setTimeout(() => {
        testTouchControls();
    }, 2000);
    
    // Log test completion
    setTimeout(() => {
        console.log('All responsiveness tests completed');
    }, 3000);
}

// Run tests when page is fully loaded
window.onload = function() {
    // Wait a bit for everything to initialize
    setTimeout(() => {
        runAllTests();
    }, 2000);
};
