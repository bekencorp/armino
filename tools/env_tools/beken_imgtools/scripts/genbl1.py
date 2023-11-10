#!/usr/bin/env python3
import os
import logging
import json
import subprocess

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
    def run_cmd(self, cmd):
            p = subprocess.Popen(cmd, shell=True)
            ret = p.wait()
            if (ret):
                    logging.error(f'failed to run "{cmd}"')
                    exit(1)

    def validate_key(self):
        pass

    def sig_sch_to_prvkey(self):
        if (self.sig_sch == None):
            return ''

        if (self.sig_sch == 'ec256'):
            return 'ec256_privkey.pem'
        elif (self.sig_sch == 'ec521'):
            return 'ec521_privkey.pem'
        elif (self.sig_sch == 'rsa1024'):
            return 'rsa1024_privkey.pem'
        elif (self.sig_sch == 'rsa2048'):
            return 'rsa2048_privkey.pem'
        else:
            logging.error(f'unknow signature scheme {self.sig_sch}, should be ec256/ec521/rsa1024/rsa2048')
            exit(1)

    def sig_sch_to_pubkey(self):
        if (self.sig_sch == None):
            return ''

        if (self.sig_sch == 'ec256'):
            return 'ec256_pubkey.pem'
        elif (self.sig_sch == 'ec521'):
            return 'ec521_pubkey.pem'
        elif (self.sig_sch == 'rsa1024'):
            return 'rsa1024_pubkey.pem'
        elif (self.sig_sch == 'rsa2048'):
            return 'rsa2048_pubkey.pem'
        else:
            logging.error(f'unknow signature scheme {self.sig_sch}, should be ec256/ec521/rsa1024/rsa2048')
            exit(1)

    def sig_sch_to_scheme(self):
        if (self.sig_sch == None):
            return ''

        if (self.sig_sch == 'ec256'):
            return 'ECDSA_256_SHA256'
        elif (self.sig_sch == 'ec521'):
            return 'ECDSA_521_SHA256'
        elif (self.sig_sch == 'rsa1024'):
            return 'RSASSA_1024_PKCS_V21_SHA256'
        elif (self.sig_sch == 'rsa2048'):
            return 'RSASSA_2048_PKCS_V21_SHA256'
        else:
            logging.error(f'unknow signature scheme {self.sig_sch}, should be ec256/ec521/rsa1024/rsa2048')
            exit(1)
 
    def gen_key(self):
        if os.path.exists(self.prvkey) and os.path.exists(self.pubkey):
            logging.debug(f'private/public key exists, skip key generation')
            return

        self.run_cmd(f'./gen_key.sh {self.sig_sch} {self.prvkey} {self.pubkey}')

    def parse_key_config_json(self):
        if os.path.exists(self.key_config_json_file) == False:
            logging.error(f'{self.key_config_json_file} not exists')
            exit(1)

        with open(self.key_config_json_file, 'r') as key_file:
            self.key_config_json = json.load(key_file)

        self.is_sec_boot = self.key_config_json['sec_boot']
        if ("sec_boot" in self.key_config_json):
            self.is_sec_boot = self.key_config_json['sec_boot']
        logging.debug(f'sec_boot={self.is_sec_boot}')

        self.sig_sch = 'ec256'
        if ("sig_sch" in self.key_config_json):
            self.sig_sch = self.key_config_json['sig_sch']
        logging.debug(f'sig_sch={self.sig_sch}')

        if ("prvkey" in self.key_config_json):
            self.prvkey = self.key_config_json['prvkey']
        else:
            self.prvkey = self.sig_sch_to_prvkey()
        logging.debug(f'prvkey={self.prvkey}')

        if ("pubkey" in self.key_config_json):
            self.pubkey = self.key_config_json['pubkey']
        else:
            self.pubkey = self.sig_sch_to_pubkey()
        logging.debug(f'pubkey={self.pubkey}')

        self.scheme = self.sig_sch_to_scheme()
        logging.debug(f'scheme={self.scheme}')

        self.gen_key()

    def gen_key_desc(self):
        self.out_key_desc_file = 'key_desc.json'
        logging.debug(f'start to gen key description file')
        with open(self.out_key_desc_file, 'w+') as f:
            s_key['mnft_sig_cfg']['mnft_sig_sch'] = self.scheme
            s_key['mnft_sig_cfg']['mnft_prvkey'] = self.prvkey
            s_key['mnft_sig_cfg']['mnft_pubkey'] = self.pubkey
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


