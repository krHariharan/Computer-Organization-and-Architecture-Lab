def int_to_bin(n):
    return '{0:08b}'.format(n)


def bin_to_signed(binstr, len):
    if binstr[0] == '1':
        tempstr = ""
        for i in range(len):
            if binstr[i] == '1':
                tempstr += '0'
            else:
                tempstr += '1'
        return - int(tempstr, 2) - 1
    else:
        return int(binstr, 2)

