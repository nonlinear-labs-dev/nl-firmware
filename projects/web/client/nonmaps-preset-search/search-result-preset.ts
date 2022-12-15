import { ReactiveVar } from "meteor/reactive-var";
import { globals, glue } from "../glue";
import "./search.html";
import { cachedSearch, getPresetHeight, scrollY } from "./shared-state";

const presetCSVFormat = "text/preset-csv";
var multipleSelection = new ReactiveVar<Array<string> | null>(null);
var multipleSelectionStartedByShiftClick = false;
const db = globals.syncedDatabase;
const pg = globals.playgroundProxy;

function getFilteredMultipleSelection() {
    const c = cachedSearch.get();
    return multipleSelection.get()!.filter(v => c.includes(v));
}

class Presenter {
    constructor(idx: number) {
        const search = cachedSearch.get();
        const scroll = scrollY.get();
        const offset = scroll / getPresetHeight();

        idx = Math.floor(idx + offset);
        const id = cachedSearch.get()[idx];

        if (id) {
            var preset = db.queryItem("/preset/" + id)!;
            var bankUuid = preset['bank-uuid'];
            var eb = db.queryItem("/editbuffer")!;
            var pm = db.queryItem("/preset-manager")!;
            var bank = db.queryItem("/bank/" + bankUuid)!;
            var presetNumber = bank['presets'].indexOf(id) + 1;
            var number = bank['order-number'].toString().padStart(2, "0") + " - " + presetNumber.toString().padStart(3, "0");
            var typeSign = "";

            if (preset['type'] == "Layer")
                typeSign = "=";
            else if (preset['type'] == "Split")
                typeSign = "ᐦ";

            var ms = multipleSelection.get();
            var presetSelected = ms == null && bank['selected-preset'] == id && pm['selected-bank'] == bankUuid;
            var presetLoaded = ms == null && eb['loaded-preset'] == id;

            if (ms)
                presetSelected = ms.includes(id);

            this.id = id;
            this.number = number;
            this.name = preset['name-suffixed'];
            this.selected = presetSelected;
            this.loaded = presetLoaded;
            this.color = preset['attributes']['color'];
            this.type = typeSign;
        }
    }

    id: string = "";
    number: string = "";
    name: string = "";
    selected: boolean = false;
    loaded: boolean = false;
    color: string = "none";
    type: string = "";
}

const attributes = {
    "presetAttributes"(presenter: Presenter) {
        return {
            'class': ['preset ', presenter.selected ? "selected " : "", presenter.loaded ? "loaded " : ""],
            'id': presenter.id
        }
    },
    "colorAttributes"(presenter: Presenter) {
        return {
            'class': ['color ', presenter.color]
        }
    }
};

enum DragMode {
    None,
    DragPresets
}

class InnerState {
    constructor(public droppedMultipleSelection: boolean) {
    }

    public pointerDownX = 0;
    public pointerDownY = 0;
    public pointerY = 0;
    public dragMode = DragMode.None;
    public dataTransfer = new DataTransfer();
}

function getElementAt(x: number, y: number) {
    var el = document.elementFromPoint(x, y);
    while (el && getComputedStyle(el).pointerEvents == 'none') {
        el = el.parentElement;
    }
    return el;
}

function dispatchDragEvent(target: Element | null, srcEvent: PointerEvent, d: DataTransfer, type: string) {
    if (target) {
        var init = { dataTransfer: d, clientX: srcEvent.clientX, clientY: srcEvent.clientY, pageX: srcEvent.pageX, pageY: srcEvent.pageY };
        var e = new DragEvent(type, init);
        target.dispatchEvent(e);
    }
}

const events = {
    "pointerdown"(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
        innerState!.pointerDownX = event.clientX!;
        innerState!.pointerDownY = event.clientY!;
        innerState!.pointerY = event.clientY!;
        console.log("capture pointer ", event['originalEvent'].pointerId, " for element " + event['currentTarget']);
        event['currentTarget']?.setPointerCapture(event['originalEvent'].pointerId);
        event.preventDefault();
    },
    "pointermove"(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
        if (event.buttons) {
            const diffFromStartX = Math.abs(innerState!.pointerDownX - event.clientX!);
            const diffFromStartY = Math.abs(innerState!.pointerDownY - event.clientY!);
            const hysteresis = document.getElementById("hysteresis")?.clientWidth || 10;

            if (innerState?.dragMode == DragMode.DragPresets) {
                const target = getElementAt(event.pageX!, event.pageY!);
                dispatchDragEvent(target, event['originalEvent'], innerState.dataTransfer, "drag");

                if (target != this._lastTarget) {
                    dispatchDragEvent(this._lastTarget, event['originalEvent'], innerState.dataTransfer, "dragleave");
                    this._lastTarget = target;
                    dispatchDragEvent(target, event['originalEvent'], innerState.dataTransfer, "dragenter");
                }
                dispatchDragEvent(target, event['originalEvent'], innerState.dataTransfer, "dragover");
                event.stopPropagation();
            }
            else if (diffFromStartX > hysteresis && (innerState?.dragMode == DragMode.None)) {
                innerState!.dragMode = DragMode.DragPresets;
                dispatchDragEvent(event['currentTarget'], event['originalEvent'], innerState.dataTransfer, 'dragstart');
                event.stopPropagation();
            }
            else if ((diffFromStartY > hysteresis && (innerState?.dragMode == DragMode.None))) {
                document.getElementById("preset-search-results")?.dispatchEvent(
                    new CustomEvent("scrollY", {
                        detail: {
                            diffY: 0,
                            pageY: event['originalEvent'].pageY,
                            pointerId: event['originalEvent'].pointerId
                        }
                    }))
            }
            event.preventDefault();
        }
    },
    "pointerup"(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
        event['currentTarget']?.releasePointerCapture(event['originalEvent'].pointerId);
        event.preventDefault();

        if (innerState?.dragMode == DragMode.None) {
            var ms = multipleSelection.get();

            if (multipleSelectionStartedByShiftClick && !event.shiftKey) {
                multipleSelection.set(null);
            }

            if (event.shiftKey && !ms) {
                multipleSelectionStartedByShiftClick = true;
                multipleSelection.set([]);
            }

            ms = multipleSelection.get();

            if (ms) {
                multipleSelection.set(ms.includes(presenter.id) ? ms.filter(v => v != presenter.id) : ms.concat([presenter.id]));
            }
            else {
                if (presenter.selected)
                    pg.loadPreset(presenter.id);
                else
                    pg.selectPreset(presenter.id);
            }
        }
        else if (innerState?.dragMode == DragMode.DragPresets) {
            const target = getElementAt(event.pageX!, event.pageY!);
            dispatchDragEvent(target, event['originalEvent'], innerState.dataTransfer, "drop");
            dispatchDragEvent(target, event['originalEvent'], innerState.dataTransfer, "dragend");
            event.stopPropagation();
        }

        innerState!.dragMode = DragMode.None;
    },
    "contextmenu"(event: JQuery.Event) {
        event['currentTarget']?.releasePointerCapture(event['originalEvent'].pointerId);
        event.preventDefault();
    },
    'dragstart'(event: JQuery.Event, presenter: Presenter) {
        var t = (event as JQuery.DragStartEvent).originalEvent!.dataTransfer!;
        t.clearData();
        var s = multipleSelection.get();
        if (s == null) {
            t.setData(encodePresetDragDropData(presenter.id), '');
            t.dropEffect = "copy";
            t.effectAllowed = "copyLink";
        }
        else {
            var cp = [...s];
            if (!cp.includes(presenter.id)) {
                cp.push(presenter.id);
                multipleSelection.set(cp);
            }
            t.setData(encodePresetDragDropData(getFilteredMultipleSelection().join(",")), '');
        }
    },
    'dragenter'(event: JQuery.Event) {
        event.preventDefault();
        $("#preset-search").addClass("dragtarget");
    },
    'dragleave'(event: JQuery.Event) {
        event.preventDefault();
        $("#preset-search").removeClass("dragtarget");
    },
    'dragend'(event: JQuery.Event): void {
        event.preventDefault();
        $("#preset-search").removeClass("dragtarget");
    }
};

glue("searchResultPreset", (id) => new Presenter(Number.parseInt(id)), () => new InnerState(false), attributes, events);

$['contextMenu']({
    selector: '.preset',
    build: () => {
        return {
            items: {
                "multiple": {
                    name: multipleSelection.get() ? "Disable Multiple Selection" : "Start Multiple Selection",
                    callback: function () {
                        multipleSelectionStartedByShiftClick = false;
                        multipleSelection.get() ? multipleSelection.set(null) : multipleSelection.set([this[0].id])
                    }
                },
                "bank": {
                    name: "Show in Bank",
                    callback: function () {
                        window['scrollToNonMapsPreset'](this[0].id);
                    }
                },
                "info": {
                    name: "Preset Info",
                    visible: function (key, opt) {
                        return !window['isPresetInfoVisible']();
                    },
                    callback: function () {
                        pg.selectPreset(this[0].id);
                        window['showPresetInfo'](this[0].id);
                    }
                },
                "select-all": {
                    name: "Select All",
                    callback: function () {
                        multipleSelectionStartedByShiftClick = true;
                        multipleSelection.set([...cachedSearch.get()]);
                    }
                }
            }
        }
    }
});

function encodePresetDragDropData(csv: string): string {
    return JSON.stringify({ format: 'preset/csv', data: csv });
}

