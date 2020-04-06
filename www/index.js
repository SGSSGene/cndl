let createWebSocket = function(url) {
	let path = "ws://" + window.location.host + "/" + url
	console.log("opening new ws " + path)

	let ws = new WebSocket(path)
	ws.binaryType = 'arraybuffer';

	ws.onmessage = function(evt) {
		console.log("received data");
	}
}
createWebSocket("ws");

