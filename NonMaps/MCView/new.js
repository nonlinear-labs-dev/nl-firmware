var InterpolationStepSize = 50;

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
    this.webSocket = new WebSocket('ws://192.168.8.2:80/ws/');
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
    var message = event.data;
    if(message.startsWith("MCVIEW")) {
      console.log(message);

      var id = serverProxy.getValueForKeyFromMessage(message, "ID");
      var val = Number(Number(serverProxy.getValueForKeyFromMessage(message, "VAL")) * 100).toFixed(3);
      var uuid = serverProxy.getValueForKeyFromMessage(message, "UUID");

      var mc = model.mcs[id - 243];
      if(mc !== undefined) {
        if(uuid !== serverProxy.uuid.uuid) {
          mc.active = false;
          mc.setValue(val);
        } else {
          mc.active = true;
        }
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
    this.updateThrottler = new Timer(20);
    this.callBackAfterUpdate = undefined;
    this.active = false;
  }

  setCallbackAfterUpdate(cb) {
    this.callBackAfterUpdate = cb;
  }

  setTarget(val) {
    this.targetValue = val;
    this.onTargetChanged.onChange(val);
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
    this.paramValue = val;
    this.targetValue = undefined;
    this.onValueChanged.onChange(val);
  }

  connectValue(cb) {
    this.onValueChanged.connect(cb);
  }

  sendMC() {
    //// TODO:
    if(Number(this.paramID) > 246)
      return;

    var scaled = this.paramValue.toFixed(3) / 100;
    serverProxy.send("/presets/param-editor/set-mc", "?id="+this.paramID+"&value="+scaled);
  }

  update() {
    var step = 1.0 / ((InterpolationStepSize) * 0.5 + 1.0);
		var foo = (1 - step) * this.paramValue + step * this.targetValue;
    this.updateValue(foo);


    if(this.callBackAfterUpdate) {
      this.callBackAfterUpdate(this);
    }
  }
}

class MCModel {
  constructor() {
    this.mcs = [];

    for(var i = 0; i < 6; i++) {
      this.mcs[i] = new MC(243 + i);
    }
    setInterval(this.update.bind(this), 20);
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
                     {"ID":1,"x":0.5, "y":0,    "w":0.5,  "h":0.5,  "type":"xy",  "MCX":247, "MCY":248},
                     {"ID":2,"x":0,   "y":0.5,  "w":1,    "h":0.25, "type":"x",   "MCX":245,  "MCY":null},
                     {"ID":3,"x":0,   "y":0.75, "w":1,    "h":0.25, "type":"x",   "MCX":246,  "MCY":null}];

    this.deadzones = [{"ID":0,"x":0.1,"y":0.1},
                      {"ID":1,"x":0.1,"y":0.1},
                      {"ID":2,"x":0.1,"y":0.1},
                      {"ID":3,"x":0.1,"y":0.1}];
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

  getMCForPagePos(pageX, pageY) {
    var width = view.canvas.width;
    var heigth = view.canvas.height;
    var mc = null;
    view.range.controls.forEach(function(division) {
      var x = width * division.x;
      var y = heigth * division.y;
      var w = width * division.w;
      var h = heigth * division.h;

      if(pageX >= x && pageX <= x + w  &&
         pageY >= y && pageY <= y + h) {
        mc = division.MCX;
      }
    });
    return mc;
  }

  getMCForTouch(touch) {
    return this.getMCForPagePos(touch.pageX, touch.pageY);
  }

  addEventsToElement(element) {
    element.addEventListener('touchstart', function(event) {
      var found = false;
      var newTouches = [];

      for(var i = 0; i < event.changedTouches.length; i++) {
        var changedTouch = event.changedTouches[i];
        controller.touches.forEach(function(touch) {
          if(touch.touch.identifier === changedTouch.identifier) {
            found = true;
          } else {
            newTouches.push(changedTouch);
          }
        });
      }

      var currTouch = event.changedTouches[0];
      if(currTouch) {
        var found = controller.touches.some(function(touch) {
          return touch.touch.identifier === currTouch.identifier;
        });

        if(!found) {
          var mc = view.getMCForTouch(currTouch);
          controller.touches.push({"touch":currTouch, "mc":mc});
          controller.onChange();
        }
      }
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
      var changed = false;

      event.preventDefault();
      controller.touches.forEach(function(inputTouch) {
        var touches = event.touches;
        var l = touches.length;
        for(var i = 0; i < l; i++) {
          var eventTouch = touches[i];
          if(inputTouch.touch.identifier === eventTouch.identifier) {
            inputTouch.touch = eventTouch;
            changed = true;
          }
        }
      });

      if(changed)
        controller.onChange();
    });

    element.addEventListener('touchend', function(event) {

      var endingTouch = event.changedTouches[0];

      if(endingTouch) {
        var filtered = controller.touches.filter(function(touch){
            return touch.touch.identifier !== endingTouch.identifier;
        });

        controller.touches = filtered;
        controller.onChange();
      }

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

    ctx.font = '20px nonlinearfont';
    ctx.strokeStyle = "black";
    ctx.fillStyle = "rgb(30,30,30)";
    ctx.fillRect(0, 0, width, heigth);

    var i;
    for(i in info.controls) {
      var division = info.controls[i];
      var deadZones = view.range.deadzones[i];
      var x = width * division.x;
      var y = heigth * division.y;
      var w = width * division.w;
      var h = heigth * division.h;

      ctx.beginPath();
      ctx.lineWidth = "1";
      ctx.strokeStyle = "black";

      ctx.rect(x, y, w, h);
      ctx.stroke();

      ctx.beginPath();
      ctx.fillStyle = "#2b2b2b";
      var wD = w - width * deadZones.x;
      var hD = h - heigth * deadZones.y;
      var xD = x + (w - wD) / 2;
      var yD = y + (h  - hD) / 2;

      ctx.fillRect(xD, yD, wD, hD);
      ctx.fill();
      this.drawHandle(division, xD, yD, wD, hD);
    }
  }

  drawHandle(division, xD, yD, wD, hD) {
    if(division.type == null)
      return;
    if(division.type.startsWith("xy")) {
      this.draw2DDivision(division, xD, yD, wD, hD);
    } else if(division.type.startsWith("x")) {
      this.draw1DDivision(division, xD, yD, wD, hD);
    }
  }

  draw2DDivision(division, xD, yD, wD, hD) {
    var canvas = view.canvas;
    var ctx = canvas.getContext('2d');
    var x = xD;
    var y = yD;
    var w = wD;
    var h = hD;

    var xTarget = model.mcs[division.MCX - 243].targetValue;
    var yTarget = model.mcs[division.MCY - 243].targetValue;
    var xVal = model.mcs[division.MCX - 243].paramValue;
    var yVal = model.mcs[division.MCY - 243].paramValue;
    var yLabel = getUnicodeForMC(division.MCY);
    var xLabel = getUnicodeForMC(division.MCX);

    var size = canvas.width / 200 * 2;

    ctx.strokeStyle = "lightgray";
    ctx.fillStyle = "lightgray";
    ctx.font = "20px nonlinearfont";
    var lineHeight=ctx.measureText("\uE001").width;

    //X
    var textMeasure = ctx.measureText(xLabel);
    var valMeasure = ctx.measureText(Number(xVal).toFixed(1));
    ctx.fillText(xLabel, x + w - textMeasure.width * 2, y + h - lineHeight * 2.5);
    ctx.fillText(Number(xVal).toFixed(1), x + w - textMeasure.width * 1.5 - valMeasure.width / 2, y + h - lineHeight);

    //Y
    textMeasure = ctx.measureText(yLabel);
    valMeasure = ctx.measureText(Number(yVal).toFixed(1));
    ctx.fillText(yLabel, x + textMeasure.width, y + lineHeight);
    ctx.fillText(Number(yVal).toFixed(1), x + textMeasure.width * 1.5 - valMeasure.width / 2, y + lineHeight * 2.5);


    ctx.beginPath();
    ctx.strokeStyle = "transparent";
    ctx.fillStyle = "blue";

    yTarget = 100 - yTarget;
    yVal = 100 - yVal;

    ctx.arc(x + w / 100 * xVal, y + h / 100 * yVal, size, 0, 2*Math.PI, true);
    ctx.stroke();
    ctx.fill();

    if(xTarget !== undefined && yTarget !== undefined && xTarget !== xVal || yTarget !== yVal) {
      ctx.beginPath();
      ctx.lineWidth = "3";
      ctx.strokeStyle = "gray";
      ctx.fillStyle = "transparent";
      ctx.arc(x + w / 100 * xTarget, y + h / 100 * yTarget, size, 0, 2*Math.PI, true);
      ctx.fill();
      ctx.stroke();
    }
  }

  draw1DDivision(division, xD, yD, wD, hD) {
    var canvas = view.canvas;
    var ctx = canvas.getContext('2d');
    var x = xD;
    var y = yD;
    var w = wD;
    var h = hD;

    var xVal = model.mcs[division.MCX - 243].paramValue;
    var xTarget = model.mcs[division.MCX - 243].targetValue;
    var xLabel = getUnicodeForMC(division.MCX);

    ctx.beginPath();
    ctx.strokeStyle = "lightgray";
    ctx.fillStyle = "lightgray";
    ctx.font = "20px nonlinearfont";
    var textMeasure = ctx.measureText(xLabel);
    var valMeasure = ctx.measureText(String(Number(xVal).toFixed(1)));

    var lineHeight=ctx.measureText("\uE001").width;
    ctx.fillText(xLabel, x + w / 2 - textMeasure.width / 2, y + h / 2 - lineHeight / 2);
    ctx.fillText(Number(xVal).toFixed(1), x + w / 2 - valMeasure.width / 2, y + h / 2 + lineHeight);

    if(xTarget !== undefined && xTarget !== xVal) {
      ctx.beginPath();
      ctx.lineWidth = "10";
      ctx.fillStyle = "transparent";
      ctx.strokeStyle = "grey";
      ctx.moveTo(xD + wD / 100 * xTarget, yD + 1);
      ctx.lineTo(xD + wD / 100 * xTarget, yD + hD - 1);
      ctx.stroke();
      ctx.fill();
    }

    ctx.beginPath();
    ctx.fillStyle = "transparent";
    ctx.lineWidth = "5";
    ctx.strokeStyle = "blue";
    ctx.lineWidth = w / 200 * 1;
    ctx.moveTo(x + w / 100 * xVal, y + 1);
    ctx.lineTo(x + w / 100 * xVal, y + h - 1);
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
		return "\uE200";
		case 248:
		return "\uE201";
	}
}

class Input {
  constructor(x, y, originMC) {
    this.x = x;
    this.y = y;
    this.mc = originMC;
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
      var deadzone = view.range.deadzones[i];
      var activeInputs = [];
      var cW = view.canvas.width;
      var cH = view.canvas.height;
      var rect = view.canvas.getBoundingClientRect();
      var dX = (cW * division.x);
      var dY = (cH * division.y);
      var dW = cW * division.w;
      var dH = cH * division.h;

      var ctx = view.canvas.getContext("2d");

      var wD = dW - cW * deadzone.x;
      var hD = dH - cH * deadzone.y;
      var xD = dX + (dW - wD) / 2;
      var yD = dY + (dH - hD) / 2;

      var padding = 2;

      for(var iI in this.userInputs) {
        var input = this.userInputs[iI];

        if(input.x >= xD - padding && input.x <= Number(xD) + Number(wD) + padding &&
           input.y >= yD - padding && input.y <= Number(yD) + Number(hD) + padding &&
            input.mc === division.MCX || input.mc === division.MCY && input.mc !== null) {
          activeInputs.push(input);
        }
      }

      var inputCount = activeInputs.length;
      if(inputCount <= 0 || division.type == null) {
        continue;
      }

      var x = (wD - dW) / 2;
      var y = (hD - dH) / 2;

      activeInputs.forEach(function(input) {
        x+=input.x;
        y+=input.y;
      });

      //HUH?! TODO: Investigate
      if(division.MCX === 247) {
        x -= xD;
        x += (dW - wD) / 2;
      }

      if(inputCount > 0) {
        x /= inputCount;
        y /= inputCount;
      }

      var xVal = Number(x / wD * 100).toFixed(1);
      var yVal = 100 - Number(y / hD * 100).toFixed(1);

      //Just To Be Safe!
      xVal = Math.min(100, Math.max(xVal, 0));
      yVal = Math.min(100, Math.max(yVal, 0));

      if(division.type.startsWith("xy") && division.MCX !== null && division.MCY !== null) {
        if(  xVal !== model.mcs[division.MCX - 243].targetValue ||
          yVal !== model.mcs[division.MCY - 243].targetValue) {
          model.setTarget(division.MCX, xVal);
          model.setTarget(division.MCY, yVal);
        }
      } else if(division.type.startsWith("x") && division.MCX) {
        if(xVal !== model.mcs[division.MCX - 243].targetValue) {
          model.setTarget(division.MCX, xVal);
        }
      }
    }
  }

  collectInputs() {
    var activePositions = [];

    this.touches.forEach(function(touch) {
      activePositions.push(new Input(touch.touch.pageX, touch.touch.pageY, touch.mc));
    });

    if(this.lastMouseEvent !== undefined) {
      if(this.mouseDown !== 0) {
        activePositions.push(new Input(this.lastMouseEvent.pageX, this.lastMouseEvent.pageY, view.getMCForPagePos(this.lastMouseEvent.pageX, this.lastMouseEvent.pageY)));
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
  var href = window.location.href;
  if(href.includes("settings")) {
    toggleSettings();
  }

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
    setInterval(function() {
      model.mcs.forEach(function(mc){
        var changed = false;
        if(mc !== undefined && mc.targetValue !== undefined && mc.paramValue !== undefined) {
          if(Number(mc.paramValue).toFixed(1) === Number(mc.targetValue).toFixed(1)) {
            mc.targetValue = undefined;
            changed = true;
          }
        }
        if(changed)
          view.redraw(model);
      });
    }, 50);
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

function setDeadZoneY(val) {
  view.range.deadzones.forEach(function(dz) {
    dz.y = val / 10;
  });
  document.getElementById("y-deadzone-label").innerHTML = "Deadzone Y: " + Number(val*10).toFixed(1) + "%";
}

function setDeadZoneX(val) {
  view.range.deadzones.forEach(function(dz) {
    dz.x = val / 10;
  });
  document.getElementById("x-deadzone-label").innerHTML = "Deadzone X: " + Number(val*10).toFixed(1) + "%";
}
