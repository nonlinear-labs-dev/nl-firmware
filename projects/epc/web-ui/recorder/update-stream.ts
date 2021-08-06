
class TimingInfo {
    serverTime: number = 0;
    localTime: number = 0;
}

function assert(b: boolean) {
    if (!b)
        throw new Error('assertion failed');
}

class MockUpdateStream {
    constructor(private c15: C15ProxyIface, addBars: boolean) {
        console.log("connect");
        this.bars.clear();

        this.timingInfo.serverTime = this.time;
        this.timingInfo.localTime = this.time;

        if (addBars)
            this.addBars(5000, true);

        this.timer = setInterval(() => this.addBars(1, false), 85);

        c15.updateMemoryUsage(1234567, 500 * 1024 * 1024);
        c15.updateTransportStates(false, true, this.bars.firstId + this.bars.count() / 2);
    }

    stop() {
        clearInterval(this.timer);
    }

    addBars(n: number, force: boolean) {
        if (this.c15.getRecordingState() != TransportState.Paused || force) {
            for (var i = 0; i < n; i++) {
                var b = new Bar();
                b.id = this.id++;
                b.max = Math.abs(Math.sin(this.id / 10) * Math.sin(this.id / 100) * Math.sin(this.id / 1000) * Math.sin(this.id / 10000) * 256);

                if (b.max < 5)
                    b.max = 0;

                b.recordTime = this.time;
                this.time += singleBarLength;
                this.bars.add(b);
            }
            this.c15.synced();
        }
    }

    bars: Bars = new Bars(singleBarLength);
    timingInfo: TimingInfo = new TimingInfo();

    private id = 3;
    private timer = 0;
    private time = new Date().getTime() * 1000 * 1000;
}

class Expiration {
    constructor(handler: (() => void) | null, ms: number) {
        this.renew(handler, ms);
    }

    renew(handler: (() => void) | null, ms: number) {
        this.cancel();
        if (handler)
            this.timer = setTimeout(handler, ms);
    }

    cancel() {
        if (this.timer != -1)
            clearTimeout(this.timer);
    }

    private timer = -1;
}

class WebSocketAPI {
    constructor(private onError: (showBanner: boolean) => void, onOpen: () => void) {
        this.socket = new WebSocket("ws://" + hostName + wsPort);
        this.socket.onopen = (event) => onOpen();
        this.socket.onerror = (event) => onError(true);
        this.socket.onclose = (event) => onError(true);
        this.socket.onmessage = (event) => this.readMessage(event.data);
    }

    doRPC(rpc: any, handler: (e: JSON) => void): void {
        if (this.handler) {
            console.log("rpc while in flight!" + handler + this.handler);
            return;
        }

        try {
            this.socket!.send(JSON.stringify(rpc));
            this.handler = handler;
        }
        catch (err) {
            this.onError(true);
        }
    }

    close() {
        this.socket.onopen = null;
        this.socket.onerror = null;
        this.socket.onclose = null;
        this.socket.onmessage = null;
        this.socket.close();
        this.handler = null;
    }

    private readMessage(data: any): void {
        var reader = new FileReader()
        reader.onload = () => {
            var h = this.handler;
            this.handler = null;

            if (h)
                h(JSON.parse(reader.result as string));
        }

        reader.onerror = () => this.onError(false);
        reader.readAsText(data);
    }

    private socket: WebSocket;
    private handler: ((e: JSON) => void) | null = null;
}

class UpdateStream {
    constructor(private c15: C15ProxyIface) {
        this.c15.setConnectionState(ConnectionState.Disconnected);
        this.connect();
    }

    private connect() {
        console.log("connect");
        this.bars.clear();
        this.socket = new WebSocketAPI((showBanner) => this.retry(showBanner), () => this.scheduleUpdate(true));
    }

    stop() {
        console.log("stop");
        this.updateTimer.cancel();
        this.socket!.close();
        this.socket = null;
    }

    private scheduleUpdate(fast: boolean) {
        this.updateTimer.renew(() => this.update(), fast ? 1 : 200);
    }

    private update(): void {
        this.c15.setConnectionState(ConnectionState.Connected);
        this.socket!.doRPC({ 'get-info': {} }, (e) => this.processInfo(e));
    }

    private processInfo(info: any) {
        this.timingInfo.serverTime = info.time;
        this.timingInfo.localTime = Date.now() * 1000 * 1000;

        if (!info.storage) {
            this.c15.synced();
            this.scheduleUpdate(false);
            return;
        }

        this.c15.updateMemoryUsage(info.storage.memUsage.current, info.storage.memUsage.max);
        this.c15.updateTransportStates(info.recorder.paused, info.player.paused, info.player.pos);

        // remove outdated bars
        this.updateWaveform = this.bars.removeUntil(info.storage.first.id) || this.updateWaveform;

        // query new bars
        var nextId = info.storage.first.id;

        if (this.bars.count() > 0)
            nextId = this.bars.last()!.id + 1;

        if (info.storage.last.id - nextId < 1) {
            this.c15.synced();
            this.scheduleUpdate(false);
            return;
        }

        this.socket!.doRPC({ 'query-frames': { 'begin': nextId, 'end': info.storage.last.id } }, (e) => this.processQuery(e, info.storage.last.id));
    }

    private processQuery(info: any, expectedLastId: number) {
        info.forEach((a: Bar) => {
            this.bars.add(a);
            this.updateWaveform = true;
        });

        if (this.updateWaveform) {
            this.updateWaveform = false;
            this.c15.synced();
        }

        var fast = expectedLastId != this.bars.last().id;
        this.scheduleUpdate(fast);
    }

    private retry(showDisconnected: boolean): void {
        console.log("retry");

        if (showDisconnected)
            this.c15.setConnectionState(ConnectionState.Disconnected);

        this.stop();
        this.retryTimer.renew(() => this.connect(), 1000);
    }

    bars: Bars = new Bars(singleBarLength);
    timingInfo: TimingInfo = new TimingInfo();

    private updateWaveform = false;
    private socket: WebSocketAPI | null = null;

    private updateTimer = new Expiration(null, 0);
    private retryTimer = new Expiration(null, 0);
}