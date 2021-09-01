import { SyncedItemDatabase } from "./synced-item-database";

declare var playgroundPort: string;
declare var sync: SyncedItemDatabase

export class PlaygroundProxy {
    constructor() {
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
        const preset = sync.queryItem("/preset/" + uuid);
        var bankVar = sync.queryVar("/bank/" + preset["bank-uuid"]);

        var val = bankVar.get();
        val['selected-preset'] = uuid;
        bankVar.set(val);

        // make it
        this.send("/presets/banks/select-preset?uuid=" + uuid);
    }

    loadPreset(uuid: string) {
        this.send("/presets/banks/load-preset?uuid=" + uuid);
    }

    selectBank(uuid: any) {
        this.send("/presets/select-bank?uuid=" + uuid);
    }

    private connect() {
        const hostName = location.hostname.length == 0 ? "localhost" : location.hostname;
        this.socket = new WebSocket("ws://" + hostName + playgroundPort + "/ws/" + this.clientId);
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