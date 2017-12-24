import os
import time

WATCH_INTERVAL = 5
DATA_FILE = "cmake-build-release/out.txt"
MODEL_NAME = "bob"
WR_FILE = "_wr.txt"
HDR = ["v", "h", "d"]


def _read_data(fname):
    vertical = 0
    horizontal = 0
    draw = 0
    with open(fname) as fp:
        for line in fp:
            vertical += line.startswith("Vertical")
            horizontal += line.startswith("Horizontal")
            draw += line.startswith("The")
    return (vertical, horizontal, draw)


def _update_wins(model_name, vert, hori, draw):
    fout = model_name + WR_FILE
    if os.path.exists(fout):
        # must obey HDR's order, v,h,d
        with open(fout, "a") as fp:
            # must obey HDR's order, v,h,d
            fp.write(",".join([str(vert), str(hori), str(draw)]) + "\n")
    else:
        with open(fout, "w") as fp:
            fp.write(",".join(HDR) + '\n')
            fp.write(",".join([str(vert), str(hori), str(draw)]) + "\n")


def main(model_name, fname):
    last_mod = 0
    while True:
        if last_mod < os.path.getmtime(fname):
            last_mod = os.path.getmtime(fname)
            _update_wins(model_name, *_read_data(fname))
        time.sleep(WATCH_INTERVAL)


if __name__ == '__main__':
    main(MODEL_NAME, DATA_FILE)
