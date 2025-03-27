export function updateTime() {
    const timeElement = document.querySelector('.time');
    const now = new Date();
    timeElement.textContent = 
        `${String(now.getHours()).padStart(2, '0')}:${String(now.getMinutes()).padStart(2, '0')}`;
}

// Initialize time update
setInterval(updateTime, 1000);
updateTime();