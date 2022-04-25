const ui = (function() {
    const
        ui = document.querySelector("svg#mod-scheme"),
        createValue = function(param) {
            return {
                get name() { return param.getAttribute("name"); },
                get min() { return parseInt(param.dataset.min); },
                get max() { return parseInt(param.dataset.max); },
                getValue() { return parseInt(param.dataset.value); },
                setValue(unclipped) {
                    this.unclipped = unclipped;
                    param.dataset.value = Math.max(this.min, Math.min(this.max, unclipped));
                },
                get bipolar() { return this.min < 0; },
                get default() { return this.bipolar ? 0 : 500; },
                unclipped: parseInt(param.dataset.value),
                draw() {}
            }
        },
        createParam = function(param) {
            return {
                ...createValue(param),
                get param() { return param; },
                get header() { return param.querySelector("header > a"); },
                get footer() { return param.querySelector(".value"); },
                get meter() { return ui.querySelector(`[param="${this.name}"][type="meter"]`); },
                get handle() { return ui.querySelector(`[param="${this.name}"][type="handle"]`); },
                get modamt() { return ui.querySelectorAll(`[param="${this.name}"][type="modamt"]`); },
                get mouseHandle() { return ui.querySelector(`[param="${this.name}"].handle`); }
            };
        },
        params = [...ui.querySelectorAll(".maps .param")].map((param) => createParam(param)).reduce((out, param) => {
            out[param.name] = {
                ...param,
                draw() {
                    const
                        normalValue = this.getValue() * 1e-3,
                        position = 50 - (100 * (this.bipolar ? 0.5 + (0.5 * normalValue) : normalValue)),
                        displayValue = (this.getValue() * 0.1).toFixed(1);
                    this.meter.setAttribute("y2", position);
                    this.handle.setAttribute("y1", position);
                    this.handle.setAttribute("y2", position);
                    this.footer.textContent = displayValue;
                    if(this.callback) this.callback();
                }
            };
            return out;
        }, {
            upper: createValue(ui.querySelector('.maps [name="upper"]')),
            lower: createValue(ui.querySelector('.maps [name="lower"]'))
        }),
        editor = {
            focus: "macro",
            runAnimation: false,
            get current() {
                return params[this.focus];
            },
            ...createParam(ui.querySelector(".edit .param")),
            draw() {
                const
                    normalValue = this.current.getValue() * 1e-3,
                    position = -138 + (276 * (this.current.bipolar ? 0.5 + (0.5 * normalValue) : normalValue)),
                    displayValue = (this.current.getValue() * 0.1).toFixed(1);;
                this.meter.setAttribute("x1", this.current.bipolar ? 0 : -138);
                this.meter.setAttribute("x2", position);
                this.handle.setAttribute("x1", position);
                this.handle.setAttribute("x2", position);
                this.footer.textContent = displayValue;
            }
        },
        graphs = (function(arr) {
            const
                polygon = arr.find((item) => item.tagName.toLowerCase() === "polygon"),
                polyline = arr.find((item) => item.tagName.toLowerCase() === "polyline");
            return {
                items: arr,
                draw() {
                    const
                        macro = 50 - (params.macro.getValue() * 0.1),
                        upperUnclipped = 50 - (params.upper.unclipped * 0.1),
                        upper = 50 - (params.upper.getValue() * 0.1),
                        lowerUnclipped = 50 - (params.lower.unclipped * 0.1),
                        lower = 50 - (params.lower.getValue() * 0.1),
                        paramUnclipped = 50 - (params.param.unclipped * 0.1),
                        param = 50 - (params.param.getValue() * 0.1),
                        polylineCoords = [
                            [-395, macro], [-150, macro], [150, paramUnclipped], [150, param],
                            [395, param]
                        ].join(" "),
                        polygonCoords = [
                            [-325, -50], [-150, -50], [150, upperUnclipped], [305, upperUnclipped],
                            [305, lowerUnclipped], [150, lowerUnclipped], [-150, 50], [-325, 50]
                        ].join(" ");
                    //
                    polyline.setAttribute("points", polylineCoords);
                    polygon.setAttribute("points", polygonCoords);
                    for(const amt of params.param.modamt) {
                        amt.setAttribute("y1", upper);
                        amt.setAttribute("y2", lower);
                    }
                }
            };
        })([...ui.querySelectorAll(".graph")]),
        presets = [...ui.querySelectorAll(".presets > li")],
        animation = {
            id: null,
            time: 0,
            increment: 1,
            run(timestamp) {
                if(editor.runAnimation) {
                    const delta = timestamp - this.time;
                    this.time = timestamp;
                    const value = editor.current.getValue();
                    const factor = (1 + editor.current.bipolar) * delta;
                    if(value === editor.current.max) {
                        this.increment *= -1;
                    }
                    if(value === editor.current.min) {
                        this.increment *= -1;
                    }
                    // console.log(value, this.increment);
                    editor.current.onChange(value + (this.increment * factor));
                    requestAnimationFrame((timestamp) => animation.run(timestamp));
                }
            }
        };
    //
    for(const item of ui.querySelectorAll("[tab]")) {
        item.addEventListener("mousedown", () => main.select(item.getAttribute("tab")));
    }
    //
    params.macro.onChange = function(value) {
        const param = params.lower.unclipped + (value * 1e-3 * params.amount.getValue());
        this.setValue(value);
        params.param.setValue(param);
        main.draw();
    };
    params.amount.onChange = function(value) {
        const
            lower = params.param.getValue() - (1e-3 * value * params.macro.getValue()),
            upper = lower + value;
        this.setValue(value);
        params.lower.setValue(lower);
        params.upper.setValue(upper);
        main.draw();
    };
    params.upper.onChange = function(value) {
        const
            lower = params.lower.getValue(),
            amount = value - lower,
            param = lower + (1e-3 * amount * params.macro.getValue());
        this.setValue(value);
        params.amount.setValue(amount);
        params.param.setValue(param);
        main.draw();
    };
    params.lower.onChange = function(value) {
        const
            upper = params.upper.getValue(),
            amount = upper - value,
            param = value + (1e-3 * amount * params.macro.getValue());
        this.setValue(value);
        params.amount.setValue(amount);
        params.param.setValue(param);
        main.draw();
    };
    params.param.onChange = function(value) {
        const
            lower = value - (1e-3 * params.amount.getValue() * params.macro.getValue()),
            upper = lower + params.amount.getValue();
        this.setValue(value);
        params.lower.setValue(lower);
        params.upper.setValue(upper);
        main.draw();
    };
    for(const param of [params.macro, params.amount, params.param]) {
        for(const item of [param.param, param.mouseHandle]) {
            item.addEventListener("mousedown", () => main.select(param.name));
        }
        param.mouseHandle.addEventListener("mousedown", (event) => {
            if(event.buttons === 1) {
                param.param.classList.add("active");
            }
        });
        param.mouseHandle.addEventListener("dblclick", () => {
            param.onChange(param.default);
        });
        param.mouseHandle.addEventListener("mousemove", (event) => {
            if((event.buttons !== 0) && param.param.classList.contains("active")) {
                const { top, height } = param.mouseHandle.getBoundingClientRect();
                const normal = 1 - Math.max(0, Math.min(1, (event.clientY - top - 2) / (height - 4)));
                const rawValue = Math.round(1e3 * (param.bipolar ? ((2 * normal) - 1) : normal));
                param.onChange(rawValue);
            }
        });
        param.mouseHandle.addEventListener("mouseup", (event) => {
            param.param.classList.remove("active");
        });
    }
    editor.mouseHandle.addEventListener("mousedown", (event) => {
        if(event.buttons === 1) {
            editor.param.classList.add("active");
        }
    });
    editor.mouseHandle.addEventListener("dblclick", () => {
        editor.current.onChange(editor.current.default);
    });
    editor.mouseHandle.addEventListener("mousemove", (event) => {
        if((event.buttons === 1) && editor.param.classList.contains("active")) {
            const { left, width } = editor.mouseHandle.getBoundingClientRect();
            const normal = Math.max(0, Math.min(1, (event.clientX - left - 2) / (width - 4)));
            const rawValue = Math.round(1e3 * (editor.current.bipolar ? ((2 * normal) - 1) : normal));
            editor.current.onChange(rawValue);
        }
    });
    editor.mouseHandle.addEventListener("mouseup", (event) => {
        editor.param.classList.remove("active");
    });
    //
    for(const item of ui.querySelectorAll(".settings > a")) {
        function draw() {
            switch(item.getAttribute("setting")) {
                case "graph":
                    for(const graph of graphs.items) {
                        graph.classList.toggle("hidden", item.classList.contains("transparent"));
                    }
                    break;
                case "clipping":
                    ui.querySelector(".clipped").classList.toggle("show", !item.classList.contains("transparent"));
                    break;
                case "animate":
                    editor.runAnimation = !item.classList.contains("transparent");
                    if(editor.runAnimation) {
                        animation.id = requestAnimationFrame((timestamp) => {
                            animation.time = timestamp;
                            animation.run(timestamp);
                        });
                    } else {
                        cancelAnimationFrame(animation.id);
                    }
                    break;
            }
        }
        item.addEventListener("mousedown", () => {
            item.classList.toggle("transparent");
            const state = item.classList.contains("transparent");
            if((item.getAttribute("setting") === "graph")) {
                const clip = ui.querySelector('[setting="clipping"]');
                clip.classList.toggle("disabled", state);
                if(state) {
                    clip.classList.toggle("transparent", true);
                    ui.querySelector(".clipped").classList.toggle("show", false);
                }
            }
            draw();
        });
    }
    for(const preset of presets) {
        preset.addEventListener("mousedown", () => main.load(presets.indexOf(preset)));
    }
    const main = {
        draw() {
            for(const param of Object.values(params)) param.draw();
            editor.draw();
            graphs.draw();
        },
        select(name) {
            if(name in params) {
                ui.querySelector(".maps .selected")?.classList.remove("selected");
                ui.querySelector("[tab][selected]")?.removeAttribute("selected");
                ui.querySelector(`[tab="${name}"]`).toggleAttribute("selected", true);
                editor.focus = name;
                editor.draw();
                if(["upper", "lower"].includes(name)) {
                    params.amount.param.classList.add("selected");
                } else {
                    params[name].param.classList.add("selected");
                }
            }
        },
        load(presetNumber) {
            if(presetNumber in presets) {
                ui.querySelector(".presets > [selected]")?.removeAttribute("selected");
                presets[presetNumber].toggleAttribute("selected", true);
                const state = Object.entries(presets[presetNumber].dataset).reduce((out, [name, value]) => {
                    out[name] = parseInt(value);
                    return out;
                }, {});
                state.lower = parseInt(state.param - (1e-3 * state.amount * state.macro));
                state.upper = state.lower + state.amount;
                params.macro.setValue(state.macro);
                params.amount.setValue(state.amount);
                params.upper.setValue(state.upper);
                params.lower.setValue(state.lower);
                params.param.setValue(state.param);
                this.draw();
            }
        }
    };
    // init
    main.select(editor.focus);
    main.load(1);
    //
    return main;
})();
