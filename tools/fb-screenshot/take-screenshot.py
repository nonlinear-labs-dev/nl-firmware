import os

os.system("ssh root@192.168.8.2 'cat /dev/fb0 > /tmp/fb.raw'")
os.system("scp root@192.168.8.2:/tmp/fb.raw ./fb.raw")


dict = {'\x00':(43, 32, 21), '\x02':(77, 60, 10), '\x05':(103, 81, 12), '\x06':(128, 102, 16), '\x0A':(179, 142, 21), '\x0B':(204, 162, 24), '\x0F':(255, 203, 32) }

with open ("./fb-converted.data", "wb") as out:
    with open("./fb.raw", "rb") as f:
        while True:
            chunk = f.read(8192)
            if chunk:
                for b in chunk:
                    u = dict[b]
                    v = ((u[0] >> 3) << 11) | ((u[1] >> 2) << 5) | ((u[2] >> 3) << 0);
                    out.write(chr((v >> 0) & 0xFF))
                    out.write(chr((v >> 8) & 0xFF))
            else:
                break;
