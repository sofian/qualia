# Tree growers
DIRS+=src/robgp/TreeGrower/

# Population operations
DIRS+=src/robgp/PopOperations/

# Crossovers
DIRS+=src/robgp/Crossover/

# Mutation
DIRS+=src/robgp/Mutation/

# Selection Methods
DIRS+=src/robgp/Selection/

# Tree Selection Methods
DIRS+=src/robgp/TreeSelection/

# Base objects
DIRS+=src/robgp/

# Find all source files
SRC+=$(wildcard $(DIRS:%/=%/*.cpp))

