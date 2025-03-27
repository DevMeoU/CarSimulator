// Warning Indicators and Status Icons Implementation
document.addEventListener('DOMContentLoaded', function() {
    // Get indicator elements
    const handbrakeIndicator = document.getElementById('handbrake-indicator');
    const seatbeltIndicator = document.getElementById('seatbelt-indicator');
    const doorIndicator = document.getElementById('door-indicator');
    const warningIndicator = document.getElementById('warning-indicator');
    
    // Get status display
    const statusDisplay = document.getElementById('status');
    
    // Initial states
    let handbrakeActive = true;
    let seatbeltFastened = false;
    let doorsLocked = false;
    let warningActive = false;
    
    // Initialize indicators
    updateIndicators();
    
    // Update all indicators
    function updateIndicators() {
        // Update handbrake indicator
        if (handbrakeIndicator) {
            if (handbrakeActive) {
                handbrakeIndicator.classList.add('active');
            } else {
                handbrakeIndicator.classList.remove('active');
            }
        }
        
        // Update seatbelt indicator
        if (seatbeltIndicator) {
            if (!seatbeltFastened) {
                seatbeltIndicator.classList.add('active');
            } else {
                seatbeltIndicator.classList.remove('active');
            }
        }
        
        // Update door indicator
        if (doorIndicator) {
            if (doorsLocked) {
                doorIndicator.classList.add('active');
            } else {
                doorIndicator.classList.remove('active');
            }
        }
        
        // Update warning indicator
        if (warningIndicator) {
            if (warningActive) {
                warningIndicator.classList.add('active');
            } else {
                warningIndicator.classList.remove('active');
            }
        }
        
        // Update status display
        updateStatusDisplay();
    }
    
    // Update status display based on current states
    function updateStatusDisplay() {
        if (!statusDisplay) return;
        
        let statusMessage = '';
        
        // Priority order: warning > handbrake > seatbelt > doors > normal operation
        if (warningActive) {
            statusMessage = 'Cảnh báo: Hệ thống cần kiểm tra';
        } else if (handbrakeActive) {
            statusMessage = 'Xe đứng yên, phanh tay đang kéo';
        } else if (!seatbeltFastened) {
            statusMessage = 'Vui lòng thắt dây an toàn';
        } else if (!doorsLocked) {
            statusMessage = 'Cửa chưa khóa, vui lòng kiểm tra';
        } else {
            statusMessage = 'Hệ thống sẵn sàng, an toàn để lái xe';
        }
        
        statusDisplay.textContent = statusMessage;
    }
    
    // Handbrake toggle (Space key)
    document.addEventListener('keydown', function(event) {
        if (event.key === ' ') {
            handbrakeActive = !handbrakeActive;
            updateIndicators();
            
            if (handbrakeActive) {
                showNotification('PHANH TAY', 'KÉO', '#ff3b30');
            } else {
                showNotification('PHANH TAY', 'NHẢ', '#4cd964');
                
                // Show handbrake release instructions
                showHandbrakeInstructions();
            }
        }
    });
    
    // Seatbelt toggle (B key)
    document.addEventListener('keydown', function(event) {
        if (event.key === 'b' || event.key === 'B') {
            seatbeltFastened = !seatbeltFastened;
            updateIndicators();
            
            if (seatbeltFastened) {
                showNotification('DÂY AN TOÀN', 'ĐÃ THẮT', '#4cd964');
            } else {
                showNotification('DÂY AN TOÀN', 'CHƯA THẮT', '#ff9500');
            }
        }
    });
    
    // Door lock toggle (L key)
    document.addEventListener('keydown', function(event) {
        if (event.key === 'l' || event.key === 'L') {
            doorsLocked = !doorsLocked;
            updateIndicators();
            
            if (doorsLocked) {
                showNotification('CỬA XE', 'ĐÃ KHÓA', '#4cd964');
            } else {
                showNotification('CỬA XE', 'ĐÃ MỞ KHÓA', '#ff9500');
            }
        }
    });
    
    // Warning toggle (W key)
    document.addEventListener('keydown', function(event) {
        if (event.key === 'w' || event.key === 'W') {
            if (event.ctrlKey) { // Ctrl+W to toggle warning
                warningActive = !warningActive;
                updateIndicators();
                
                if (warningActive) {
                    showNotification('CẢNH BÁO', 'KÍCH HOẠT', '#ff3b30');
                } else {
                    showNotification('CẢNH BÁO', 'ĐÃ TẮT', '#4cd964');
                }
            }
        }
    });
    
    // Click handlers for indicators
    if (handbrakeIndicator) {
        handbrakeIndicator.addEventListener('click', function() {
            handbrakeActive = !handbrakeActive;
            updateIndicators();
            
            if (handbrakeActive) {
                showNotification('PHANH TAY', 'KÉO', '#ff3b30');
            } else {
                showNotification('PHANH TAY', 'NHẢ', '#4cd964');
                showHandbrakeInstructions();
            }
        });
    }
    
    if (seatbeltIndicator) {
        seatbeltIndicator.addEventListener('click', function() {
            seatbeltFastened = !seatbeltFastened;
            updateIndicators();
            
            if (seatbeltFastened) {
                showNotification('DÂY AN TOÀN', 'ĐÃ THẮT', '#4cd964');
            } else {
                showNotification('DÂY AN TOÀN', 'CHƯA THẮT', '#ff9500');
            }
        });
    }
    
    if (doorIndicator) {
        doorIndicator.addEventListener('click', function() {
            doorsLocked = !doorsLocked;
            updateIndicators();
            
            if (doorsLocked) {
                showNotification('CỬA XE', 'ĐÃ KHÓA', '#4cd964');
            } else {
                showNotification('CỬA XE', 'ĐÃ MỞ KHÓA', '#ff9500');
            }
        });
    }
    
    // Show handbrake release instructions
    function showHandbrakeInstructions() {
        // Create instruction overlay if it doesn't exist
        let instructionOverlay = document.querySelector('.instruction-overlay');
        if (!instructionOverlay) {
            instructionOverlay = document.createElement('div');
            instructionOverlay.className = 'instruction-overlay';
            document.body.appendChild(instructionOverlay);
            
            const content = document.createElement('div');
            content.className = 'instruction-content';
            instructionOverlay.appendChild(content);
            
            // Add instruction content
            content.innerHTML = `
                <h3>Hướng dẫn nhả phanh tay đúng cách</h3>
                <div class="instruction-steps">
                    <div class="instruction-step">
                        <div class="step-number">1</div>
                        <div class="step-text">Đạp chân phanh và giữ chắc</div>
                    </div>
                    <div class="instruction-step">
                        <div class="step-number">2</div>
                        <div class="step-text">Nhấn nút nhả phanh tay</div>
                    </div>
                    <div class="instruction-step">
                        <div class="step-number">3</div>
                        <div class="step-text">Thả từ từ cho đến khi đèn báo tắt</div>
                    </div>
                </div>
                <div class="instruction-note">
                    <strong>Lưu ý:</strong> Luôn đảm bảo xe đã dừng hẳn và đạp phanh chân trước khi nhả phanh tay
                </div>
                <div class="instruction-close">
                    <button>Đã hiểu</button>
                </div>
            `;
            
            // Add close button handler
            const closeButton = content.querySelector('.instruction-close button');
            closeButton.addEventListener('click', function() {
                instructionOverlay.classList.remove('visible');
            });
        }
        
        // Show overlay
        instructionOverlay.classList.add('visible');
        
        // Auto-hide after 5 seconds
        setTimeout(() => {
            instructionOverlay.classList.remove('visible');
        }, 5000);
    }
    
    // Help button handler
    const helpButton = document.getElementById('help-button');
    if (helpButton) {
        helpButton.addEventListener('click', function() {
            showHelpOverlay();
        });
    }
    
    // Show help overlay
    function showHelpOverlay() {
        // Create help overlay if it doesn't exist
        let helpOverlay = document.querySelector('.help-overlay');
        if (!helpOverlay) {
            helpOverlay = document.createElement('div');
            helpOverlay.className = 'help-overlay';
            document.body.appendChild(helpOverlay);
            
            const content = document.createElement('div');
            content.className = 'help-content';
            helpOverlay.appendChild(content);
            
            // Add help content
            content.innerHTML = `
                <h2>Hướng dẫn sử dụng</h2>
                <div class="help-grid">
                    <div class="help-item">
                        <div class="help-icon">Space</div>
                        <div class="help-text">Kéo/Nhả phanh tay</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">L</div>
                        <div class="help-text">Khóa/Mở khóa cửa</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">B</div>
                        <div class="help-text">Thắt/Nhả dây an toàn</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">E</div>
                        <div class="help-text">Chế độ ECO</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">S</div>
                        <div class="help-text">Chế độ SPORT</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">↑</div>
                        <div class="help-text">Tăng tốc</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">↓</div>
                        <div class="help-text">Phanh</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">+</div>
                        <div class="help-text">Tăng nhiệt độ điều hòa</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">-</div>
                        <div class="help-text">Giảm nhiệt độ điều hòa</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">]</div>
                        <div class="help-text">Tăng mức quạt gió</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">[</div>
                        <div class="help-text">Giảm mức quạt gió</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">C</div>
                        <div class="help-text">Bật/Tắt sạc pin</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">Ctrl+W</div>
                        <div class="help-text">Bật/Tắt cảnh báo</div>
                    </div>
                    <div class="help-item">
                        <div class="help-icon">?</div>
                        <div class="help-text">Hiển thị hướng dẫn này</div>
                    </div>
                </div>
                <div class="help-close">
                    <button>Đóng</button>
                </div>
            `;
            
            // Add close button handler
            const closeButton = content.querySelector('.help-close button');
            closeButton.addEventListener('click', function() {
                helpOverlay.classList.remove('visible');
            });
        }
        
        // Show overlay
        helpOverlay.classList.add('visible');
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
        
        // Set icon based on notification type
        if (title === 'PHANH TAY') {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm-1-13h2v6h-2zm0 8h2v2h-2z" fill="currentColor"/>
            </svg>`;
        } else if (title === 'DÂY AN TOÀN') {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm-1-13h2v6h-2zm0 8h2v2h-2z" fill="currentColor"/>
            </svg>`;
        } else if (title === 'CỬA XE') {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M18 8h-1V6c0-2.76-2.24-5-5-5S7 3.24 7 6v2H6c-1.1 0-2 .9-2 2v10c0 1.1.9 2 2 2h12c1.1 0 2-.9 2-2V10c0-1.1-.9-2-2-2zm-6 9c-1.1 0-2-.9-2-2s.9-2 2-2 2 .9 2 2-.9 2-2 2zm3.1-9H8.9V6c0-1.71 1.39-3.1 3.1-3.1 1.71 0 3.1 1.39 3.1 3.1v2z" fill="currentColor"/>
            </svg>`;
        } else if (title === 'CẢNH BÁO') {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M1 21h22L12 2 1 21zm12-3h-2v-2h2v2zm0-4h-2v-4h2v4z" fill="currentColor"/>
            </svg>`;
        } else {
            iconDiv.innerHTML = `<svg viewBox="0 0 24 24" width="24" height="24">
                <path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zm4.59-12.42L10 14.17l-2.59-2.58L6 13l4 4 8-8z" fill="currentColor"/>
            </svg>`;
        }
        
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
    
    // Add CSS for instruction overlay
    const style = document.createElement('style');
    style.textContent = `
        .instruction-overlay {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background-color: rgba(0, 0, 0, 0.8);
            display: flex;
            justify-content: center;
            align-items: center;
            z-index: 1000;
            opacity: 0;
            pointer-events: none;
            transition: opacity 0.3s ease;
        }
        
        .instruction-overlay.visible {
            opacity: 1;
            pointer-events: auto;
        }
        
        .instruction-content {
            background-color: rgba(30, 30, 30, 0.9);
            border-radius: 15px;
            padding: 30px;
            width: 80%;
            max-width: 600px;
            box-shadow: 0 0 30px rgba(0, 0, 0, 0.8);
            border: 1px solid rgba(255, 255, 255, 0.1);
        }
        
        .instruction-content h3 {
            text-align: center;
            margin-top: 0;
            margin-bottom: 20px;
            font-size: 24px;
            color: white;
        }
        
        .instruction-steps {
            margin: 20px 0;
        }
        
        .instruction-step {
            display: flex;
            align-items: center;
            margin: 15px 0;
        }
        
        .step-number {
            width: 30px;
            height: 30px;
            border-radius: 50%;
            background-color: rgba(0, 122, 255, 0.8);
            display: flex;
            justify-content: center;
            align-items: center;
            margin-right: 15px;
            font-weight: bold;
        }
        
        .step-text {
            font-size: 18px;
        }
        
        .instruction-note {
            background-color: rgba(255, 149, 0, 0.2);
            padding: 15px;
            border-radius: 5px;
            margin: 20px 0;
            border-left: 4px solid rgba(255, 149, 0, 0.8);
        }
        
        .instruction-close {
            text-align: center;
            margin-top: 20px;
        }
        
        .instruction-close button {
            background-color: rgba(0, 122, 255, 0.8);
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            font-size: 16px;
            cursor: pointer;
            transition: all 0.3s ease;
        }
        
        .instruction-close button:hover {
            background-color: rgba(0, 122, 255, 1);
            transform: translateY(-2px);
        }
    `;
    document.head.appendChild(style);
    
    // Show help overlay when pressing ? key
    document.addEventListener('keydown', function(event) {
        if (event.key === '?' || event.key === '/') {
            showHelpOverlay();
        }
    });
});
