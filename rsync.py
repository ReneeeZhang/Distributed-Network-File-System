import os
import sys

if (__name__ == "__main__"):
  cnt = len(sys.argv)
  assert(cnt == 3)
  _, src, dst = sys.argv
  os.system("rsync -av {} {}".format(src, dst))
  print("rsync succeeds")