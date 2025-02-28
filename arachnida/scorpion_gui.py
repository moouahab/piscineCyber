import os
import argparse
import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ImageTk, ExifTags
import folium
import webbrowser

VALID_EXTENSIONS = (".jpg", ".jpeg", ".png", ".gif", ".bmp")

class ScorpionApp:
    def __init__(self, root):
        """Initialise l'interface Tkinter."""
        self.root = root
        self.root.title("Scorpion - Analyse EXIF")
        self.root.geometry("600x500")

        self.image_path = None
        self.image = None

        # Boutons
        self.btn_open = tk.Button(root, text="📂 Ouvrir une image", command=self.open_image)
        self.btn_open.pack(pady=10)

        self.info_label = tk.Label(root, text="Aucune image chargée", fg="blue")
        self.info_label.pack()

        self.canvas = tk.Canvas(root, width=300, height=300)
        self.canvas.pack()

        self.exif_text = tk.Text(root, height=8, width=60)
        self.exif_text.pack()

        self.btn_remove_exif = tk.Button(root, text="❌ Supprimer EXIF", command=self.remove_exif)
        self.btn_remove_exif.pack(pady=5)

        self.btn_gps = tk.Button(root, text="🌍 Voir GPS", command=self.show_gps)
        self.btn_gps.pack(pady=5)

    def open_image(self):
        """Ouvre une image et affiche les métadonnées."""
        file_path = filedialog.askopenfilename(filetypes=[("Images", "*.jpg *.jpeg *.png *.gif *.bmp")])
        if not file_path:
            return
        
        self.image_path = file_path
        self.image = Image.open(self.image_path)
        self.display_image()
        self.display_exif()

    def display_image(self):
        """Affiche l'image dans l'interface."""
        img_resized = self.image.resize((300, 300))
        img_tk = ImageTk.PhotoImage(img_resized)
        self.canvas.create_image(150, 150, image=img_tk)
        self.canvas.image = img_tk
        self.info_label.config(text=f"📷 Image : {os.path.basename(self.image_path)}")

    def display_exif(self):
        """Affiche les métadonnées EXIF si disponibles."""
        self.exif_text.delete(1.0, tk.END)

        # Vérifier si le format de l'image supporte les EXIF
        if self.image.format not in ["JPEG", "TIFF"]:
            self.exif_text.insert(tk.END, "⚠️ Pas de métadonnées EXIF pour ce format.\n")
            return

        # Récupérer les métadonnées EXIF
        exif_data = self.image._getexif()
        if exif_data:
            for tag_id, value in exif_data.items():
                tag_name = ExifTags.TAGS.get(tag_id, tag_id)
                self.exif_text.insert(tk.END, f"{tag_name}: {value}\n")
        else:
            self.exif_text.insert(tk.END, "⚠️ Aucune donnée EXIF trouvée.\n")


    def remove_exif(self):
        """Supprime les métadonnées EXIF et enregistre une nouvelle image."""
        if not self.image_path:
            messagebox.showerror("Erreur", "Ouvrez une image d'abord !")
            return

        clean_image = Image.new(self.image.mode, self.image.size)
        clean_image.putdata(list(self.image.getdata()))

        new_path = self.image_path.replace(".", "_clean.")
        clean_image.save(new_path)
        messagebox.showinfo("Succès", f"Métadonnées supprimées ! Image enregistrée sous {new_path}")

    def show_gps(self):
        """Affiche la position GPS sur une carte si disponible."""
        if not self.image_path:
            messagebox.showerror("Erreur", "Ouvrez une image d'abord !")
            return

        exif_data = self.image._getexif()
        if not exif_data:
            messagebox.showerror("Erreur", "Aucune donnée EXIF trouvée.")
            return

        gps_info = None
        for tag_id, value in exif_data.items():
            tag_name = ExifTags.TAGS.get(tag_id, tag_id)
            if tag_name == "GPSInfo":
                gps_info = value
                break

        if not gps_info:
            messagebox.showerror("Erreur", "Pas de coordonnées GPS dans cette image.")
            return

        # Convertir les données GPS en coordonnées décimales
        def convert_gps(coord, ref):
            deg, min, sec = coord
            result = deg + (min / 60.0) + (sec / 3600.0)
            return -result if ref in ["S", "W"] else result

        latitude = convert_gps(gps_info[2], gps_info[1])
        longitude = convert_gps(gps_info[4], gps_info[3])

        # Créer la carte avec Folium
        map_file = "gps_map.html"
        gps_map = folium.Map(location=[latitude, longitude], zoom_start=15)
        folium.Marker([latitude, longitude], popup="Lieu de la photo").add_to(gps_map)
        gps_map.save(map_file)

        # Ouvrir la carte dans le navigateur
        webbrowser.open(map_file)

if __name__ == "__main__":
    root = tk.Tk()
    app = ScorpionApp(root)
    root.mainloop()
