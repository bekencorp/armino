#!/usr/bin/env python3
import os
import json
import logging

class check_sha384:
    def __init__(self, infile):
        self.json_file = infile

    def check_json_data(self):
        with open(self.json_file, 'r') as json_file:
            try:
                json_data = json.load(json_file)
                if (json_data['mnft_sig_cfg']['pubkey_hash_sch'] == 'SHA384'):
                    json_data['mnft_sig_cfg']['pubkey_hash_sch'] = 'SHA1'
                    return 1
                else:
                    return 0
            except Exception as e:
                logging.error(f"Could not open : '{json_file}': {e}", err=True)
