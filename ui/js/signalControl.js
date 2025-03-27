export function initSignalControl() {
    console.log('[SIGNAL] Khởi tạo hệ thống đèn signal');

    let leftSignalActive = false;
    let rightSignalActive = false;

    // Kiểm tra xung đột event listener
console.log('[SIGNAL] Đăng ký event listener cho keydown');
document.addEventListener('keydown', (e) => {
        console.log(`[DEBUG] Key pressed: ${e.key}, Code: ${e.code}`);
        console.log(`Trạng thái phím trước khi xử lý - V: ${leftSignalActive}, B: ${rightSignalActive}`);
        
        if (e.key.toLowerCase() === 'v') {
            e.stopImmediatePropagation();
            e.preventDefault();
            leftSignalActive = !leftSignalActive;
            const signal = document.querySelector('.signal-left');
            if (signal) {
                leftSignalActive ? signal.classList.add('active') : signal.classList.remove('active');
            } else {
                console.error('LỖI: Không tìm thấy phần tử .signal-left trong DOM');
            console.log('Kiểm tra lại HTML có chứa phần tử với class này không');
            }
        } else if (e.key.toLowerCase() === 'b') {
            e.stopImmediatePropagation();
            e.preventDefault();
            rightSignalActive = !rightSignalActive;
            const signal = document.querySelector('.signal-right');
            if (signal) {
                rightSignalActive ? signal.classList.add('active') : signal.classList.remove('active');
            } else {
                console.error('LỖI: Không tìm thấy phần tử .signal-right trong DOM');
            console.log('Kiểm tra lại HTML có chứa phần tử với class này không');
            }
        }
    });
}