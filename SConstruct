sources = Glob("*.cpp")
target  = "qualia"
env = Environment()
lib = env.SharedLibrary(target = target, source = sources)

prefix = ARGUMENTS.get('PREFIX', "/usr/local")
installPath = prefix + "/lib"

env.Install(installPath, lib)
env.Alias("install", [installPath])

#execfile("./tools/scons/SConstruct.new")
#
#SConscript("tests/subsconstest/SConscript")

#SConscript("tests/unit/SConscript")
#SConscript("tests/example/SConscript")
#SConscript("tests/libmapper/SConscript")
