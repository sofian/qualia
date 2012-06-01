# Get mode.
AddOption('--avr',
          dest='avr',
          action="store_true",
          default=False,
          help="AVR mode");
AddOption('--arduino',
          dest='arduino',
          action="store_true",
          default=False,
          help="Arduino mode");

if GetOption('arduino'):
  MODE = 'arduino'
elif GetOption('avr'):
  MODE = 'avr'
else:
  MODE = 'computer'

SConscript("src/SConscript", variant_dir="build/", duplicate=0, exports=["MODE"])