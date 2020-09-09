var colors = {
	// RGBA-Werte können hier verändert werden:
		// gestrichelte outline
		outline: "rgba(142, 144, 148, 0.5)",
		// rechtecke um controls
		rectangles: "rgba(40, 40, 40, 1)",
		// slider farben
		sliderShape: "rgba(60, 60, 70, 1.0)",
		sliderInnerShape: "rgba(40, 40, 50, 1)",
		// macro control (kreis)
		mcIndicator: "rgba(210, 200, 165, 1)",
		// target parameter position (im clipping)
		targetClippingIndicator: "rgba(240, 70, 50, 1)",
		// target parameter position
		targetIndicator: "rgba(95, 140, 230, 1)",
		// horizontale linie zwischen amount und target
		targetPosition: "rgba(210, 200, 165, 1)",
		// gefülltes dreieck/viereck um amount
		amountRange: "rgba(75, 80, 115, 1)",
		// gestrichelte horizontale linien von amount (max, min)
		amountModRange: "rgba(75, 80, 115, 1)",
		// mod amount balken links und rechts vom parameter
		amountIndicator: "rgba(75, 80, 115, 1)",
		// gestrichelte horizontale linie vom amount (im clipping)
		modulationClippingIndicator: "rgba(240, 70, 50, 1)",
		// horizontale linie zwischen macro und amount
		modulationIndicator: "rgba(210, 200, 165, 1)",
		// grüne linie im amount (verbindet rote macro- und blaue param- linie)
		modulationWeightIndicator: "rgba(210, 200, 165, 1)",
		// dunkles overlay, damit geklippte bereiche weniger stark gefärbt sind
		clipOverlay: "rgba(9, 9, 9, 0.3)",
};

var modulation = {
	values: {
		mc: 0.5, amt: 0, par: 0.5, parBase: 0.5, parCeil: 0.5, parUncl: 0.5,
	},
	cnv: "", ctx: "",
	init: function() {
		this.cnv = document.getElementById("ModulationPlot");
		window.addEventListener("resize", function(event) {modulation.redraw()});
		this.ctx = this.cnv.getContext("2d");
		this.redraw();
	},
	updateMC: function(value) {
		this.values.mc = this.calcScaledValue(value);
		this.modPar();
		document.getElementById("MacroValue").innerHTML = this.calcDisplayValue(value);
		this.redraw();
	},
	updateAmt: function(value) {
		this.values.amt = this.calcScaledValue(value);
		document.getElementById("AmountValue").innerHTML = this.calcDisplayValue(value);
		this.values.parBase = this.values.par - (this.values.amt * this.values.mc);
		this.values.parCeil = this.values.parBase + this.values.amt;
		this.redraw();
	},
	updatePar: function(value) {
		this.values.par = this.values.parUncl = this.calcScaledValue(value);
		document.getElementById("ParamValue").innerHTML = this.calcDisplayValue(value);
		this.values.parBase = this.values.par - (this.values.amt * this.values.mc);
		this.values.parCeil = this.values.parBase + this.values.amt;
		this.redraw();
	},
	clip: function(value) {
		return(Math.max(0, Math.min(1, value)));
	},
	calcDisplayValue: function(value) {
		return(parseInt(value / 10) + "." + Math.abs(value % 10));
	},
	calcRawValue: function(value) {
		return(parseInt(1000 * value));
	},
	calcScaledValue: function(value) {
		return(1e-3 * value);
	},
	modPar: function() {
		this.values.parUncl = this.values.parBase + (this.values.amt * this.values.mc);
		this.values.par = this.clip(this.values.parUncl);
		document.getElementById("Param").value = this.calcRawValue(this.values.par);
		document.getElementById("ParamValue").innerHTML = this.calcDisplayValue(document.getElementById("Param").value);
	},
	redraw: function() {
		this.cnv.width = Math.floor(window.getComputedStyle(this.cnv, null).getPropertyValue("width").replace("px",""));
		this.cnv.height = Math.floor(window.getComputedStyle(this.cnv, null).getPropertyValue("height").replace("px",""));
		
		this.ctx.clearRect(0, 0, this.cnv.width, this.cnv.height);
		
		var p = this.cnv.height / 3;
		var y0 = this.cnv.height - p;
		var h = this.cnv.height - (2 * p);
		
		var mc = this.values.mc;
		var amt = this.values.amt;
		var par = this.values.par;
		var parUncl = this.values.parUncl;
		var parBase = this.values.parBase;
		var parCeil = this.values.parCeil;
		
		// outline
		this.ctx.setLineDash([5,5]);
		this.ctx.strokeStyle = colors.outline;
		this.ctx.lineWidth = 1;
		
		this.ctx.beginPath();
		this.ctx.moveTo(0, p);
		this.ctx.lineTo(this.cnv.width, p);
		this.ctx.stroke();
		
		this.ctx.beginPath();
		this.ctx.moveTo(0,this.cnv.height - p);
		this.ctx.lineTo(this.cnv.width, this.cnv.height - p);
		this.ctx.stroke();
		
		// rectangles
		this.ctx.strokeStyle = colors.rectangles;
		this.ctx.setLineDash([1,0]);
		this.ctx.lineWidth = 1;
		
		this.ctx.strokeRect(1 * this.cnv.width / 36, p, this.cnv.width / 6, h);
		this.ctx.strokeRect(15 * this.cnv.width / 36, p, this.cnv.width / 6, h);
		this.ctx.strokeRect(29 * this.cnv.width / 36, p, this.cnv.width / 6, h);
		
		// slider shape
		this.ctx.strokeStyle = colors.sliderShape;
		this.ctx.lineWidth = 16;
		
		this.ctx.beginPath();
		this.ctx.moveTo(4 * this.cnv.width / 36, p);
		this.ctx.lineTo(4 * this.cnv.width / 36, y0);
		this.ctx.stroke();
		
		this.ctx.beginPath();
		this.ctx.moveTo(32 * this.cnv.width / 36, p);
		this.ctx.lineTo(32 * this.cnv.width / 36, y0);
		this.ctx.stroke();
		
		// slider inner shape
		this.ctx.strokeStyle = colors.sliderInnerShape;
		this.ctx.lineWidth = 12;
		
		this.ctx.beginPath();
		this.ctx.moveTo(4 * this.cnv.width / 36, p);
		this.ctx.lineTo(4 * this.cnv.width / 36, y0);
		this.ctx.stroke();
		
		this.ctx.beginPath();
		this.ctx.moveTo(32 * this.cnv.width / 36, p);
		this.ctx.lineTo(32 * this.cnv.width / 36, y0);
		this.ctx.stroke();
		
		// mc indicator
		this.ctx.fillStyle = colors.mcIndicator;
		this.ctx.beginPath();
		this.ctx.arc(4 * this.cnv.width / 36, y0 - (h * mc), 6, 0, 2 * Math.PI);
		this.ctx.fill();
		
		this.ctx.lineWidth = 8;
		
		// target clipping indicator
		this.ctx.strokeStyle = colors.targetClippingIndicator;
		this.ctx.beginPath();
		this.ctx.moveTo(32 * this.cnv.width / 36, y0);
		this.ctx.lineTo(32 * this.cnv.width / 36, y0 - (h * parUncl));
		this.ctx.stroke();
		
		// target indicator
		this.ctx.strokeStyle = colors.targetIndicator;
		this.ctx.beginPath();
		this.ctx.moveTo(32 * this.cnv.width / 36, y0);
		this.ctx.lineTo(32 * this.cnv.width / 36, y0 - (h * par));
		this.ctx.stroke();
		
		// amount range
		this.ctx.fillStyle = colors.amountRange,
		this.ctx.beginPath();
		this.ctx.moveTo(15 * this.cnv.width / 36, p);
		this.ctx.lineTo(21 * this.cnv.width / 36, y0 - (h * parCeil));
		this.ctx.lineTo(21 * this.cnv.width / 36, y0 - (h * parBase));
		this.ctx.lineTo(15 * this.cnv.width / 36, y0);
		this.ctx.closePath();
		this.ctx.fill();
		
		// amount mod range
		this.ctx.setLineDash([5,5]);
		this.ctx.lineWidth = 1;
		this.ctx.strokeStyle = colors.amountModRange;
		
		this.ctx.beginPath();
		this.ctx.moveTo(21 * this.cnv.width / 36, y0 - (h * parBase));
		this.ctx.lineTo(4 + 30.5 * this.cnv.width / 36, y0 - (h * parBase));
		this.ctx.stroke();
		
		this.ctx.beginPath();
		this.ctx.moveTo(21 * this.cnv.width / 36, y0 - (h * parCeil));
		this.ctx.lineTo(4 + 30.5 * this.cnv.width / 36, y0 - (h * parCeil));
		this.ctx.stroke();
		
		// modulation clipping indicator
		this.ctx.strokeStyle = colors.modulationClippingIndicator;
		this.ctx.beginPath();
		this.ctx.moveTo(21 * this.cnv.width / 36, y0 - (h * parUncl));
		this.ctx.lineTo(29 * this.cnv.width / 36, y0 - (h * parUncl));
		this.ctx.stroke();
		
		this.ctx.setLineDash([1,0]);
		this.ctx.lineWidth = 2;
		
		// modulation Indicator
		this.ctx.beginPath();
		this.ctx.strokeStyle = colors.modulationIndicator;
		this.ctx.moveTo(7 * this.cnv.width / 36, y0 - (h * mc));
		this.ctx.lineTo(15 * this.cnv.width / 36, y0 - (h * mc));
		this.ctx.stroke();
		
		// modulation weight indicator
		this.ctx.strokeStyle = colors.modulationWeightIndicator;
		this.ctx.beginPath();
		this.ctx.moveTo(15 * this.cnv.width / 36, y0 - (h * mc));
		this.ctx.lineTo(21 * this.cnv.width / 36, y0 - (h * parUncl));
		this.ctx.stroke();
		
		// target position
		this.ctx.strokeStyle = colors.targetPosition;
		this.ctx.beginPath();
		this.ctx.moveTo(21 * this.cnv.width / 36, y0 - (h * par));
		this.ctx.lineTo(29 * this.cnv.width / 36, y0 - (h * par));
		this.ctx.stroke();
		
		this.ctx.lineWidth = 8;
		
		// amount indicator
		this.ctx.strokeStyle = colors.amountIndicator;
		
		this.ctx.beginPath();
		this.ctx.moveTo(30.75 * this.cnv.width / 36, y0 - (h * parBase));
		this.ctx.lineTo(30.75 * this.cnv.width / 36, y0 - (h * parCeil));
		this.ctx.stroke();
		
		this.ctx.beginPath();
		this.ctx.moveTo(33.25 * this.cnv.width / 36, y0 - (h * parBase));
		this.ctx.lineTo(33.25 * this.cnv.width / 36, y0 - (h * parCeil));
		this.ctx.stroke();
		
		// clip overlay
		this.ctx.fillStyle = colors.clipOverlay;
		this.ctx.beginPath();
		this.ctx.fillRect(0.125 * this.cnv.width, 0, this.cnv.width, p - 1);
		
		this.ctx.fillStyle = colors.clipOverlay;
		this.ctx.beginPath();
		this.ctx.fillRect(0.125 * this.cnv.width, y0 + 1, this.cnv.width, p - 1);
	},
};

window.onload = function() {
	modulation.init();
};