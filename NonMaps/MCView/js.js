var canvas;
var ctx;
var w = 0;
var h = 0;
var timer;
var updateStarted = false;
var touches = [];
var modRanges = [];

class Rect {
	constructor(x,y,w,h) {
		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}

	contains(x, y) {
		return this.x < x && x < this.x + this.w && this.y < y && y < this.y + this.h;
	}
}

class ModRange {
	constructor(id, idX, idY) {
		this.id = id;
		this.idX = idX;
		this.idY = idY;
		this.valueX = 0;
		this.valueY = 0;
		this.realX = 0;
		this.realY = 0;
	}

	accumilateTouches(touches) {
		let len = touches.length;
		var touchesOfInterest = [];
		let myRect = getModRect(this.id);

		for(var i = 0; i < len; i++) {
			if(myRect.contains(touches[i].pageX, touches[i].pageY)) {
				touchesOfInterest.push(touches[i]);
			}
		}
		return touchesOfInterest;
	}

	update(touches) {
		var filteredToches = this.accumilateTouches(touches);

		if(filteredToches.length > 0)
		{
			this.valueX = 0;
			this.valueY = 0;
			this.realX = 0;
			this.realY = 0;
		} else {
			return;
		}

		for(var i = 0; i < filteredToches.length; i++) {
			this.realX += filteredToches[i].pageX;
			this.realY += filteredToches[i].pageY;
		}

		if(filteredToches.length > 0) {
			this.realX /= filteredToches.length;
			this.realY /= filteredToches.length;
		}

		var rect = getModRect(this.id)
		this.valueX = (((this.realX - rect.x) / rect.w) * 100).toFixed(2);
		this.valueY = (((this.realY - rect.y) / rect.h) * 100).toFixed(2);


		if(this.id == 0)
			handle2DMc(this);
		else if(this.id == 2 || this.id == 3)
			handle1DMc(this);
	}
}

function drawTouch(ctx, touch) {
	var px = touch.pageX;
	var py = touch.pageY;
	ctx.beginPath();
	ctx.arc(px, py, 40, 0, 2*Math.PI, true);
	ctx.fillStyle = "rgba(0, 0, 200, 0.2)";
	ctx.fill();
	ctx.lineWidth = 2.0;
	ctx.strokeStyle = "rgba(0, 0, 200, 0.8)";
	ctx.stroke();
}

function updateCanvasSize() {
	var nw = window.innerWidth;
	var nh = window.innerHeight;
	if ((w != nw) || (h != nh)) {
		w = nw;
		h = nh;
		canvas.style.width = w+'px';
		canvas.style.height = h+'px';
		canvas.style.marginTop = '-8px';
		canvas.style.marginLeft = '-8px';
		canvas.style.marginRight = '0px';
		canvas.style.marginBottom = '0px';
		canvas.width = w;
		canvas.height = h;
		document.body.style.margin = "0";
		document.body.style.height = h+'px';
		document.body.style.width = w+'px';
	}
}

function sendToPlayground(id, value) {
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open("GET", "http://192.168.8.2:80/presets/param-editor/set-param?id="+id+"&value="+value, true);
	xmlHttp.send();
}

function getModRect(id) {
	var ret;
	let margin = w / 100; // 1%
	switch(id) {
		case 0:
			ret = new Rect(0,0,w/2,h/2);
			break;
		case 1:
			ret = new Rect(w/2, 0, w/2, h/2);
			break;
		case 2:
			ret = new Rect(0, h/2, w, h/4);
		break;
		case 3:
			ret = new Rect(0, h/2 + h/4, w, h/4);
		break;
		default:
			ret = new Rect(0, 0, 0, 0);
	}
	return ret;
}

function drawModRangeValue(ctx, modRange) {
	ctx.fillStyle = "Black";
	var rect = getModRect(modRange.id);
	ctx.beginPath();
	if(modRange.id == 2 || modRange.id == 3) {
		ctx.arc(modRange.realX, rect.y + rect.h / 2, 10, 0, 2*Math.PI, true);
	} else {
		ctx.arc(modRange.realX, modRange.realY, 10, 0, 2*Math.PI, true);
	}
	ctx.fill();
	ctx.stroke();
}

function drawModRanges(ctx) {
	var colors = ["#FF0000", "#48FF00", "#ff00b6", "#00ffbb"];

	ctx.beginPath();
	for(var i = 0; i < 4; i++) {
		ctx.fillStyle = colors[i];
		var rect = getModRect(i);
		var modRange = modRanges[i];

		ctx.fillRect(rect.x, rect.y, rect.w, rect.h);
		ctx.fillStyle = "Black";
		if(modRange.idX != null)
		ctx.fillText(modRange.idX + ":" + modRange.valueX, rect.x + rect.w / 2, rect.y + rect.h - 1);

		if(modRange.idY != null)
		ctx.fillText(modRange.idY + ": " + modRange.valueY, rect.x + 10, rect.y + rect.h / 2);

	}
}

function update() {
	if (updateStarted)
		return;
	updateStarted = true;

	updateCanvasSize();

	ctx.clearRect(0, 0, w, h);

	drawModRanges(ctx);
	var i, len = modRanges.length;
	for(i=0; i<len;i++) {
		modRanges[i].update(touches);
		drawModRangeValue(ctx, modRanges[i]);
	}

	updateStarted = false;
}

/*
243 - a
244 - b
245 - c
246 - d
*/

function handle1DMc(modRange) {
	var value = (modRange.valueX / 100).toFixed(2);
	var mcId;
	if(modRange.id == 2)
		mcId = 245;
	else
		mcId = 246;

	sendToPlayground(mcId, value);
}

function handle2DMc(modRange) {
	var value1 = (modRange.valueX / 100).toFixed(2);
	var value2 = (modRange.valueY / 100).toFixed(2);
	var mc1 = 243;
	var mc2 = 244;

	sendToPlayground(mc1, value1);
	sendToPlayground(mc2, value2);
}

function sendValues() {
	for(var i = 0; i < modRanges.length; i++) {
		var modRange = modRanges[i];
		if(modRange.id == 0)
			handle2DMc(modRange);
		else if(modRange.id == 2 || modRange.id == 3)
			handle1DMc(modRange);
	}
}

var sendTimer;

function onLoad() {
	canvas = document.getElementById('canvas');
	ctx = canvas.getContext('2d');
	timer = setInterval(update, 1);

	modRanges = [new ModRange(0, 'A', 'B'), new ModRange(1, 'Kein', 'Kein'), new ModRange(2, 'C', null), new ModRange(3, 'D', null)];


	canvas.addEventListener('touchmove', function(event) {
  		event.preventDefault();
  		touches = event.touches;
	});
};
