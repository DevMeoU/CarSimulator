class DoorLockDisplay {
    constructor() {
        this.icon = document.querySelector('.door-lock-icons');
        this.element = document.querySelector('.door-lock-status');
    }

    update(status) {
        const lockIcons = document.querySelectorAll('.door-lock-icons i');
        if (lockIcons.length === 0) return;
        
        lockIcons.forEach(icon => {
            if (icon) icon.style.display = 'none';
        });

        const showIcon = (index) => {
            if (lockIcons[index]) {
                lockIcons[index].style.display = 'inline';
                lockIcons[index].classList.add('active'); // Add active class
            }
        };
        
        showIcon(status);

        if (this.element && this.element instanceof HTMLElement) {
            this.element.textContent = (status === 1) ? 'Locked' : 'Unlocked';
            this.element.style.color = (status === 1) ? 'red' : 'green';
        }
    }
}

export default DoorLockDisplay;