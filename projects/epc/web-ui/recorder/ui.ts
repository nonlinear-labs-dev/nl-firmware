class UI {
    constructor(private c15: C15ProxyIface) {
        this.waveform = new Waveform(c15);
        document.getElementById("toggle-recording")!.onclick = (e) => c15.toggleRecording();

        var pb = document.getElementById("toggle-playback")!;
        pb.onclick = (e) => {
            if (pb.classList.contains("disabled"))
                return;

            c15.togglePlayback();
        }

        var undo = document.getElementById("undo")!;
        undo.onclick = (e) => {
            if (undo.classList.contains("disabled"))
                return;

            c15.undo();
        }

        var download = document.getElementById("download")!;
        download.onclick = (e) => {
            if (download.classList.contains("disabled"))
                return;

            e.stopPropagation();
            e.preventDefault();
            c15.download(this.waveform.selectedRange.playbackRange.min(), this.waveform.selectedRange.playbackRange.max());
        }

        document.getElementById("reset")!.onclick = (e) => {
            c15.reset();
            this.waveform.selectedRange.playbackRange.reset();

        }

        document.getElementById("waveform")!.onkeydown = (e) => c15.togglePlayback();

    }

    update() {
        if(this.c15.getConnectionState() == ConnectionState.Connected)
            document.getElementById("not-connected-box")!.classList.add("hidden");
        else
            document.getElementById("not-connected-box")!.classList.remove("hidden");

        document.getElementById("first-available-frame-timestamp")!.textContent = this.c15.buildTime(this.c15.getBars().first().recordTime);
        document.getElementById("last-available-frame-timestamp")!.textContent = this.c15.buildTime(this.c15.getBars().last().recordTime);
        document.getElementById("consumed-mem")!.textContent = this.formatMemSize(this.c15.getCurrentMemoryUsage());
        document.getElementById("max-mem")!.textContent = this.formatMemSize(this.c15.getMaxMemoryUsage());
        var rec = document.getElementById("toggle-recording")!;
        var pb = document.getElementById("toggle-playback")!;
        var undo = document.getElementById("undo")!;

        if (this.c15.getCurrentPlayPosition() > 0) {
            pb.classList.remove("disabled");
            undo.classList.remove("disabled");
        }
        else {
            pb.classList.add("disabled");
            undo.classList.add("disabled");
        }

        if (this.c15.getRecordingState() == TransportState.Paused) {
            rec.classList.remove("recording");
            rec.classList.add("paused");
        }
        else {
            rec.classList.add("recording");
            rec.classList.remove("paused");
        }

        if (this.c15.getPlaybackState() == TransportState.Paused) {
            pb.classList.remove("playing");
            pb.classList.add("paused");
        }
        else {
            pb.classList.add("playing");
            pb.classList.remove("paused");
        }

        this.waveform.update();
    }

    private formatMemSize(s: number) {
        var i = 0;
        var units = [' B', ' kB', ' MB'];

        while (s > 1024 && i < 2) {
            s = s / 1024;
            i++;
        }

        return Math.max(s, 0.1).toFixed(1) + units[i];
    }

    private waveform: Waveform;
}
