import binascii

from aes import AES

class XTSAES:
    def __init__(self, key, tweak):
        self.aes = AES(key[:16])
        self.tweak = AES(key[16:]).encrypt(tweak)

    def encrypt(self, plain_text):
        return self._process_data(plain_text, self.aes.encrypt, True)

    def decrypt(self, cipher_text):
        return self._process_data(cipher_text, self.aes.decrypt, False)

    def _process_data(self, data, encryptor, is_last_tweaks_in_order):
        tweak = self.tweak[:]

        blocks = [data[i:i + 16] for i in range(0, len(data), 16)]
        if len(blocks[-1]) == 16:
            blocks.append(b'')

        for i in range(0, len(blocks) - 2):
            blocks[i] = self._process_block(blocks[i], tweak, encryptor)
            tweak = self._calculate_next_tweak(tweak)

        partial_length = len(blocks[-1])
        if partial_length == 0: # multiple of block size
            blocks[-2] = self._process_block(blocks[-2], tweak, encryptor)
        else: # not multiple of block size
            if is_last_tweaks_in_order: # in-order
                first_tweak = tweak
                second_tweak = self._calculate_next_tweak(tweak)
            else: # reverse-order
                second_tweak = tweak
                first_tweak = self._calculate_next_tweak(tweak)


            cc = self._process_block(blocks[-2], first_tweak, encryptor)
            pp = blocks[-1] + cc[partial_length:]
            blocks[-1] = cc[:partial_length]
            blocks[-2] = self._process_block(pp, second_tweak, encryptor)

        return b''.join(blocks)


    def _process_block(self, block, tweak, encryptor):
        new_block = map(lambda x, y: x ^ y, block, tweak)
        new_block = encryptor(new_block)
        new_block = map(lambda x, y: x ^ y, new_block, tweak)

        return bytearray(new_block)

    def _calculate_next_tweak(self, tweak):
        next_tweak = bytearray()

        carry_in = 0
        carry_out = 0

        for j in range(0, 16):
            carry_out = (tweak[j] >> 7) & 1
            next_tweak.append(((tweak[j] << 1) + carry_in) & 0xFF)
            carry_in = carry_out

        if carry_out:
            next_tweak[0] ^= 0x87

        return next_tweak
