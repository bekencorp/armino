#!/usr/bin/env python3
import os
import logging
import json

s_bc_code = {\
    "//":                   "This is a generate file, do not modify it",\
    "description": "flash control configurations",\
    "tlvs": [\
        {"name": "msp",      "size": "4b",     "value": "0", "hex":"y"},\
        {"name": "pc",       "size": "4b",     "value": "0", "hex":"y"}\
    ]\
}

s_key = {\
    "//":                   "This is a generate file, do not modify it",\
\
    "key_desc_cfg": {\
        "fmt_ver":          "0x00010001"\
    },\
\
    "mnft_sig_cfg": {\
        "pubkey_hash_sch":  "SHA256",\
        "mnft_sig_sch":     "ECDSA_256_SHA256",\
        "mnft_prvkey":      "ec_256_privkey.pem",\
        "mnft_pubkey":      "ec_256_pubkey.pem"\
    },\
\
    "img_dgst_cfg" : {\
        "img_hash_sch":     "SHA256"\
    },\
\
    "//":                   "Optional, only required if image(s) encrypted.",\
\
\
    "img_enc_cfg": {\
        "enc_sch":          "AES-ECB",\
        "enc_key":          "B75AFF7160BE351026753874D38846BEDE916E167043382B313FF2FA54250B67",\
        "bits_enc_key":     256,\
        "//":               "optional if enc_sch doesn't need it",\
        "iv":               "11111111111111111111111111111111",\
        "//":               "In our BootROM hal implementation, the key blob is not used in converting to",\
        "//":               "the real encryption key. The HAL implementation compares the key blob",\
        "//":               "and selects the actual image encryption key. Details see hal_crypto_aes.c",\
        "key_blob":         "325AFF7160BE351026753874D38846BEDE916E167043382B313FF2FA54250B67",\
        "//":               "0000000000000000000000000000000000000000000000000000000000000001",\
        "bits_key_blob":    256\
    }\
}

s_man = {\
    "//":                   "This is a generate file, do not modify it",\
\
    "mnft_desc_cfg": {\
        "fmt_ver":          "0x00010001",\
        "mnft_ver":         "0x00000003",\
        "sec_boot":         True,\
    },\
    "imgs": [\
        {\
            "is_enc":       False,\
            "static_addr":  "0x02024000",\
            "load_addr":    "0x28040000",\
            "entry":        "0x28040000",\
            "path":         "bl2.bin"\
        }\
    ]\
}\

class Genbl1:
    def parse_key_config_json(self):
        if os.path.exists(self.key_config_json_file) == False:
            logging.error(f'{self.key_config_json_file} not exists')
            exit(1)

        with open(self.key_config_json_file, 'r') as key_file:
            self.key_config_json = json.load(key_file)

        if ("sec_boot" not in self.key_config_json):
            logging.error(f'{self.key_config_json_file} missing "sec_boot"')
            exit(1)
        self.is_sec_boot = self.key_config_json['sec_boot']

        if ("prvkey" not in self.key_config_json):
            logging.error(f'{self.key_config_json_file} missing "prvkey"')
            exit(1)
        self.prvkey = self.key_config_json['prvkey']

        if ("pubkey" not in self.key_config_json):
            logging.error(f'{self.key_config_json_file} missing "pubkey"')
            exit(1)
        self.pubkey = self.key_config_json['pubkey']

    def gen_key_desc(self):
        self.out_key_desc_file = 'key_desc.json'
        logging.debug(f'start to gen key description file')
        with open(self.out_key_desc_file, 'w+') as f:
            s_key['mnft_sig_cfg']['mnft_prvkey'] = self.key_config_json['prvkey']
            s_key['mnft_sig_cfg']['mnft_pubkey'] = self.key_config_json['pubkey']
            json.dump(s_key, f, indent=4, separators=(',', ':'))


    def gen_manifest(self, version, static_addr, load_addr="0x28040000", bin_name="bl2.bin", out_manifest_file="primary_manifest"):
        logging.debug(f'start to gen {out_manifest_file}')
        with open(out_manifest_file, 'w+') as f:
            s_man['mnft_desc_cfg']['fmt_ver'] = version
            s_man['mnft_desc_cfg']['sec_boot'] = self.is_sec_boot
            s_man['imgs'][0]['static_addr'] = static_addr
            s_man['imgs'][0]['load_addr'] = load_addr
            s_man['imgs'][0]['entry'] = load_addr
            s_man['imgs'][0]['path'] = bin_name
            json.dump(s_man, f, indent=4, separators=(',', ':'))

    def __init__(self, key_config_json_file='bl1_key.json'):
        self.key_config_json_file = key_config_json_file
        self.parse_key_config_json()

if __name__ == '__main__':
    logging.basicConfig()
    logging.getLogger().setLevel(logging.DEBUG)
    g = Genbl1("bl1_key.json")
    g.gen_key_desc()
    g.gen_manifest("0x0101", "0x1234567", "0x28040000", 'bl2.bin', 'primary_manifest.json')
    g.gen_manifest("0x0101", "0x1234567", "0x28040000", 'app.bin', 'secondary_manifest.json')


