export function initPedalDisplay(brake, gas) {
    const brakePedal = document.getElementById('brake-pedal');
    const gasPedal = document.getElementById('gas-pedal');
    if (brakePedal && brake) {
        brakePedal.classList.add('active');
    } else {
        brakePedal.classList.remove('active');
    }

    if (gasPedal && gas) {
        gasPedal.classList.add('active'); 
    } else {
        gasPedal.classList.remove('active');
    }
}