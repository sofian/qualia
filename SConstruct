platform = ARGUMENTS.get("platform", "computer")
mode     = ARGUMENTS.get("mode", "release")

SConscript("src/SConscript", variant_dir="build/" + platform + "/qualia/", duplicate=0, exports=["platform","mode"])