// Update time every second
function updateTime() {
    const timeElement = document.querySelector('.time');
    const now = new Date();
    timeElement.textContent = 
        `${String(now.getHours()).padStart(2, '0')}:${String(now.getMinutes()).padStart(2, '0')}`;
}
setInterval(updateTime, 1000);
updateTime();

let leftSignalActive = false;
let rightSignalActive = false;

// Xử lý sự kiện xi nhan toggle
document.addEventListener('keydown', (e) => {
    if (e.key.toLowerCase() === 'v') {
        leftSignalActive = !leftSignalActive;
        const signal = document.querySelector('.signal-left');
        leftSignalActive ? signal.classList.add('active') : signal.classList.remove('active');
    } else if (e.key.toLowerCase() === 'b') {
        rightSignalActive = !rightSignalActive;
        const signal = document.querySelector('.signal-right');
        rightSignalActive ? signal.classList.add('active') : signal.classList.remove('active');
    }
});