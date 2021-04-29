
class Waveform extends Draggable {
    constructor(private c15: C15ProxyIface) {
        super("waveform");
        this.selectedRange = new SelectedRange(c15, this);
        this.scrollbar = new Scrollbar(c15, this);

        var waveForm = document.getElementById("waveform")!;
        waveForm.onwheel = (e) => this.wheel(e);

        document.getElementById("zoom-in")!.onclick = (e) => this.zoomIn();
        document.getElementById("zoom-out")!.onclick = (e) => this.zoomOut();
    }

    update() {
        var c = document.getElementById("bars") as HTMLCanvasElement;
        var ctx = c.getContext("2d") as CanvasRenderingContext2D;

        c.width = c.clientWidth;
        c.height = c.clientHeight;

        var center = c.height / 2;

        ctx.clearRect(0, 0, c.width, c.height);
        ctx.strokeStyle = window.getComputedStyle(document.querySelector('#bars')!).color!;
        ctx.lineWidth = 1;

        // middle line
        ctx.beginPath();
        ctx.moveTo(0, center);
        ctx.lineTo(c.clientWidth, center);
        ctx.stroke();

        if (this.c15.getBars().count() == 0)
            return;

        this.sanitize(c.width);

        var lastId = this.lastBarIdToShow != -1 ? this.lastBarIdToShow : this.c15.getBars().last().id;
        var firstBarId = lastId - c.width * this.zoom;
        var id = firstBarId;

        for (var i = 0; i < c.width; i++) {
            var m = this.c15.getBars().getMax(id, this.zoom);

            var from = Math.round(id);
            var to = Math.round(id + this.zoom);
            id += this.zoom;

            var m = center * m / 256;
            var top = center - m;
            var bottom = center + m;
            ctx.moveTo(i, top);
            ctx.lineTo(i, bottom);

            if (id >= lastId)
                break;
        }

        ctx.stroke();
        this.drawGrid(ctx, c.clientWidth, c.clientHeight, firstBarId);
        this.selectedRange.update(firstBarId);
        this.scrollbar.update(firstBarId);

        if (this.c15.getBars().count() > 0) {
            var playPosIndicator = document.getElementById("play-pos") as HTMLElement;
            var lastId = this.lastBarIdToShow != -1 ? this.lastBarIdToShow : this.c15.getBars().last().id;
            var firstBarId = lastId - c.width * this.zoom;
            var playPos = this.isDragging() ? this.dragPosition : this.c15.getCurrentPlayPosition();
            playPos = Math.min(Math.max(playPos, this.c15.getBars().first().id), lastId);
            playPosIndicator.style.left = (playPos - firstBarId) / this.zoom + "px";
            playPosIndicator.style.visibility = playPos > 0 ? "visible" : "hidden";

            var playPosTime = document.getElementById("play-pos-time") as HTMLElement;
            var playBar = this.c15.getBars().get(playPos);
            if (playBar)
                playPosTime.innerText = this.c15.buildTime(playBar.recordTime);
        }
    }

    private drawGrid(ctx: CanvasRenderingContext2D, width: number, height: number, id: number) {
        if (this.c15.getBars().count() < 2)
            return;

        var pixelsPerTimeMarker = 100;
        var timeMarkersOnScreen = Math.floor(width / pixelsPerTimeMarker);
        var barsOnScreen = width * this.zoom;
        var barsPerTimeMarker = Math.floor(barsOnScreen / timeMarkersOnScreen);

        var samplesPerBar = 4096; // constant defined in audio-engine
        var samplerate = 48000;

        var samplesPerTimeMarker = barsPerTimeMarker * samplesPerBar;
        var msPerTimeMarker = 1000 * samplesPerTimeMarker / samplerate;
        var nsPerTimeMarker = 1000 * 1000 * msPerTimeMarker;

        var fontHeight = 10;
        ctx.font = fontHeight + "px serif";

        var serverTime = this.c15.getBars().first().recordTime;

        for (var i = 0; i < width; i++) {
            var from = Math.round(id);
            var to = Math.round(id + this.zoom);

            var markerOffset = (1 + Math.floor(Math.abs(from) / barsPerTimeMarker)) * barsPerTimeMarker;

            var isWrapInRange = ((from + markerOffset) % barsPerTimeMarker) > ((to + markerOffset) % barsPerTimeMarker);
            id += this.zoom;

            if (isWrapInRange) {
                var b = this.c15.getBars().get(from) || this.c15.getBars().get(to);
                serverTime = b ? b.recordTime : serverTime + nsPerTimeMarker;
                ctx.moveTo(i, 0);
                ctx.lineTo(i, height - fontHeight);
                var str = this.c15.buildTime(serverTime);
                var xOffset = ctx.measureText(str).width / 2;
                ctx.strokeText(str, i - xOffset, height - 2);
            }
        }
        ctx.stroke();
    }

    getFirstVisibleBar(): number {
        if (this.c15.getBars().count() == 0)
            return -1;

        var c = document.getElementById("bars") as HTMLCanvasElement;
        return this.getLastVisibleBar() - c.clientWidth * this.zoom;
    }

    getLastVisibleBar(): number {
        if (this.c15.getBars().count() == 0)
            return -1;
        return this.lastBarIdToShow != -1 ? this.lastBarIdToShow : this.c15.getBars().last().id;
    }

    showBars(first: number, last: number) {
        var c = document.getElementById("bars") as HTMLCanvasElement;
        var numBars = last - first;
        var newZoom = Math.max(1, numBars / c.width);

        if (newZoom == this.zoom)
            return;

        this.zoom = newZoom;
        this.lastBarIdToShow = last;
        this.update();
    }

    private wheel(e: WheelEvent) {
        var oldZoom = this.zoom;
        var newZoom = oldZoom + (e.deltaY < 0 ? 1 : -1);
        newZoom = Math.max(newZoom, 1);

        if (newZoom == oldZoom)
            return;

        var c = document.getElementById("bars") as HTMLDivElement;
        var lastId = this.lastBarIdToShow != -1 ? this.lastBarIdToShow : this.c15.getBars().last().id;
        var firstBarId = lastId - c.clientWidth * oldZoom;
        var wheelOnBar = firstBarId + e.offsetX * oldZoom;
        var b = wheelOnBar + newZoom * c.clientWidth - e.offsetX * newZoom;

        this.lastBarIdToShow = b;
        this.zoom = newZoom;
        this.update();
    }

    startDrag(e: PointerEvent) {
        this.drag(e);
    }

    drag(e: PointerEvent) {
        var c = document.getElementById("bars") as HTMLDivElement;
        var lastId = this.lastBarIdToShow != -1 ? this.lastBarIdToShow : this.c15.getBars().last().id;
        var firstBarId = lastId - c.clientWidth * this.zoom;
        var bar = firstBarId + e.offsetX * this.zoom;
        this.dragPosition = bar;
        this.update();
    }

    stopDrag(e: PointerEvent) {
        this.c15.setPlaybackPosition(this.dragPosition);
    }

    scrollBy(amount: number) {
        var lastId = this.c15.getBars().last().id;

        if (this.lastBarIdToShow == -1) {
            if (amount > 0)
                this.lastBarIdToShow = Math.round(lastId - amount * this.zoom);
        }
        else {
            this.lastBarIdToShow = Math.round(this.lastBarIdToShow - amount * this.zoom);
        }

        if (this.lastBarIdToShow > lastId - 5 * this.zoom)
            this.lastBarIdToShow = -1; // auto scroll

        this.update();
    }

    private zoomIn() {
        this.zoom /= 2;
        this.update();
    }

    private zoomOut() {
        this.zoom *= 2;
        this.update();
    }

    private sanitize(width: number) {
        this.zoom = Math.max(1, this.zoom);

        var c = document.getElementById("bars") as HTMLCanvasElement;

        if (c.width * this.zoom > this.c15.getBars().count())
            this.lastBarIdToShow = -1; // auto scroll

        if (this.lastBarIdToShow != -1) {
            var barsToShow = Math.floor(this.zoom * width);
            this.lastBarIdToShow = Math.min(this.lastBarIdToShow, this.c15.getBars().last().id);

            if (this.c15.getBars().count() > barsToShow) {
                this.lastBarIdToShow = Math.max(this.lastBarIdToShow, this.c15.getBars().get(barsToShow + this.c15.getBars().firstId)!.id);
            }
        }
    }

    zoom = 1.0;
    lastBarIdToShow = -1; // -1 = autoscroll
    selectedRange: SelectedRange;

    private scrollX = -1;
    private pointers = new Array<PointerEvent>();
    private barsPerPixelOnPinchStart = 0;
    private scrollbar: Scrollbar;
    private dragPosition = 0;
}