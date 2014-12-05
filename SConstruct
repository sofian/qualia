platform = ARGUMENTS.get("platform", "computer")
mode     = ARGUMENTS.get("mode", "release")
precision = ARGUMENTS.get("precision", "float")

SConscript("src/SConscript", variant_dir="build/" + platform + "/qualia/", duplicate=0, exports=["platform","mode","precision"])
