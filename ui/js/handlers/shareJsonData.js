var share_data = {
    speed: 0,
    mode: "eco",
    battery: 100,
    battery_temp: 25,
    gear: 'P',
    estimated_distance: 0,
    distance_traveled: 0,
    signal_left: 0,
    signal_right: 0,
    wind: 0,
    air_condition: 0,
    plug_in: 0,
    door_lock: 0,
    seat_belt: 0,
    parking_brake: 0,
}

export default share_data;
export const updateShareData = (data) => {
    share_data = data;
    console.log('Share_data', share_data);
    document.getElementsByClassName('speed').innerText = share_data.speed;
}

