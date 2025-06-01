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

const text = [
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.",
	"Another message here.",
	"Final message."
];

const display = document.getElementById("captions");
let textIndex = 0;

function writeText() {
	const words = text[textIndex].split(" ");
	let wordIndex = 0;
	display.textContent = "";

	const interval = setInterval(() => {
		if (wordIndex < words.length) {
			display.textContent += words[wordIndex] + " ";
			wordIndex++;
		} else {
			clearInterval(interval);
			textIndex = (textIndex + 1) % text.length;
			setTimeout(writeText, 1000); // wait 1s before next line
		}
	}, 275);
}

writeText();
