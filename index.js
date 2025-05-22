let currentVolume = 0; // 0-100
let speaking = false;

function fakeVolume() {
	if (speaking) {
		// Small random fluctuation
		let change = (Math.random() - 0.5) * 30;
		currentVolume += change;

		// Clamp to 40-100
		currentVolume = Math.max(30, Math.min(70, currentVolume));

		if (Math.random() < 0.05) currentVolume = 80;
		// Snap back towards 60 slowly
		currentVolume += (60 - currentVolume) * 0.1;

		if (Math.random() < 0.04) speaking = false;
	} else {
		currentVolume *= 0.85;
		if (Math.random() < 0.08) speaking = true;
	}
	// Round it
	currentVolume = Math.round(currentVolume);

	// Set your image src based on currentVolume
	let level = Math.floor(currentVolume / 10); // 0-10 levels

	document.querySelector(".volume").src = `./assets/vol${level}.png`;
}

// Run every 100ms
setInterval(fakeVolume, 70);
