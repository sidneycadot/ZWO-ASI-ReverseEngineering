#! /usr/bin/env python3

import sys

for filename in ["libASICamera2.a", "libASICamera2.so.0.0.0918"]:

    with open(filename + ".orig", "rb") as f:
        data = f.read()

    data = data.replace(b"libusb", b"libUSB")

    with open(filename, "wb") as f:
        f.write(data)
