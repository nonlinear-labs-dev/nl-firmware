var InterpolationStepSize = 0.5;

function guid() {
  function s4() {
    return Math.floor((1 + Math.random()) * 0x10000)
      .toString(16)
      .substring(1);
  }
  return s4() + s4() + '-' + s4() + '-' + s4() + '-' + s4() + '-' + s4() + s4() + s4();
};

class UUID {
  constructor() {
    this.uuid = guid();
    console.log(this.uuid);
  }
}

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

class ServerProxy {
  constructor(onStartCB) {
    this.webSocket = new WebSocket('ws://192.168.0.2:8080/ws/');
    this.uuid = new UUID();
    this.webSocket.onopen =  onStartCB;
    this.webSocket.onmessage = this.onMessage;
  }

  getValueForKeyFromMessage(message, key) {
    var ret = null;
    if(message.includes(key)) {
      var keyPos = message.indexOf(key + "=");
      var endPos = message.indexOf("&", keyPos);
      if(endPos !== -1) {
          ret = message.slice(keyPos + key.length + 1, endPos);
  	} else {
      	ret = message.slice(keyPos + key.length + 1);
      }
    }
    return ret;
  }

  onMessage(event) {
    var websocketData = document.getElementById("websocket-data");
    websocketData.innerHTML = serverProxy.webSocket;

    var message = event.data;
    if(message.startsWith("MCVIEW")) {
      var id = serverProxy.getValueForKeyFromMessage(message, "ID");
      var val = Number(Number(serverProxy.getValueForKeyFromMessage(message, "VAL")) * 100).toFixed(3);
      var uuid = serverProxy.getValueForKeyFromMessage(message, "UUID");

      if(uuid !== serverProxy.uuid.uuid) {
        var mc = model.mcs[id - 243];
        if(mc !== undefined) {
          mc.setValue(val);
        }
      } else {
        console.log("ignoring update that came from this MCView!");
      }
    }
  }

  send(path, message) {
    this.webSocket.send(path + message + "&uuid=" + this.uuid.uuid);
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
    this.changed = new Slot(mcID);
    this.throttler = new Timer(1);
    this.ignoreSet = new Timer(250);
    this.updateThrottler = new Timer(20);
    this.callBackAfterUpdate = undefined;
  }

  setCallbackAfterUpdate(cb) {
    this.callBackAfterUpdate = cb;
  }

  setTarget(val) {
    this.targetValue = val;
    this.onTargetChanged.onChange(val);
    this.ignoreSet.restart();
  }

  updateValue(val) {
    var old = this.paramValue;
    if(old !== val) {
      this.paramValue = val;
      this.changed.onChange();

      if(this.throttler.expired()) {
        this.sendMC();
        this.throttler.restart();
      }
    }
  }

  setValue(val) {
    if(this.ignoreSet.expired()) {
      if(!this.isCurrentlyControlled) {
        this.paramValue = val;
        this.targetValue = undefined;
        this.onValueChanged.onChange(val);
      }
    }
  }

  connectValue(cb) {
    this.onValueChanged.connect(cb);
  }

  sendMC() {
    var scaled = this.paramValue.toFixed(3) / 100;
    serverProxy.send("/presets/param-editor/set-mc", "?id="+this.paramID+"&value="+scaled);
  }

  update() {
    var n = 1.0 / (1 * InterpolationStepSize + 1.0);
    this.updateValue((1 - n) * this.targetValue + n * this.paramValue);

    if(this.callBackAfterUpdate) {
      this.callBackAfterUpdate(this);
    }
  }
}

class MCModel {
  constructor() {
    this.mcs = [];

    for(var i = 0; i < 4; i++) {
      this.mcs[i] = new MC(243 + i);
      this.mcs[i].onTargetChanged.connect(function(val, id) {
        model.mcs[id - 243].update();
      });
    }
    setInterval(this.update.bind(this), 16);
  }

  setTarget(id, target) {
    model.mcs[id - 243].setTarget(target);
  }

  update() {
    var i;
    for(i = 0; i < model.mcs.length; i++) {
      var mc = model.mcs[i];
      if(mc.targetValue !== undefined && mc.targetValue !== mc.paramValue) {
        mc.update();
      }
    }
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

      mc.changed.connect(function(val, id) {
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
    var xLabel = model.mcs[division.MCX - 243].paramID;
    var yLabel = model.mcs[division.MCY - 243].paramID;

    var size = canvas.width / 200 * 2;

    ctx.beginPath();
    ctx.strokeStyle = "lightgray";
    ctx.fillStyle = "lightgray";
    ctx.font = "20px nonlinearfont";
    var lineHeight=ctx.measureText('M').width;
    var textMeasure = ctx.measureText(xLabel);
    ctx.fillText(getUnicodeForMC(xLabel), x + w - textMeasure.width * 2, y + h - lineHeight * 2.5);
    ctx.fillText(Number(xVal).toFixed(1), x+w - textMeasure.width * 2, y + h - lineHeight);

    //Y
    textMeasure = ctx.measureText(yLabel);
    ctx.fillText(getUnicodeForMC(yLabel), x + textMeasure.width / 2, y + lineHeight);
    ctx.fillText(Number(yVal).toFixed(1), x + textMeasure.width / 2, y + lineHeight * 2.5);


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
    var xLabel = model.mcs[division.MCX - 243].paramID;

    ctx.beginPath();
    ctx.strokeStyle = "lightgray";
    ctx.fillStyle = "lightgray";
    ctx.font = "20px nonlinearfont";
    var textMeasure = ctx.measureText(xLabel);
    var lineHeight=ctx.measureText('M').width;
    ctx.fillText(getUnicodeForMC(xLabel), x + w / 2, y + h - lineHeight * 2.5);
    ctx.fillText(Number(xVal).toFixed(1), x + w / 2, y + h - lineHeight);


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
    return "A";
		return "\uE000";
		case 244:
    return "B";
		return "\uE001";
		case 245:
    return "C";
		return "\uE002";
		case 246:
    return "D";
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

      //Just To Be Safe!
      xVal = Math.min(100, Math.max(xVal, 0));
      yVal = Math.min(100, Math.max(yVal, 0));

      if(division.type.startsWith("xy") && division.MCX !== null && division.MCY !== null) {
        if(  xVal !== model.mcs[division.MCX - 243].targetValue ||
          yVal !== model.mcs[division.MCY - 243].targetValue) {
          model.setTarget(division.MCX, xVal);
          model.setTarget(division.MCY, yVal);
          model.mcs[division.MCX - 243].isCurrentlyControlled = true;
          model.mcs[division.MCY - 243].isCurrentlyControlled = true;
        }
      } else if(division.type.startsWith("x") && division.MCX) {
        if(xVal !== model.mcs[division.MCX - 243].targetValue) {
          model.setTarget(division.MCX, xVal);
          model.mcs[division.MCX - 243].isCurrentlyControlled = true;
        }
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
var serverProxy;

function onLoad() {
  document.onkeypress = function(event) {
    if(event.key == "H" || event.key == "h") {
      toggleSettings();
    }
  }

  serverProxy = new ServerProxy(function() {
    model = new MCModel(serverProxy.webSocket);
    view = new MCView();
    controller = new MCController();
    window.requestAnimationFrame(function() { view.redraw(model); });
    setInterval(model.update.bind(model), 25);
    setInterval(function() {
      model.mcs.forEach(function(mc, i, arr) {
        if(mc.targetValue) {
          if(mc.paramValue.toFixed(3) === mc.targetValue.toFixed(3))
            mc.isCurrentlyControlled = false;
        } else {
          mc.isCurrentlyControlled = false;
        }
      }, 50);
    });
  });
}

//UI Functionality
function toggleSettings() {
  var e = document.getElementById("settings-overlay");
  e.classList.toggle("collapsed");
  e.classList.toggle("full");
}

function setInterpolation(val) {
  InterpolationStepSize = val;
  document.getElementById("interpolation-step-label").innerHTML = val;
}
