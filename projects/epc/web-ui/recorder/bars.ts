const barLength = (4096 / 48000) * 1000 * 1000 * 1000;

class Bar {
    id: number = 0;
    max: number = 0;
    recordTime: number = 0;
}

class Bars {
    removeUntil(id: number): boolean {
        var ret = false;

        while (this.count() > 0) {
            if (this.first().id >= id)
                break;

            this.remove();
            ret = true;
        }
        return ret;
    }

    add(bar: Bar) {
        if (this.bars.length > 0) {
            var last = this.bars[this.bars.length - 1];
            var idDiff = bar.id - last.id;
            var timeDiff = bar.recordTime - last.recordTime;
            var diffFromExpected = Math.abs(timeDiff - idDiff * barLength);
            var ms = idDiff * 1000 * 1000; // accept 1 ms per frame

            if (last.max == bar.max && diffFromExpected < ms) {
                this.lastId = bar.id;
                return;
            }
        }

        this.lastId = bar.id;
        this.bars.push(bar);
    }

    remove() {
        var c = this.count();

        if (c == 0)
            return;

        var nextId = (c > 1) ? this.get(1).id : this.lastId;
        var firstBar = this.bars[0];

        if (nextId == (firstBar.id + 1)) {
            this.bars.splice(0);
        }
        else {
            firstBar.id++;
            firstBar.recordTime += barLength;
        }
    }

    clear() {
        this.bars = new Array<Bar>();
        this.lastId = -1;
    }

    count(): number {
        if (this.bars.length != 0)
            return 1 + (this.lastId - this.first().id);

        return 0;
    }

    empty(): boolean {
        return this.lastId == -1;
    }

    get(id: number): Bar {
        var b = this.searchLessOrEqual(id, 0, this.bars.length - 1);
        var dist = id - b.id;
        var ret = new Bar();
        ret.id = id;
        ret.max = b.max;
        ret.recordTime = b.recordTime + dist * barLength;
        return ret;
    }

    first(): Bar {
        return this.bars[0];
    }

    last(): Bar {
        return this.get(this.count() - 1);
    }

    private searchLessOrEqual(idx: number, start: number, end: number): Bar {
        var diff = end - start;

        if (diff <= 3) {
            for (var i = end; i >= start; i--) {
                if (this.bars[i].id <= idx)
                    return this.bars[i];
            }
            return this.bars[start];
        }
        else {
            var mid = Math.round(start + diff / 2);
            var b = this.bars[mid];
            if (b.id == idx)
                return b;
            else if (b.id > idx)
                return this.searchLessOrEqual(idx, start, mid);
            else
                return this.searchLessOrEqual(idx, mid, end);
        }
    }

    private bars = Array<Bar>();
    private lastId = 0;
}