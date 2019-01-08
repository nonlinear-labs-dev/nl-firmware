class Slot {
  constructor(id) {
    this.paramID = id;
    this.callbacks = [];
  }
  connect(cb) {
    this.callbacks.push(cb);
  }
  onChange(val) {
    for(var i = 0; i < this.callbacks.length; i++) {
      var cb = this.callbacks[i];
      cb(val, this.paramID);
    }
  }
}

class MC {
  constructor(mcID) {
    this.paramID = mcID;
    this.paramValue = 0;
    this.targetValue = undefined;
    this.onValueChanged = new Slot(mcID);
    this.onTargetChanged = new Slot(mcID);
  }

  setTarget(val) {
    this.targetValue = val;
    this.onTargetChanged.onChange();
  }

  setValue(val) {
    this.paramValue = val;
    this.onValueChanged.onChange(val);
  }

  connectValue(cb) {
    this.onValueChanged.connect(cb);
  }
}

class MCModel {
  constructor(webSocket) {
    this.mcs = [];

    for(var i = 0; i < 4; i++) {
      this.mcs[i] = new MC(243+i);
    }

    webSocket.onmessage = this.onMessage;
  }

  onMessage(event) {
    var text = event.data;
    if(text.startsWith("MCVIEW")) {
      var val = Number(text.substr(10)*100).toFixed(1);
      var id = Number(text.substr(6, 3));
      var mc = model.mcs[id - 243]; //have to use model, because 'this' is not MCModel but WebSocket!
      if(mc !== undefined) {
        mc.setValue(val);
      }
    }
  }
}

class RangeDivision {
  constructor() {
    this.controls = [{"ID":0,"x":0,   "y":0,    "w":0.5,  "h":0.5,  "type":"xy",  "MCX":243,  "MCY":244},
                     {"ID":1,"x":0.5, "y":0,    "w":0.5,  "h":0.5,  "type":"none",  "MCX":null, "MCY":null},
                     {"ID":2,"x":0,   "y":0.5,  "w":1,    "h":0.25, "type":"x",   "MCX":245,  "MCY":null},
                     {"ID":3,"x":0,   "y":0.75, "w":1,    "h":0.25, "type":"x",   "MCX":246,  "MCY":null}];
  }
}

class MCView {
  constructor() {
    this.canvas = document.getElementById('canvas');
    this.range = new RangeDivision();

    model.mcs.forEach(function(mc) {
      mc.connectValue(function(val, id) {
        view.redraw(model);
      });
    });

    this.canvas.addEventListener('touchstart', function(event) {
  		event.preventDefault();
  		controller.touches = event.touches;
      controller.onChange();
  	});

  	this.canvas.addEventListener('mousemove', function(event) {
  		controller.lastMouseEvent = event;
      controller.onChange();
  	});

  	this.canvas.addEventListener('mousedown', function() {
  	   controller.mouseDown = 1;
       controller.onChange();
  	});

  	this.canvas.addEventListener('mouseup', function() {
  	   controller.mouseDown = 0;
       controller.onChange();
  	});

  	this.canvas.addEventListener('touchmove', function(event) {
  		event.preventDefault();
  		controller.touches = event.touches;
      controller.onChange();
  	});

  	this.canvas.addEventListener('touchend', function(event) {
  		controller.touches = event.touches;
      controller.onChange();
  	});
  }

  redraw(model) {
    var canvas = view.canvas;
    var ctx = canvas.getContext('2d');
    var info = view.getViewInfo();
    var width = canvas.width;
    var heigth = canvas.height;

    ctx.font = '4em nonlinearfont';
    ctx.fillStyle = "#2b2b2b";
    ctx.fillRect(0, 0, width, heigth);

    var i;
    for(i in info.controls) {
      var division = info.controls[i];
      var x = width * division.x;
      var y = heigth * division.y;
      var w = width * division.w;
      var h = heigth * division.h;
      ctx.beginPath();
      ctx.strokeStyle = "#00000";
      ctx.moveTo(x, y);
      ctx.lineTo(x+w, y);
      ctx.lineTo(x+w, h+y);
      ctx.lineTo(x, h+y);
      ctx.lineTo(x, y);
      ctx.stroke();

      this.drawHandle(division);
    }
  }

  drawHandle(division) {
    var canvas = view.canvas;
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var heigth = canvas.height;
    var x = width * division.x;
    var y = heigth * division.y;
    var w = width * division.w;
    var h = heigth * division.h;

    ctx.save();
    ctx.beginPath();

    if(division.type.startsWith("xy")) {
      var xTarget = model.mcs[division.MCX - 243].targetValue;
      var yTarget = model.mcs[division.MCY - 243].targetValue;

      var xVal = model.mcs[division.MCX - 243].paramValue;
      var yVal = model.mcs[division.MCY - 243].paramValue;
    	ctx.fillStyle = "blue";
    	ctx.arc(w / 100 * xVal, h / 100 * yVal, canvas.width / 200 * 2, 0, 2*Math.PI, true);
      ctx.fill();

      if(xTarget !== undefined && yTarget !== undefined) {
        ctx.strokeStyle = "gray";
        ctx.fillStyle = "transparent";
        ctx.lineWidth = w / 200 * 2;
        ctx.arc(w / 100 * xTarget, h / 100 * yTarget, canvas.width / 200 * 3, 0, 2*Math.PI, true);
      }

    } else if(division.type.startsWith("x")) {
      var xVal = model.mcs[division.MCX - 243].paramValue;
    	ctx.strokeStyle = "blue";
      ctx.lineWidth = w / 200 * 1;
    	ctx.moveTo(w / 100 * xVal, y + 1);
      ctx.lineTo(w / 100 * xVal, y + h - 1);

      ctx.strokeStyle = "black";
      ctx.fillStyle = "gray";
      var mcText = String(getUnicodeForMC(mcId);
      var valueText = String(xVal);
      var maxWidth = Math.max(, );
      ctx.fillText(mcText, w / 2 - ctx.measureText(mcText) / 2, h / 2);
      ctx.fillText(valueText, w / 2 - ctx.measureText(valueText) / 2, h / 1.5);
    }

    ctx.fill();
    ctx.stroke();
    ctx.restore();

  }

  getViewInfo() {
    return {"canvas-w": this.canvas.width, "canvas-h": this.canvas.height, "controls": this.range.controls}
  }
}

function getUnicodeForMC(mcId) {
	switch(mcId) {
		case 243:
		return "\uE000";
		case 244:
		return "\uE001";
		case 245:
		return "\uE002";
		case 246:
		return "\uE003";
		case 247:
		return "\u039B";
		case 248:
		return "\uE181";
	}
}

class Input {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
}

class MCController {
  constructor() {
    this.touches = [];
    this.lastMouseEvent = null;
    this.mouseDown = 0;
    this.userInputs = null;
  }

  onChange() {
    this.userInputs = this.collectInputs();
    //model.setTargets(this.inputsToTargets());
    view.redraw(model);
  }

  inputsToTargets() {
    var targets = [];
    var displayInformation = view.getViewInfo();

    for(var input in this.userInputs) {

    }
    return targets;
  }

  collectInputs() {
    var activePositions = [];

    for(var touch in this.touches) {
      activePositions.push(new Input(touch.pageX, touch.pageY));
    }

    if(this.lastMouseEvent !== undefined) {
      if(this.mouseDown !== 0) {
        activePositions.push(new Input(this.lastMouseEvent.clientX, this.lastMouseEvent.clientY));
      }
    }

    return activePositions;
  }
}



var webSocket;
var model;
var view;
var controller;

function onLoad() {
  webSocket = new WebSocket('ws://192.168.0.2:8080/ws/');
  webSocket.onopen = function() {
    model = new MCModel(webSocket);
    view = new MCView();
    controller = new MCController();
  };
}
