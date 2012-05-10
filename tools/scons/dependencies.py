from sets import Set
import os
from os import getcwd
import re
from os.path import *

# From http://code.activestate.com/recipes/502263/
# By Paul Rubin
def unique(seq, keepstr=True):
  t = type(seq)
  if t in (str, unicode):
    t = (list, ''.join)[bool(keepstr)]
  seen = []
  return t(c for c in seq if not (c in seen or seen.append(c)))
    
def _findFile(filename, paths):
  # print"Trying to find " + filename + " in paths " + str(paths)
  ret = filter(lambda f: exists(f), map(lambda p: join(p, filename), paths))
  if len(ret) == 0:
#    print "Include filename " + filename + " not found"
    return False
  elif len(ret) > 1:
    print "ERROR: Two candidates found for filename " + filename + ": " + str(ret)
    #exit(-1)
  else:
    return ret[0]

def getDependencies(sources, includePaths=["/usr/local/include", "/usr/include"], sourcePaths=[], extraHeaders=[]):
  sources = unique(map(abspath, sources))
  includePaths = unique(map(realpath, includePaths) + [getcwd()])
  sourcePaths = unique(map(realpath, sourcePaths) + [getcwd()])
  (inc, src) = _recursiveGetDependencies(sources, extraHeaders, includePaths, sourcePaths)
  return (list(inc), list(src))

# Dependencies find
# NOTE:
def _recursiveGetDependencies(sources, headers, includePaths=[], sourcePaths=[], includes = Set([])):
#  print "Calling getDep(" + str(sources) + "," + str(headers) + ',' + str(includePaths) + "," + str(sourcePaths) + "," + str(includes)
  currentDepthIncludes = Set([]) # Files included for that depth
  includeRegexp = re.compile(r'^[ ]*#[ ]*include [<"](.*)\.h[>"]')
  allSources = sources + headers

  # Run through all source files, detect the includes and add them to currentDepthIncludes
  for source in allSources:
    # print"Looking into " + source
    sourceIncludePaths = unique(includePaths + [dirname(source)])
    for line in open (source):
      result = includeRegexp.findall(line)
      if result:
        basename = result[0]
#        print "Found " + basename
        filename = basename + '.h'
        incFilename = _findFile(filename, sourceIncludePaths)
        if incFilename:
          incFileRealpath = realpath(incFilename)
          if (incFileRealpath not in includes):
            currentDepthIncludes.add(incFileRealpath)
        #else:
          #print "ERROR: File " + filename + " included from file " \
          #  + source + " not found in include paths"
          #exit(-1)

  # Construct the next list of sources (.c, .cpp, .cxx) by looking at files with the same
  # name as the include file (eg. if "goo.h" we will look for "goo.c", "goo.cpp", "goo.cxx"
  # files in the sourcePaths
  currentDepthSources = Set([])
  for filepath in currentDepthIncludes:
    (directory, filename) = split(filepath)
    (basename, extension) = splitext(filename)
    # print"Looking at " + filepath + "(" + directory + "," + filename + "," + basename + ")"
    for ext in ['.c', '.cpp', '.cxx']:
      file = _findFile(basename + ext, unique(sourcePaths + [directory]))
      #print"Trying to find file " + basename + ext
      if file:
        #print"File found, adding it"
        currentDepthSources.add(file)

  includes = includes | currentDepthIncludes
  sources = Set(sources)
  sources = sources | currentDepthSources
  if currentDepthSources or currentDepthIncludes:
#    print "CurrentDepthSrc = " + str(currentDepthSources)
#    print "CurrentDepthInc = " + str(currentDepthIncludes)
#    print "Current includes = " + str(includes)
#    print "Unite includes = " + str(includes)
    (inc, src) = _recursiveGetDependencies(list(currentDepthSources), list(currentDepthIncludes), includePaths, sourcePaths, includes)
    includes = includes | inc
    sources = sources | src

  return (includes, sources)