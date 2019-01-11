var InterpolationStepSize = 0.5;

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

class Timer {
  constructor(length) {
    this.length = length;
    this.start = Date.now();
  }
  expired() {
    return Date.now() >= this.start + this.length;
  }
  restart() {
    this.start = Date.now();
  }
}

class MC {
  constructor(mcID) {
    this.paramID = mcID;
    this.paramValue = 0;
    this.targetValue = undefined;
    this.onValueChanged = new Slot(mcID);
    this.onTargetChanged = new Slot(mcID);
    this.throttler = new Timer(5);
    this.ignoreSet = new Timer(50);
  }

  setTarget(val) {
    this.targetValue = val;
    this.onTargetChanged.onChange(val);
    this.ignoreSet.restart();
  }

  updateValue(val) {
    var old = this.paramValue;
    this.paramValue = val;
    if(old !== val) {
      this.sendMC(this.paramValue, this.paramID);
      this.onTargetChanged.onChange(val);
    }
  }

  setValue(val) {
    if(this.ignoreSet.expired()) {
      this.paramValue = val;
      this.targetValue = undefined;
      this.onValueChanged.onChange(val);
    }
  }

  connectValue(cb) {
    this.onValueChanged.connect(cb);
  }

  sendMC() {
    if(this.throttler.expired()) {
      var scaled = this.paramValue.toFixed(3) / 100;
      webSocket.send("/presets/param-editor/set-mc?id="+this.paramID+"&value="+scaled);
      this.throttler.restart();
    }
  }
}

class MCModel {
  constructor(webSocket) {
    this.mcs = [];

    for(var i = 0; i < 4; i++) {
      this.mcs[i] = new MC(243+i);
      this.mcs[i].onTargetChanged.connect(function(val, id) {
        model.update();
      });
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

  setTarget(id, target) {
    model.mcs[id - 243].setTarget(target);
  }

  update() {
    var i;
    for(i = 0; i < model.mcs.length; i++) {
      var mc = model.mcs[i];
      if(mc.targetValue !== undefined && mc.targetValue != mc.paramValue) {
        var oldVal = mc.paramValue;

        var n = 1.0 / (1 * InterpolationStepSize + 1.0);

        if(InterpolationStepSize !== 1) {
          mc.updateValue((1 - n) * mc.targetValue + n * mc.paramValue);
        } else {
          mc.updateValue(mc.targetValue);
        }
      }
    }

    if(mc.paramValue != mc.targetValue)
      setInterval(this.update, 16);
  }
}

class RangeDivision {
  constructor() {
    this.controls = [{"ID":0,"x":0,   "y":0,    "w":0.5,  "h":0.5,  "type":"xy",  "MCX":243,  "MCY":244},
                     {"ID":1,"x":0.5, "y":0,    "w":0.5,  "h":0.5,  "type":"none","MCX":null, "MCY":null},
                     {"ID":2,"x":0,   "y":0.5,  "w":1,    "h":0.25, "type":"x",   "MCX":245,  "MCY":null},
                     {"ID":3,"x":0,   "y":0.75, "w":1,    "h":0.25, "type":"x",   "MCX":246,  "MCY":null}];
  }
}

class MCView {
  constructor() {
    this.canvas = document.getElementById('canvas');
    this.range = new RangeDivision();

    model.mcs.forEach(function(mc) {
      mc.onValueChanged.connect(function(val, id) {
        view.redraw(model);
      });
    });

    this.body = document.getElementById('body');
    this.addEventsToElement(this.canvas);
  }

  addEventsToElement(element) {
    element.addEventListener('touchstart', function(event) {
      event.preventDefault();
      controller.touches = event.touches;
      controller.onChange();
    });

    element.addEventListener('mousemove', function(event) {
      controller.lastMouseEvent = event;
      controller.onChange();
    });

    element.addEventListener('mousedown', function() {
       controller.mouseDown = 1;
       controller.onChange();
    });

    element.addEventListener('mouseup', function() {
       controller.mouseDown = 0;
       controller.onChange();
    });

    element.addEventListener('touchmove', function(event) {
      event.preventDefault();
      controller.touches = event.touches;
      controller.onChange();
    });

    element.addEventListener('touchend', function(event) {
      controller.touches = event.touches;
      controller.onChange();
    });
  }

  redraw(model) {
    var canvas = view.canvas;
    var ctx = canvas.getContext('2d');
    var info = view.getViewInfo();
    canvas.width  = window.innerWidth;
    canvas.height = window.innerHeight;
    var width = canvas.width;
    var heigth = canvas.height;

    ctx.font = '4em nonlinearfont';
    ctx.strokeStyle = "black";
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
      ctx.lineWidth = "1";
      ctx.strokeStyle = "black";
      ctx.rect(x, y, w, h);
      ctx.stroke();

      this.drawHandle(division);
    }
  }

  drawHandle(division) {
    if(division.type == null)
      return;
    if(division.type.startsWith("xy")) {
      this.draw2DDivision(division);
    } else if(division.type.startsWith("x")) {
      this.draw1DDivision(division);
    }
  }

  draw2DDivision(division) {
    var canvas = view.canvas;
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var heigth = canvas.height;
    var x = width * division.x;
    var y = heigth * division.y;
    var w = width * division.w;
    var h = heigth * division.h;

    var xTarget = model.mcs[division.MCX - 243].targetValue;
    var yTarget = model.mcs[division.MCY - 243].targetValue;
    var xVal = model.mcs[division.MCX - 243].paramValue;
    var yVal = model.mcs[division.MCY - 243].paramValue;

    var size = canvas.width / 200 * 2;


    ctx.beginPath();
    ctx.strokeStyle = "transparent";

    if(xTarget !== undefined && yTarget !== undefined)
      ctx.fillStyle = "blue";
    else
      ctx.fillStyle = "darkblue";

    ctx.arc(w / 100 * xVal, h / 100 * yVal, size, 0, 2*Math.PI, true);
    ctx.stroke();
    ctx.fill();

    if(xTarget !== undefined && yTarget !== undefined) {
      ctx.beginPath();
      ctx.lineWidth = "2";
      ctx.strokeStyle = "gray";
      ctx.fillStyle = "transparent";
      ctx.arc(w / 100 * xTarget, h / 100 * yTarget, size, 0, 2*Math.PI, true);
      ctx.fill();
      ctx.stroke();
    }
  }

  draw1DDivision(division) {
    var canvas = view.canvas;
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var heigth = canvas.height;
    var x = width * division.x;
    var y = heigth * division.y;
    var w = width * division.w;
    var h = heigth * division.h;

    var xVal = model.mcs[division.MCX - 243].paramValue;
    var xTarget = model.mcs[division.MCX - 243].targetValue;

    if(xTarget !== undefined) {
      ctx.beginPath();
      ctx.lineWidth = "6";
      ctx.fillStyle = "transparent";
      ctx.strokeStyle = "grey";
      ctx.moveTo(w / 100 * xTarget, y + 1);
      ctx.lineTo(w / 100 * xTarget, y + h - 1);
      ctx.stroke();
      ctx.fill();
    }

    ctx.beginPath();
    ctx.fillStyle = "transparent";
    ctx.lineWidth = "3";

    if(xTarget !== undefined)
      ctx.strokeStyle = "blue";
    else
      ctx.strokeStyle = "darkblue";

    ctx.lineWidth = w / 200 * 1;
    ctx.moveTo(w / 100 * xVal, y + 1);
    ctx.lineTo(w / 100 * xVal, y + h - 1);
    ctx.stroke();
    ctx.fill();


  }

  getViewInfo() {
    var canvas = document.getElementById('canvas');
    return {"canvas-w": canvas.width, "canvas-h": canvas.height, "controls": this.range.controls}
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
    this.inputsToTargets();
    view.redraw(model);
  }

  inputsToTargets() {
    var targets = [];
    var displayInformation = view.getViewInfo();

    var i;
    for(i in displayInformation.controls) {
      var division = displayInformation.controls[i];
      var activeInputs = [];
      var cW = view.canvas.width;
      var cH = view.canvas.height;
      var rect = view.canvas.getBoundingClientRect();
      var dX = (cW * division.x) + rect.left;
      var dY = (cH * division.y) + rect.top;
      var dW = cW * division.w;
      var dH = cH * division.h;

      for(var iI in this.userInputs) {
        var input = this.userInputs[iI];

        if(input.x >= dX && input.x <= dX + dW &&
           input.y >= dY && input.y <= dY + dH) {
          activeInputs.push(input);
        }
      }

      var inputCount = activeInputs.length;
      if(inputCount <= 0 || division.type == null) {
        continue;
      }

      var rect = view.canvas.getBoundingClientRect();
      var x = -rect.left;
      var y = -rect.top;
      activeInputs.forEach(function(input) {
        x+=input.x;
        y+=input.y;
      });
      if(inputCount > 0) {
        x /= inputCount;
        y /= inputCount;
      }

      var xVal = Number(x / dW * 100).toFixed(1);
      var yVal = Number(y / dH * 100).toFixed(1);

      //To Be Safe!
      xVal = Math.min(100, Math.max(xVal, 0));
      yVal = Math.min(100, Math.max(yVal, 0));

      if(division.type.startsWith("xy")) {
        model.setTarget(division.MCX, xVal);
        model.setTarget(division.MCY, yVal);
      } else if(division.type.startsWith("x")) {
        model.setTarget(division.MCX, xVal);
      }
    }
  }

  collectInputs() {
    var activePositions = [];


    for(var i = 0; i < this.touches.length; i++) {
      var touch = this.touches[i];
      activePositions.push(new Input(touch.pageX, touch.pageY));
    }

    if(this.lastMouseEvent !== undefined) {
      if(this.mouseDown !== 0) {
        activePositions.push(new Input(this.lastMouseEvent.pageX, this.lastMouseEvent.pageY));
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
    model.update();
    window.requestAnimationFrame(function() { view.redraw(model); });
  };
}

//UI Functionality
function toggleSettings() {
  var e = document.getElementById("settings-burger");
  e.classList.toggle("collapsed");
  e.classList.toggle("full");
}

function setInterpolation(val) {
  InterpolationStepSize = val;
  document.getElementById("interpolation-step-label").innerHTML = val;
}
