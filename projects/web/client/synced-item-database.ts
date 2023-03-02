import { ReactiveVar } from 'meteor/reactive-var';

declare var syncPort: string;

interface SyncedItem {
    topic: string,
    state: any
}

enum SyncedDataState {
    Pending,
    Unsubscribed
}

type SyncedData = {};
type Topic = string;

export class SyncedItemDatabase {
    constructor() {
        console.log("synced item db ctor");
        this.connect();
    }

    public queryItem(topic: Topic): SyncedData | null {
        var v = this.queryVar(topic).get();

        if(v == SyncedDataState.Pending || v == SyncedDataState.Unsubscribed)
            return null;

        return v;
    }

    private queryVar(topic: Topic): ReactiveVar<SyncedData | SyncedDataState> {
        if (!this.database.has(topic)) {
            this.database.set(topic, new ReactiveVar<SyncedData | SyncedDataState>(SyncedDataState.Pending));

            if (this.socket.readyState != WebSocket.OPEN)
                this.pendingSubs.add(topic);
            else
                this.socket.send(JSON.stringify({ 'sub': topic }));
        }

        if(this.database.get(topic)!.get() == SyncedDataState.Unsubscribed) {
            this.socket.send(JSON.stringify({ 'sub': topic }));
            this.database.set(topic, new ReactiveVar<SyncedData | SyncedDataState>(SyncedDataState.Pending));
        }

        return this.database.get(topic)!;
    }

    public update(topic: Topic, item: string, value: string) {
        var v = this.queryVar(topic);
        var c = v.get()!;
        c[item] = value;
        v.set(c);
    }

    private connect() {
        console.log("synced item db connect");
        const hostName = location.hostname.length == 0 ? "localhost" : location.hostname;
        const port = !syncPort || syncPort.length < 2 ? ":8893" : syncPort;
        this.socket = new WebSocket("ws://" + hostName + port);
        this.socket.onopen = (event) => this.onOpen();
        this.socket.onerror = (event) => this.onError();
        this.socket.onclose = (event) => this.onClose();
        this.socket.onmessage = (event) => this.onMessage(event.data);
    }

    private onOpen() {
        this.pendingSubs.forEach((t) => this.socket.send(JSON.stringify({ 'sub': t })));
        this.pendingSubs.clear();
        this.database.forEach((v, k) => v.set(SyncedDataState.Pending));
        this.database.clear();
    }

    private onError() {
        console.log("socket error");
        this.reconnect();
    }

    private onClose() {
        console.log("socket close");
        this.reconnect();
    }

    private reconnect() {
        console.log("socket reconnect");
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
            console.log("onMessage: " + reader.result as string);

            if (c) {
                if (!c.state || Object.keys(c.state).length == 0) {
                    this.queryVar(c.topic).set(SyncedDataState.Unsubscribed);
                }
                else {
                    var k = this.queryVar(c.topic);
                    if (JSON.stringify(k.get()) != JSON.stringify(c.state))
                        this.queryVar(c.topic).set(c.state);
                }
            }

        }
        reader.readAsText(data);
    }

    private socket!: WebSocket;
    private database = new Map<Topic, ReactiveVar<SyncedData | SyncedDataState>>();
    private reconnectTimeout = -1;
    private pendingSubs = new Set<Topic>();
}
