import sys

if len(sys.argv) < 2:
    print("usage: python separate_regrouped.py file1 file2 ...")
    print("   or: python separate_regrouped.py file*")
    exit()

jec_files = sys.argv[1:]

for jec_file in jec_files:
    print("Parsing {}".format(jec_file))
    with open(jec_file, "r") as f:
        group_name = None
        group_lines = {}
        for line in f.readlines():
            if line[0] == "[":
                group_name = line[1:-2]
                group_lines[group_name] = []
            else:
                group_lines[group_name].append(line[:-1].replace("JECSource", "Uncertainty"))

    for group_name, jec_lines in group_lines.items():
        new_jec_file = (jec_file.replace("Regrouped", "Ungrouped")
                                .replace(".txt", "_{}.txt".format(group_name)))
        with open(new_jec_file, "w") as f:
            f.write("\n".join(jec_lines))
            f.write("\n")
            print("Wrote {}".format(new_jec_file))

