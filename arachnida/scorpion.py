import os
import argparse
from PIL import Image
from PIL.ExifTags import TAGS

VALID_EXTENSIONS = (".jpg", ".jpeg", ".png", ".gif", ".bmp")

class Scorpion:
    def __init__(self, image_path):
        """Initialise l'image et vérifie si elle est valide."""
        self.image_path = image_path
        self.image = None  

        if self.is_valid_image():
            self.open_image()
        else:
            print(f"❌ {image_path} n'est pas une image valide.")

    def is_valid_image(self):
        """Vérifie si le fichier a une extension d'image valide."""
        return self.image_path.lower().endswith(VALID_EXTENSIONS)

    def open_image(self):
        """Ouvre l'image avec Pillow."""
        try:
            self.image = Image.open(self.image_path)
            print(f"✅ Image {self.image_path} chargée avec succès.")
        except Exception as e:
            print(f"❌ Erreur en ouvrant l'image : {e}")

    def display_info(self):
        """Affiche les informations de base de l'image."""
        if self.image:
            print(f"\n📷 Infos de l'image : {self.image_path}")
            print(f"📏 Format : {self.image.format}")
            print(f"📐 Dimensions : {self.image.size[0]}x{self.image.size[1]} pixels")
            print(f"🎨 Mode couleur : {self.image.mode}")

    def extract_exif(self):
        """Extrait et affiche les métadonnées EXIF."""
        if self.image is None:
            return

        exif_data = self.image._getexif()
        if exif_data:
            print("\n📜 Données EXIF :")
            for tag_id, value in exif_data.items():
                tag_name = TAGS.get(tag_id, tag_id)  # Convertit ID en nom
                print(f"   {tag_name}: {value}")
        else:
            print("⚠️ Aucune donnée EXIF trouvée.")

    def remove_exif(self, output_path):
        """Supprime les métadonnées EXIF et enregistre l'image nettoyée."""
        if self.image is None:
            print("❌ Impossible de modifier l'image.")
            return

        clean_image = Image.new(self.image.mode, self.image.size)
        clean_image.putdata(list(self.image.getdata()))
        clean_image.save(output_path)
        print(f"✅ Métadonnées supprimées : {output_path}")

    def modify_exif(self, key, new_value, output_path):
        """Modifie une métadonnée EXIF et sauvegarde l'image."""
        if self.image is None:
            print("❌ Impossible de modifier l'image.")
            return

        exif_data = self.image.info.get("exif")
        if exif_data:
            exif_dict = {TAGS.get(tag_id, tag_id): value for tag_id, value in self.image._getexif().items()}

            if key in exif_dict:
                exif_dict[key] = new_value
                print(f"✅ {key} changé en : {new_value}")
            else:
                print(f"⚠️ Clé EXIF {key} introuvable.")

            self.image.save(output_path, exif=exif_data)
            print(f"✅ Image enregistrée avec EXIF modifiés : {output_path}")
        else:
            print("⚠️ Aucune donnée EXIF trouvée.")


def parse_args():
    """Parse les arguments en ligne de commande."""
    parser = argparse.ArgumentParser(description="Analyse et modification des métadonnées des images.")
    parser.add_argument("files", nargs="+", help="Fichiers images à analyser")
    parser.add_argument("--remove", metavar="OUTPUT", help="Supprime les métadonnées EXIF et enregistre sous OUTPUT")
    parser.add_argument("--modify", nargs=3, metavar=("KEY", "VALUE", "OUTPUT"),
                        help="Modifie une métadonnée EXIF (KEY VALUE OUTPUT)")
    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()

    for file in args.files:
        print("\n" + "=" * 40)  # Séparation visuelle
        scorpion = Scorpion(file)
        scorpion.display_info()
        scorpion.extract_exif()

        if args.remove:
            scorpion.remove_exif(args.remove)

        if args.modify:
            key, value, output = args.modify
            scorpion.modify_exif(key, value, output)
