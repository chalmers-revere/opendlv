import sys

def main(argv):
	if (len(argv) > 2):
		print("Too many arguments, try again")
		return
	elif (len(argv) == 1):
		print("No output file specified, try again")
		return
	elif (len(argv) == 0):
		print("No files specified, try again")
		return

	newList = []

	with open(argv[0], 'r') as f:
		lines = f.readlines()
		for line in lines:
			newList.append(line[38:])
	
	with open(argv[1], 'w') as f:
	    for line in newList:
	    	f.write(line)

if __name__ == "__main__":
    main(sys.argv[1:])