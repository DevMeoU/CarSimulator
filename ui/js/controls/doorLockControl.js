class DoorLockControl {
    constructor() {
        this.doorLocked = document.getElementById('door-locked');
        this.doorUnlocked = document.getElementById('door-unlocked');
        this.doorLockIcon = document.querySelector('.door-lock-icons');
        
        if (this.doorLockIcon) {
            this.initEvents();
        } else {
            console.error('Could not find door lock icon element');
        }
    }
    
    initEvents() {
        if (this.doorLockIcon) {
            this.doorLockIcon.addEventListener('click', () => this.toggleLock());
        }
    }
    
    toggleLock() {
        if (this.doorLocked.classList.contains('active')) {
            this.doorLocked.classList.remove('active');
            this.doorUnlocked.classList.add('active');
            this.doorLockIcon.innerHTML = '<i class="fas fa-unlock active"></i><i class="fas fa-lock"></i>';
        } else {
            this.doorUnlocked.classList.remove('active');
            this.doorLocked.classList.add('active');
            this.doorLockIcon.innerHTML = '<i class="fas fa-lock active"></i><i class="fas fa-unlock"></i>';
        }
    }
}

export default DoorLockControl;