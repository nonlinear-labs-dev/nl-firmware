import { ReactiveVar } from 'meteor/reactive-var';

declare var syncPort: string;

interface SyncedItem {
    topic: string,
    state: any
}

enum DatabaseEvent {
    Add,
    Update,
    Remove
}

type SyncedData = {};


type Topic = string;

export class SyncedItemDatabase {
    constructor() {
        this.connect();
    }

    public queryItem(topic: Topic): SyncedData {
        return this.queryVar(topic).get();
    }

    public queryVar(topic: Topic): ReactiveVar<SyncedData> {
        if (!this.database.has(topic)) {
            this.database.set(topic, new ReactiveVar<SyncedData>({}));
            this.socket.send(JSON.stringify({ 'sub': topic }));
        }
        return this.database.get(topic);
    }

    private connect() {
        const hostName = location.hostname.length == 0 ? "localhost" : location.hostname;
        this.socket = new WebSocket("ws://" + hostName + syncPort);
        this.socket.onopen = (event) => this.onOpen();
        this.socket.onerror = (event) => this.onError();
        this.socket.onclose = (event) => this.onClose();
        this.socket.onmessage = (event) => this.onMessage(event.data);
    }

    private onOpen() {
        this.database.forEach((v, k) => v.set(null));
        this.database.clear();
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

    private onMessage(data: any) {
        var reader = new FileReader()
        reader.onload = () => {
            var c: SyncedItem = JSON.parse(reader.result as string);

            if (c) {
                if (!c.state || Object.keys(c.state).length == 0) {
                    this.queryVar(c.topic).set(null);
                }
                else {
                    this.queryVar(c.topic).set(c.state);
                }
            }

        }
        reader.readAsText(data);
    }

    private socket!: WebSocket;
    private database = new Map<Topic, ReactiveVar<SyncedData>>();
    private reconnectTimeout = -1;
}
