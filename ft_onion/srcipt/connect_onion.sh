#!/bin/bash

CONTAINER="ft_onion"
ONION_FILE="/var/lib/tor/hidden_service/hostname"

# 🔍 Vérifier si le conteneur tourne
if ! docker ps | grep -q "$CONTAINER"; then
  echo "❌ Le conteneur $CONTAINER n'est pas en cours d'exécution."
  exit 1
fi

# 🧅 Récupérer l'adresse .onion
ONION_ADDRESS=$(docker exec "$CONTAINER" cat "$ONION_FILE" 2>/dev/null | tr -d '\r')

if [[ -z "$ONION_ADDRESS" ]]; then
  echo "❌ Impossible de récupérer l'adresse .onion"
  exit 1
fi

echo "✅ Adresse .onion récupérée : $ONION_ADDRESS"

# 🌍 Tester l'accès via torsocks
if command -v torsocks >/dev/null 2>&1; then
  echo "🌐 Tentative de connexion via torsocks curl..."
  torsocks curl -s --max-time 10 "http://$ONION_ADDRESS" || echo "❌ Échec de la connexion (le service est peut-être indisponible)"
else
  echo "⚠️ torsocks non installé. Vous pouvez ouvrir l'adresse manuellement dans le navigateur Tor :"
  echo "➡️  http://$ONION_ADDRESS"
fi
