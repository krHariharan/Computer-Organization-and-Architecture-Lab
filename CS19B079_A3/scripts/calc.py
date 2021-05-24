f = open("output.txt")
o = open("timediff.txt", "w")
for line in f:
	val = line.split(',')
	val[1] = int(val[1])
	val[2] = int(val[2])
	val[3] = int(val[3])
	val[4] = int(val[4])
	# if value was >= 2^31, correcting -ve output
	if val[2] < 0:
		val[2] += 2**32
	if val[4] < 0:
		val[4] += 2**32
	#calculating time difference
	time  = (val[3]-val[1])*2**32 + (val[4]-val[2])
	o.write(str(time) + '\n')
