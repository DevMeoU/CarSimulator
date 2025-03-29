export function initOdoDisplay(traveled, estimated) {
    const odo_traveled = document.getElementsByClassName('distance-travelled-indicator')[0];
    const odo_estimated = document.getElementsByClassName('distance-estimate-indicator')[0];

    if (odo_traveled && typeof traveled !== 'undefined') {
        odo_traveled.textContent = traveled.toString();
    }

    if (odo_estimated && typeof estimated !== 'undefined') {
        odo_estimated.textContent = estimated.toString();
    }
}
