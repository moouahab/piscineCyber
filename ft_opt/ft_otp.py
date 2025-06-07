from utils.ParsArg import ParsArg
from utils.OptGenerate import OptGenerate
from utils.OptKeyfile import OptKeyfile
from cryptography.fernet import Fernet
import sys
import os

def main():
    cli = ParsArg("TOTP Password Generator - 42 Project")

    cli.add_argument("-g", "--generate", action="store_true", help="Generate a secure OTP key")
    cli.add_argument("-k", "--keyfile", metavar="KEYFILE", help="Encrypted file to read for OTP")
    cli.add_argument("--key", metavar="HEXFILE", help="Hexadecimal key file (used with -g)")
    cli.add_argument("--secret", metavar="FERNETKEY", help="Base64-encoded Fernet key (optional)")
    cli.add_argument("positional_keyfile", nargs="?", help="Hex file (positional, works with -g)")

    args = cli.parse()

    if cli.get("generate"):
        hex_file = cli.get("key") or cli.get("positional_keyfile")
        if not hex_file:
            print("Error: Please provide a hexadecimal key file using --key or as a positional argument.")
            return
        if cli.get("secret"):
            secret_key = cli.get("secret").encode()
        elif cli.get("key"):
            secret_key = Fernet.generate_key()
            print("Generated Fernet key (save it securely):")
            print(secret_key.decode())
        else:
            secret_key = Fernet.generate_key()
            with open(".fernet.key", "wb") as f:
                f.write(secret_key)
        generator = OptGenerate(hex_file)
        generator.validate_and_read()
        generator.encrypt_and_store(secret_key)

    elif cli.get("keyfile"):
        encrypted_file = cli.get("keyfile")
        if cli.get("secret"):
            secret_key = cli.get("secret").encode()
        elif os.path.exists(".fernet.key"):
            with open(".fernet.key", "rb") as f:
                secret_key = f.read()
        else:
            print("Error: Please provide --secret or make sure .fernet.key exists.")
            return
        otp_handler = OptKeyfile(encrypted_file, secret_key)
        otp_handler.load_and_decrypt()
        otp = otp_handler.generate_otp()
        print(otp)
    else:
        print("Error: use either -g or -k.")
        cli.parser.print_help()

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
