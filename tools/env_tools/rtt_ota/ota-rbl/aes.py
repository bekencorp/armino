import aes_tables

class AES:
    def __init__(self, key):
        self.expanded_key = self._expand_key(key)
   
    def encrypt(self, text):
        return self._process(text, 0, 1)

    def decrypt(self, text):
        return self._process(text, 14, -1)

    def _process(self, text, initial_round, round_factor):
        state_matrix = self._sequence_to_matrix(text)
        round = initial_round

        self._add_round_key(state_matrix, self._get_round_key(round))
        round += round_factor

        for i in range(0, 13):
            self._substitute_bytes(state_matrix, aes_tables.s_boxes[round_factor])
            self._shift_rows(state_matrix, round_factor)
            self._mix_columns(state_matrix, aes_tables.mix_column_constant_matrices[round_factor])

            round_key = self._get_round_key(round)
            if round_factor == -1: self._mix_columns(round_key, aes_tables.mix_column_constant_matrices[round_factor])
            self._add_round_key(state_matrix, round_key)

            round += round_factor

        self._substitute_bytes(state_matrix, aes_tables.s_boxes[round_factor])
        self._shift_rows(state_matrix, round_factor)
        self._add_round_key(state_matrix, self._get_round_key(round))

        return self._matrix_to_sequence(state_matrix)

    def _substitute_bytes(self, state_matrix, s_box):
        for row_index in range(0, 4):
            for column_index in range(0, 4):
                state_matrix[row_index][column_index] = s_box[state_matrix[row_index][column_index]]

        return state_matrix

    def _shift_rows(self, state_matrix, round_factor):
        for row_index in range(1, 4):
            state_matrix[row_index] = state_matrix[row_index][round_factor * row_index:] + state_matrix[row_index][:round_factor * row_index]

        return state_matrix

    def _mix_columns(self, state_matrix, constant_matrix):
        for column_index in range(0, 4):
            column = bytearray([
                state_matrix[0][column_index], 
                state_matrix[1][column_index], 
                state_matrix[2][column_index], 
                state_matrix[3][column_index]
            ])

            for row_index in range(0, 4):
                state_matrix[row_index][column_index] = aes_tables.multiplication[constant_matrix[row_index][0]][column[0]]
                state_matrix[row_index][column_index] ^= aes_tables.multiplication[constant_matrix[row_index][1]][column[1]]
                state_matrix[row_index][column_index] ^= aes_tables.multiplication[constant_matrix[row_index][2]][column[2]]
                state_matrix[row_index][column_index] ^= aes_tables.multiplication[constant_matrix[row_index][3]][column[3]]

        return state_matrix

    def _add_round_key(self, state_matrix, round_key_matrix):
        for row_index in range(0, 4):
            for column_index in range(0, 4):
                state_matrix[row_index][column_index] ^= round_key_matrix[row_index][column_index]

        return state_matrix

    def _get_round_key(self, round):
        key_column_index = 4 * round
        round_key = [
            self.expanded_key[0][key_column_index:key_column_index + 4],
            self.expanded_key[1][key_column_index:key_column_index + 4],
            self.expanded_key[2][key_column_index:key_column_index + 4],
            self.expanded_key[3][key_column_index:key_column_index + 4],
        ]

        return round_key

    def _expand_key(self, key):
        expanded_key = bytearray(key)
        n = 1

        step = 0
        while len(expanded_key) < 240:
            temporary_key = expanded_key[-4:]

            if step == 0:
                # rotate word
                temporary_key = temporary_key[1:] + temporary_key[:1]

                # substitue word
                for i in range(0, 4):
                    temporary_key[i] = aes_tables.s_box[temporary_key[i]]

                # xor round constant
                temporary_key[0] ^= aes_tables.rcon[n]

                n += 1
            elif step == 4:
                for i in range(0, 4):
                    temporary_key[i] = aes_tables.s_box[temporary_key[i]]

            for i in range(0, 4):
                temporary_key[i] ^= expanded_key[-16 + i]

            expanded_key += temporary_key
            step = (step + 1) % 8
            

        return self._sequence_to_matrix(expanded_key)


    def _sequence_to_matrix(self, sequence):
        matrix = [bytearray(), bytearray(), bytearray(), bytearray()]

        row = 0
        for element in sequence:
            matrix[row].append(element)
            row = (row + 1) % 4

        return matrix

    def _matrix_to_sequence(self, matrix):
        sequence = bytearray()

        column_length = len(matrix[0])
        for column in range(0, column_length):
            for row in range(0, 4):
                sequence.append(matrix[row][column])

        return sequence