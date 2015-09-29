#! /usr/bin/env python3

import sys, os

for filename_in in sys.argv[1:]

    (root, ext) = os.path.splitext(filename_in)

    filename_out = root + "_patched" + ext

    print("Patching file '{}' to '{}' ...".format(filename_in, filename_out))

    with open(filename_in, "rb") as f:
        data = f.read()

    data = data.replace(b"libusb", b"libUSB")

    with open(filename_out, "wb") as f:
        f.write(data)
