import { SyncedItemDatabase } from "./synced-item-database";

declare var playgroundPort: string;


export class PlaygroundProxy {
    constructor(private sync: SyncedItemDatabase) {
        this.connect();
    }

    selectParameter(id: string) {
        this.send("/presets/param-editor/select-param?id=" + id);
    }

    setParameter(id: string, value: number) {
        this.send("/presets/param-editor/set-param?id=" + id + "value=" + value);
    }

    selectPreset(uuid: string) {
        // fake it
        const preset = this.sync.queryItem("/preset/" + uuid);
        const bankUuid = preset!["bank-uuid"];

        this.sync.update("/bank/" + bankUuid, 'selected-preset', uuid);
        this.sync.update("/preset-manager", 'selected-bank', bankUuid);

        // make it
        this.send("/banks/select-preset?uuid=" + uuid);
    }

    loadPreset(uuid: string) {
        this.send("/banks/load-preset?uuid=" + uuid);
    }

    selectBank(uuid: any) {
        this.send("/presets/select-bank?uuid=" + uuid);
    }

    log(message: string) {
        this.send("/logger/log?message=" + message);
    }

    setPresetAttribute(preset: string, key: string, value: any) {
        this.send("/banks/set-preset-attribute?uuid=" + preset + "&key=" + key + "&value=" + value);
    }

    private connect() {
        const hostName = location.hostname.length == 0 ? "localhost" : location.hostname;
        const port = !playgroundPort || playgroundPort.length < 2 ? ":8080" : playgroundPort;

        this.socket = new WebSocket("ws://" + hostName + port + "/ws/" + this.clientId);
        this.socket.onopen = (event) => this.onOpen();
        this.socket.onerror = (event) => this.onError();
        this.socket.onclose = (event) => this.onClose();
    }

    private send(j: string) {
        this.socket.send(j);
    }

    private onOpen() {
    }

    private onError() {
        this.reconnect();
    }

    private onClose() {
        this.reconnect();
    }

    private reconnect() {
        this.disconnect();

        if (this.reconnectTimeout == -1) {
            this.reconnectTimeout = window.setTimeout(() => {
                this.reconnectTimeout = -1;
                this.connect();
            }, 1000);
        }
    }

    private disconnect() {
        this.socket.onopen = null;
        this.socket.onerror = null;
        this.socket.onclose = null;
        this.socket.onmessage = null;
        this.socket.close();
    }

    private socket!: WebSocket;
    private reconnectTimeout = -1;
    private clientId = Math.random().toString(36).substr(2, 9);
}