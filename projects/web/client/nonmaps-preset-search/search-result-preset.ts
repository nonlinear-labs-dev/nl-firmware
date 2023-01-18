import { globals, glue } from "../glue";
import "./search.html";
import { cachedSearch, currentContextMenuItem, getPresetHeight, multipleSelection, multipleSelectionStartedByShiftClick, numPresetsBeforeFirstVisible, scrollY, setPresetHeight } from "./shared-state";

const presetCSVFormat = "text/preset-csv";

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

        idx = Math.floor(idx + offset) - numPresetsBeforeFirstVisible;
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

            var isShowingCtxMenu = currentContextMenuItem.get().includes(id);

            this.id = id;
            this.number = number;
            this.name = preset['name-suffixed'];
            this.selected = presetSelected || isShowingCtxMenu;
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
    "selectOrLoad"(event: JQuery.Event, presenter: Presenter) {
        const o = event['originalEvent'].detail.originalEvent;
        var ms = multipleSelection.get();

        if (multipleSelectionStartedByShiftClick.get() && !o.shiftKey) {
            multipleSelection.set(null);
        }

        if (o.shiftKey && !ms) {
            multipleSelectionStartedByShiftClick.set(true);
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
    },
    "startDrag"(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
        const o = event['originalEvent'].detail.originalEvent;
        dispatchDragEvent(event['currentTarget'], o, innerState!.dataTransfer!, 'dragstart');
    },
    "stopDrag"(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
        const o = event['originalEvent'].detail.originalEvent;
        dispatchDragEvent(this._lastTarget, o, innerState!.dataTransfer!, "drop");
        dispatchDragEvent(this._lastTarget, o, innerState!.dataTransfer!, "dragend");
    },
    "cancelDrag"(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
        const o = event['originalEvent'].detail.originalEvent;
        var e = new DragEvent("dragend", {});
        this._lastTarget.dispatchEvent(e);
    },
    "performDrag"(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
        console.log("perform drag");

        const o = event['originalEvent'].detail.originalEvent;
        const target = getElementAt(o.pageX, o.pageY);
        dispatchDragEvent(target, o, innerState!.dataTransfer!, "drag");

        if (target != this._lastTarget) {
            dispatchDragEvent(this._lastTarget, o, innerState!.dataTransfer!, "dragleave");
            this._lastTarget = target;
            dispatchDragEvent(target, o, innerState!.dataTransfer!, "dragenter");
        }
        dispatchDragEvent(target, o, innerState!.dataTransfer!, "dragover");
    },
    'dragstart'(event: JQuery.Event, presenter: Presenter, innerState: InnerState | null) {
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

function encodePresetDragDropData(csv: string): string {
    return JSON.stringify({ format: 'preset/csv', data: csv });
}

Meteor.startup(() => {
    const observer = new MutationObserver((mutationList, observer) => {
        if (setPresetHeight())
            observer.disconnect();
    });

    observer.observe(document.body, { attributes: true, childList: true, subtree: true });
});
