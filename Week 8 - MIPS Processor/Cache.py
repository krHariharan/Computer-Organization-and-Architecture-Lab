from util import int_to_bin


class Cache:
    def __init__(self, filename, icache):
        self.cache = []
        self.icache = icache
        with open(filename, 'r') as file:
            for i in range(64):
                four_bytes = [int(file.readline(), 16) for i in range(4)]
                self.cache.append(four_bytes)

    def read(self, address: int):
        address = int_to_bin(address)
        block_index = int(address[:-2], 2)
        block_offset = int(address[-2:], 2)
        cache_block = self.cache[block_index]
        if not self.icache:
            return cache_block[block_offset]
        else:
            return int_to_bin(cache_block[block_offset]) + int_to_bin(cache_block[block_offset + 1])

    def write(self, address: int, data: int):
        address = int_to_bin(address)
        block_index = int(address[:-2], 2)
        block_offset = int(address[-2:], 2)
        self.cache[block_index][block_offset] = data

    def DcacheOut(self):
        with open("DCacheOut.txt", 'w') as outfile:
            for block in self.cache:
                out = [hex(item)[2:].zfill(2)+"\n" for item in block]
                outfile.writelines(out)


DCache = Cache("DCache.txt", False)
ICache = Cache("ICache.txt", True)
