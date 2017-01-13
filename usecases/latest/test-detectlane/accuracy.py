import sys

def main(argv):
	if (len(argv) > 1):
		print("Too many arguments, try again")
		return
	elif (len(argv) == 0):
		print("No file specified, try again")
		return

	with open(argv[0], 'r') as f:
		lines = f.readlines()

	hit2 = []
	hit3 = []
	hit = []
	toSmall = []
	toBig = []
	ExtremeBig = []

	for line in lines:
		if "frame2" in line:			
			number = (line.split('Detected frame2: ')[1]).split('\n')[0]
			hit2.append(int(number))	
		elif "frame3" in line:			
			number = (line.split('Detected frame3: ')[1]).split('\n')[0]
			hit3.append(int(number))	
		elif "Missed" in line:			
			numS = (line.split('Missed: ')[1]).split('\n')[0]
			toSmall.append(int(numS))	
		elif "Overhit" in line:			
			numB = (line.split('Overhit: ')[1]).split('\n')[0]
			toBig.append(int(numB))	
		elif "Tooo" in line:			
			numE = (line.split('detected: ')[1]).split('\n')[0]
			ExtremeBig.append(int(numE))		

	#total = len(hit2) + len(hit3)

	hit = hit2 + hit3	
	max_val = 0

	allLists = hit2 + hit3 + toSmall + toBig + ExtremeBig
	total = (len(allLists))

	for num in allLists:
		if int(num) > max_val:
			max_val = int(num)

	total_hit = float(len(hit)) / float(max_val)
	false_hit = float(len(toBig)) / float(max_val)
	
	print("Hitrate: %.2f%%" % (total_hit*100))
	print("Frames with 2 lines: " + str(len(hit2)))
	print("Frames with 3 lines: " + str(len(hit3)))
	print("Number of frames: " + str(max_val))
	print("Extremely big: " + str(len(ExtremeBig)))
	print("False Positive: " + str(len(toBig) + len(ExtremeBig)))
	print("False Positive rate: %.3f%%" % (false_hit))

	excluded = []
	for x in range(1, total):
		if(x not in hit):
			excluded.append(x)

	#print(excluded)

	ConnectedFrames = 1
	maxConnected = 0
	AllConnectedFrames = []
	onehit = 0 # number of just one connected miss
	twohit = 0 # number of two connected misses
	threehit = 0 # number of three connected misses
	fourup = 0 # number of four or more connected misses
	for i in range (0, len(excluded)-1):
		if excluded[i]+1 == excluded[i+1]:
			#print(str(excluded[i]) + " is connected to " + str(excluded[i+1]))
			#print("hit, nr: " + str(ConnectedFrames))
			ConnectedFrames += 1
		else:
			if ConnectedFrames > maxConnected:
				maxConnected = ConnectedFrames
			if ConnectedFrames > 2: # Filter out smaller amount of missed frames
				AllConnectedFrames.append(ConnectedFrames)
			
			if ConnectedFrames == 1:
				onehit += 1
			elif ConnectedFrames == 2:
				twohit += 1	
			elif ConnectedFrames == 3:
				threehit += 1
			elif ConnectedFrames > 4:
				fourup += 1		
			ConnectedFrames = 1

	print("All consecutive frames: " + str(AllConnectedFrames))
	print("Maximum consecutive frames : " + str(maxConnected))

	print("1 misses: " + str(onehit))
	print("2 misses: " + str(twohit))
	print("3 misses: " + str(threehit))
	print("4 or more misses: " + str(fourup))

	histogram = []
	for i in range (0, max_val):
		if i in excluded:
			histogram.append((i,0))
		else:
			histogram.append((i,1))
#	print '. '.join(map(str, histogram))
if __name__ == "__main__":
    main(sys.argv[1:])
