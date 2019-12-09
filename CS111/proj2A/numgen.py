import random
import sys
import os
import commands

f = open("lab2_list.csv", 'a');

for sync_var in ["  --sync=m", " --sync=s"]:
    for yield_opt in [""]:# --yield=i", " --yield=d", " --yield=il", " --yield=dl"]:
        for iterations in ["1000"]: #"2", "4", "8", "16", "32"]:
            for thread_count in ["1", "2", "4", "8", "12", "16", "24"]:
                status, output = 10, ""                
                status, output =  commands.getstatusoutput("./lab2-list " + "--iterations=" + iterations + " --threads=" + thread_count + sync_var + yield_opt)
                print output
                f.write(output + "\n")
