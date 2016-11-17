import sys

def main():
	with open("log.txt") as f:
		content = f.readlines()
	rhos = []
	thetas = []
	for line in content:
		if "rho:" in line:
			rhos.append(line[(line.find('rho:')+5):len(line)])
		elif "theta:" in line:
			thetas.append(line[(line.find('theta:')+7):len(line)])
	o1 = open("rhos.txt","w+")
	for line in rhos:
		o1.write(line)
	o1.close()
	o2 = open("thetas.txt","w+")
	for line in thetas:
		o2.write(line)
	o2.close()
	
if __name__ == '__main__':
    main()
