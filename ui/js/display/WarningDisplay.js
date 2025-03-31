class WarningDisplay {
    constructor() {
        this.warningElement = document.getElementsByClassName('warning-message')[0];
        this.warningMessages = {
            0: '',
            1: 'CẢNH BÁO: Lỗi hệ thống',
            2: 'CẢNH BÁO: Mất áp suất dầu',
            3: 'CẢNH BÁO: Nhiệt độ động cơ cao',
            4: 'CẢNH BÁO: Pin yếu',
            5: 'CẢNH BÁO: Lỗi phanh'
        };
    }

    update(warningStatus) {
        if (!this.warningElement) {
            console.error('Warning element not found in DOM');
            return;
        }
        
        const warningAlert = document.querySelector('.warning-alert');
        if (!warningAlert) {
            console.error('Warning alert element not found in DOM');
            return;
        }
        
        if (warningStatus >= 1) {
            this.warningElement.classList.add('active');
            this.warningElement.textContent = this.warningMessages[warningStatus] || 'CẢNH BÁO KHÔNG XÁC ĐỊNH';
            this.warningElement.style.display = 'inline-flex';
            warningAlert.style.display = 'inline-flex';
        } else {
            this.warningElement.classList.remove('active');
            this.warningElement.textContent = '';
            warningAlert.classList.remove('active');
            this.warningElement.style.display = 'none';
            warningAlert.style.display = 'none';
        }
    }
}

export default WarningDisplay;