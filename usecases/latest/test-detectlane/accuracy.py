import sys

def main(argv):
	if(len(argv) > 1):
		print("Too many arguments, try again")
		return

	with open(argv[0], 'r') as f:
		lines = f.readlines()

	numbers = []
	for line in lines:
		if "frame" in line:			
			number = (line.split('Detected frame: ')[1]).split('\n')[0]
			numbers.append(number)		

	total = len(numbers)

	max_val = 0
	for num in numbers:
		if int(num) > max_val:
			max_val = int(num)
	
	total_hit = float(total) / float(max_val)
	
	print("Hitrate: %.2f%%" % (total_hit*100))

if __name__ == "__main__":
    main(sys.argv[1:])