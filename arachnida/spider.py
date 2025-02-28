import os
import argparse
import requests
from urllib.parse import urljoin, urlparse
from bs4 import BeautifulSoup

VALID_EXTENSIONS = (".jpg", ".jpeg", ".png", ".gif", ".bmp")

def is_valid_url(url):
    parsed = urlparse(url)
    return parsed.scheme in ["http", "https"]

def is_valid_image_url(url):
    parsed = urlparse(url)
    return parsed.path.lower().endswith(VALID_EXTENSIONS)

def clean_filename(url):
    parsed_url = urlparse(url)
    filename = os.path.basename(parsed_url.path)
    return filename

def download_image(image_url, save_path):
    """Télécharge une image avec gestion des erreurs."""
    try:
        response = requests.get(image_url, stream=True, timeout=10)
        response.raise_for_status()
        with open(save_path, "wb") as file:
            for chunk in response.iter_content(1024):
                file.write(chunk)
        print(f"✅ Image téléchargée : {save_path}")
    except requests.RequestException as e:
        print(f"❌ Erreur pour {image_url}: {e}")

class Spider:
    def __init__(self, url, ar):
        self.url = url
        self.visited_links = set()
        self.ar = ar
    
    def fetch_html(self, url):
        """Récupère le contenu HTML avec gestion des erreurs."""
        try:
            response = requests.get(url, timeout=10)
            response.raise_for_status()
            return response.text
        except requests.RequestException as e:
            print(f"❌ Erreur lors de la récupération de {url} : {e}")
            return ""

    def extract_images(self, html, base_url):
        """Extrait et convertit les URLs des images (filtrées par extension)."""
        soup = BeautifulSoup(html, "html.parser")
        images = [
            urljoin(base_url, img.get('src', img.get('data-src', '')))
            for img in soup.find_all("img")
            if 'src' in img.attrs or 'data-src' in img.attrs
        ]
        return [img for img in images if is_valid_image_url(img)]

    def extract_links(self, html, base_url):
        """Extrait et convertit les URLs des liens, en filtrant les invalides."""
        soup = BeautifulSoup(html, "html.parser")
        return [
            urljoin(base_url, a['href'])
            for a in soup.find_all("a")
            if 'href' in a.attrs and a['href'].strip() and is_valid_url(urljoin(base_url, a['href']))
        ]
    
    def download_images(self, images, folder):
        """Télécharge toutes les images dans un dossier."""
        if not os.path.exists(folder):
            os.makedirs(folder)

        for img_url in images:
            filename = os.path.join(folder, clean_filename(img_url))
            download_image(img_url, filename)

    
    def crawl(self, url, depth=0):
        """Parcourt récursivement les pages si -r est activé, avec la bonne profondeur."""
        if url in self.visited_links or depth > self.ar.l:
            return

        print(f"\n🔎 Scraping: {url} (Profondeur {depth})")
        self.visited_links.add(url)

        html = self.fetch_html(url)
        if not html:
            return  # Ne pas continuer si la page est vide

        images = self.extract_images(html, url)
        links = self.extract_links(html, url)

        self.download_images(images, folder=self.ar.p)

        if self.ar.r:
            for link in links:
                self.crawl(link, depth + 1)

def parseArg():
    parser = argparse.ArgumentParser(prog="spider", usage="%(prog)s [options] URL",
        description="Un programme Spider pour extraire des images.")
    
    parser.add_argument("url", help="L'URL à analyser")
    parser.add_argument("-r", action="store_true", help="Mode récursif")
    parser.add_argument("-l", type=int, help="Profondeur de recherche", default=5)
    parser.add_argument("-p", help="Chemin de sauvegarde", default="data")

    return parser.parse_args()

if __name__ == "__main__":
    ar = parseArg()

    spider = Spider(ar.url, ar)

    html_content = spider.fetch_html(ar.url)
    images = spider.extract_images(html_content, ar.url)
    links = spider.extract_links(html_content, ar.url)
    
    spider.download_images(images, folder=ar.p)
    
    if ar.r:
        spider.crawl(ar.url)
