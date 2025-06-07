from cryptography.fernet import Fernet
import hmac
import hashlib
import time
import struct
import os

class OptKeyfile:
    def __init__(self, encrypted_file, secret_key):
        self.encrypted_file = encrypted_file
        self.secret_key = secret_key
        self.secret_bytes = None

    def load_and_decrypt(self):
        if not os.path.isfile(self.encrypted_file):
            raise FileNotFoundError("Encrypted file not found.")
        with open(self.encrypted_file, "rb") as f:
            encrypted = f.read()
        fernet = Fernet(self.secret_key)
        self.secret_bytes = fernet.decrypt(encrypted)

    def generate_otp(self):
        if self.secret_bytes is None:
            raise RuntimeError("The key has not been decrypted.")
        timestep = int(time.time()) // 30
        message = struct.pack(">Q", timestep)
        hmac_hash = hmac.new(self.secret_bytes, message, hashlib.sha1).digest()
        offset = hmac_hash[-1] & 0x0F
        code = (struct.unpack(">I", hmac_hash[offset:offset + 4])[0] & 0x7FFFFFFF) % 1000000
        return f"{code:06d}"
