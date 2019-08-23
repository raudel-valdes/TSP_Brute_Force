import sys
import os
from re import sub

"""
Loops through each json files provided on the command line and passes each file
to the parser
"""
def main(argv):
  if len(argv) < 2:
      print('Usage: python3 Random#.tsp <path to tsp file>', file=sys.stderr)
      sys.exit(1)

  # loops over all tsp files in the argument
  for f in argv[1:]:
    if f.endswith('.tsp'):
      cordinates = open(f,'r')
      print("Success parsing " + f)
      print('**********************************************************')
      print(cordinates.read())
      cordinates.close()
      print('**********************************************************')

    else:
      print("This script only works with a file extension of .tsp")
      sys.exit(1)
            
if __name__ == '__main__':
    main(sys.argv)