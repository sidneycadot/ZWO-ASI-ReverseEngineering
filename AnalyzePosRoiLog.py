#! /usr/bin/env python3

# Read log file

class TestCycle(object):
    def __init__(self, lines):
        self._lines = lines

        print(lines)

        n = len(lines)
        assert lines[0].startswith("ASISetStartPos") or lines[0].startswith("ASISetROIFormat")

        cmd            = lines[0].split()[0]
        cmd_parameters = map(int, lines[0].split()[1:])

        assert lines[n - 3].startswith(cmd + " exitcode")
        cmd_exitcode = int(lines[n - 3].split()[-1])

        assert lines[n - 2].startswith("ASIGetROIFormat")
        assert lines[n - 2].startswith("ASIGetROIFormat")
        assert lines[n - 1].startswith("ASIGetStartPos")

    def __str__(self):
        return "\n".join(self._lines)

tcycles = []

current_cycle_lines = []

with open("LOG1.txt", "r") as f:
    for line in f:
        line = line.rstrip()
        if line.startswith("START CYCLE"):
            current_cycle_lines = [] # flush
        elif line.startswith("END CYCLE"):
            tc = TestCycle(current_cycle_lines)
            tcycles.append(tc)
            current_cycle_lines = []
        else:
            current_cycle_lines.append(line)

print(len(tcycles))

for tc in tcycles:
    print(str(tc) +"\n")
