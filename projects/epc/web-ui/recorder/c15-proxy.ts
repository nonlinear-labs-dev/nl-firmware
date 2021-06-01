enum TransportState {
    Playing,
    Recording,
    Paused
}

enum ConnectionState {
    Connected,
    Disconnected
}

abstract class C15ProxyIface {
    abstract download(from: number, to: number): void;
    abstract getBars(): Bars;
    abstract getPresetLoadEvent(fromTime: number, toTime: number): PresetLogEntry | null;
    abstract buildTime(lastTime: number | undefined): string;
    abstract setPlaybackPosition(playPos: number): void;
    abstract reset(): void;
    abstract toggleRecording(): void;
    abstract pausePlayback(): void;
    abstract startPlayback(): void;
    abstract undo(): void;

    connect(ui: UI) {
        this.ui = ui;
    }

    synced() {
        if (this.ui)
            this.ui.update();
    }

    togglePlayback(): void {
        if (this.getPlaybackState() == TransportState.Playing)
            this.pausePlayback();
        else
            this.startPlayback();
    }

    updateTransportStates(recPaused: boolean, playPaused: boolean, playPos: number): void {
        this.recordPaused = recPaused;
        this.playbackPaused = playPaused;
        this.currentPlayPosition = playPos;
    }

    updateMemoryUsage(currentUsage: number, maxUsage: number): void {
        this.currentMemoryUsage = currentUsage;
        this.maxMemoryUsage = maxUsage;
    }

    getCurrentMemoryUsage(): number {
        return this.currentMemoryUsage;
    }

    getMaxMemoryUsage(): number {
        return this.maxMemoryUsage;
    }

    getPlaybackState(): TransportState {
        return this.playbackPaused ? TransportState.Paused : TransportState.Playing;
    }

    getRecordingState(): TransportState {
        return this.recordPaused ? TransportState.Paused : TransportState.Recording;
    }

    getCurrentPlayPosition(): number {
        return this.currentPlayPosition;
    }

    static create(): C15ProxyIface {
        var mock = location.search.includes("mock-audio-data");
        return mock ? new C15ProxyMock() : new C15Proxy();
    }

    setConnectionState(s : ConnectionState) : void {
        this.connectionState = s;
        this.synced();
    }

    getConnectionState() : ConnectionState {
        return this.connectionState;
    }

    protected ui: UI | null = null;
    protected currentMemoryUsage = 0;
    protected maxMemoryUsage = 0;
    protected recordPaused = true;
    protected playbackPaused = true;
    protected currentPlayPosition = 0;
    protected connectionState = ConnectionState.Connected;
}

class C15ProxyMock extends C15ProxyIface {
    constructor() {
        super();

        this.currentMemoryUsage = 123456;
        this.maxMemoryUsage = 1234567;

        for (var i = 0; i < 10; i++) {
            this.logEntries.push(new PresetLogEntry(this.getBars().last().recordTime - i * 10 * 1000 * 1000 * 1000, "01: Foo / 02: Bar"));
        }

        setInterval(() => this.playProgress(), 85);
    }

    getBars(): Bars {
        return this.updateBarsStream.bars;
    }

    getPresetLoadEvent(fromTime: number, toTime: number): PresetLogEntry | null {
        const r = this.logEntries.filter(s => s.time >= fromTime && s.time <= toTime);
        if (r.length > 0)
            return r[0];
        return null;
    }

    buildTime(serverTime: number | undefined): string {
        if (!serverTime)
            return "";

        var t = serverTime - this.updateBarsStream.timingInfo.serverTime + this.updateBarsStream.timingInfo.localTime;
        var d = new Date(t / 1000 / 1000);
        return d.getHours().toString().padStart(2, '0') + ":" + d.getMinutes().toString().padStart(2, '0') + ":" + d.getSeconds().toString().padStart(2, '0');
    }

    setPlaybackPosition(playPos: number): void {
        this.currentPlayPosition = playPos;
        this.synced();
    }

    reset(): void {
        this.updateBarsStream.stop();
        this.updateBarsStream = new MockUpdateStream(this, false);
    }

    toggleRecording(): void {
        this.recordPaused = !this.recordPaused;
        this.synced();
    }

    pausePlayback(): void {
        this.playbackPaused = true;
        this.synced();
    }

    startPlayback(): void {
        this.playbackPaused = false;
        this.synced();
    }

    playProgress() {
        if (!this.playbackPaused) {
            this.currentPlayPosition++;
            this.synced();
        }
    }

    undo(): void {
    }

    download(from: number, to: number): void {
    }

    private updateBarsStream = new MockUpdateStream(this, true);
    private logEntries = new Array<PresetLogEntry>();
}

class C15Proxy extends C15ProxyIface {
    buildTime(serverTime: number | undefined): string {
        if (!serverTime)
            return "";

        var t = serverTime - this.updateStream.timingInfo.serverTime + this.updateStream.timingInfo.localTime;
        var d = new Date(t / 1000 / 1000);
        return d.getHours().toString().padStart(2, '0') + ":" + d.getMinutes().toString().padStart(2, '0') + ":" + d.getSeconds().toString().padStart(2, '0');
    }

    getBars(): Bars {
        return this.updateStream.bars;
    }

    getPresetLoadEvent(fromTime: number, toTime: number): PresetLogEntry | null {
        return this.presetLogStream.find(fromTime, toTime);
    }

    setPlaybackPosition(playPos: number): void {
        playPos = Math.min(Math.max(this.getBars().first().id, playPos), this.getBars().last().id)
        this.fireAndForget({ "set-playback-position": { "frameId": playPos } }, null);
    }

    pausePlayback(): void {
        this.fireAndForget({ "pause-playback": {} }, null);
    }

    startPlayback(): void {
        this.fireAndForget({ "start-playback": {} }, null);
    }

    reset() {
        this.ui!.promptOption("This will delete the whole audio content that has been recorded. Are you sure?", () => {
            this.doReset();
        });
    }

    doReset() {
        this.updateStream.stop();
        this.fireAndForget({ "reset": {} }, () => {
            this.updateStream = new UpdateStream(this);
            this.currentPlayPosition = 0;
        });
    }

    toggleRecording() {
        this.fireAndForget({ "toggle-recording": {} }, null);
    }

    undo() {
        var bar = this.getBars().get(this.getCurrentPlayPosition());
        if (bar) {
            var r = new XMLHttpRequest();
            var url = "http://" + hostName + playgroundHttpPort + "/presets/param-editor/restore?timestamp=" + bar.recordTime;
            r.open("GET", url, true);
            r.send();
        }
    }

    download(from: number, to: number) {
        var url = "http://" + hostName + httpPort + "/?begin=" + from + "&end=" + to;
        window.location.assign(url);
    }

    private fireAndForget(msg: Object, cb: (() => void) | null) {
        var webSocket = new WebSocket("ws://" + hostName + wsPort);

        if (cb)
            webSocket.onmessage = () => { cb(); };

        webSocket.onopen = () => webSocket.send(JSON.stringify(msg));
    }

    private updateStream = new UpdateStream(this);
    private presetLogStream = new PresetLogStream(this);

}