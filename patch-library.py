#! /usr/bin/env python3

for (filename_original, filename_patched) in [
        ("libASICamera2.a"          , "libASICamera2_patched.a"),
        ("libASICamera2.so.0.0.0918", "libASICamera2_patched.so.0.0.0918")
    ]:

    with open(filename_original, "rb") as f:
        data = f.read()

    data = data.replace(b"libusb", b"libUSB")

    with open(filename_patched, "wb") as f:
        f.write(data)
