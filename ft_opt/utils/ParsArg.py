import argparse


class ParsArg : 
    def __init__(self, desc):
        self.parser = argparse.ArgumentParser(description=desc)
        self.agrs = None
    
    def add_argument(self, *args, **kwargs):
        self.parser.add_argument(*args, **kwargs)
    
    def parse(self):
        self.args = self.parser.parse_args()
        return self.args
    
    def get(self, name):
        if self.args is None:
            raise RuntimeError("Tu dois appeler parse() avant get().")
        return getattr(self.args, name)



# if __name__ == "__main__":
#     cli = ParsArg("Script de test avec classe ParsArg")
#     cli.add_argument("-g", "--generate", help="Votre nom")
#     cli.add_argument("-v", "--verbose", action="store_true", help="Mode verbeux")
# 2
#     args = cli.parse()

#     nom = cli.get("nom")
#     verbose = cli.get("verbose")

#     if verbose:
#         print(f"Bonjour {nom}, le mode verbeux est activé.")
#     else:
#         print(f"Bonjour {nom} !")