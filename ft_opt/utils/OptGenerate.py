# utils/OptGenerate.py
from cryptography.fernet import Fernet
import os
import re

class OptGenerate:

    def __init__(self, hex_file, output_file="ft_otp.key"):
        self.hex_file = hex_file
        self.output_file = output_file
        self.key_bytes = None

    def validate_and_read(self):
        if not os.path.isfile(self.hex_file):
            raise FileNotFoundError("Fichier introuvable.")
        with open(self.hex_file, "r") as f:
            content = f.read().strip()
        if not re.fullmatch(r'[0-9a-fA-F]{64}', content):
            raise ValueError("./ft_otp: error: key must be 64 hexadecimal characters.")
        self.key_bytes = bytes.fromhex(content)

    def encrypt_and_store(self, secret_key):
        fernet = Fernet(secret_key)
        encrypted = fernet.encrypt(self.key_bytes)
        with open(self.output_file, "wb") as f:
            f.write(encrypted)
        print(f"Key was successfully saved in {self.output_file}.")
