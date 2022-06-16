import struct 
import sys

if len(sys.argv) != 2:
  print("USO python3 {} [CEP]".format(sys.argv[0]))
  quit()

registroCEP = struct.Struct("72s72s72s72s2s8s2s")
cepColumn = 5

with open("cep_ordenado.dat", "rb") as f:
  f.seek(0, 2)
  pos = f.tell()
  qnt = pos // registroCEP.size
  init = 0
  end = qnt - 1
  while init <= end:
    mid = (init + end) // 2
    f.seek(mid * registroCEP.size)
    line = f.read(registroCEP.size)
    record = registroCEP.unpack(line)
    if record[cepColumn].decode("latin1") == sys.argv[1]:
      print("CEP encontrado: {}".format(record[0].decode("latin1")))
      quit()
    elif record[cepColumn].decode("latin1") > sys.argv[1]:
      end = mid - 1
    else:
      init = mid + 1
  print("CEP não encontrado")
  quit()