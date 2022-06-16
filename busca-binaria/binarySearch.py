import struct 
import sys

if len(sys.argv) != 2:
  print("USO python3 {} [CEP]".format(sys.argv[0]))
  quit()

registroCEP = struct.Struct("72s72s72s72s2s8s2s")
cepColumn = 5