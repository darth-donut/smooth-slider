import time
import os

WATCH_INTERVAL = 5
CSV_NAME = "_model_change.csv"
MODEL_NAME = "bob"
MODEL_PATH = "res/models/bob.txt"



def _is_weight(string):
    return string[0] == '-' or string[0].isdigit()

def _is_feature(string):
    return string[0].isalpha()

def _read_model(file_name):
    mapping = {}
    with open(file_name) as fp:
        weights = []
        feature = []
        for line in fp:
            line = line.strip()
            if line.startswith("a:") or line.startswith("b:"):
                continue
            else:
                if _is_weight(line):
                    weights.append(float(line))
                elif _is_feature(line):
                    feature.append(line)
        assert len(feature) == len(weights)
        for w, f in zip(weights, feature):
            mapping[f] = w
    return mapping

def _write_csv(model_name, mapping):
    fout = model_name + CSV_NAME
    if os.path.exists(fout):
        with open(fout, "a") as fp:
            vals = map(lambda x: str(x), mapping.values())
            fp.write(','.join(vals) + '\n')
    else:
        with open(fout, "w") as fp:
            # write header, then content
            fp.write(','.join(mapping.keys()) + '\n')
            vals = map(lambda x: str(x), mapping.values())
            fp.write(','.join(vals) + '\n')


def main(model_name, fname):
    last_mod = 0
    while True:
        # if file was modified since last visited
        if last_mod < os.path.getmtime(fname):
            # update timestamp first
            last_mod = os.path.getmtime(fname)
            _write_csv(model_name, _read_model(fname))

        time.sleep(WATCH_INTERVAL)


if __name__ == '__main__':
    main(MODEL_NAME, MODEL_PATH)
