#!/bin/bash

CONTAINER=ft_onion
SSH_PORT=4242
SSH_USER=onion
SSH_KEY="$HOME/.ssh/id_rsa"
FAKE_KEY="$HOME/.ssh/id_rsa_fake"
AUTHORIZED_FILE="/home/$SSH_USER/.ssh/authorized_keys"

# Générer une fausse clé si elle n'existe pas
if [ ! -f "$FAKE_KEY" ]; then
  ssh-keygen -q -t rsa -N "" -f "$FAKE_KEY"
fi

echo "======== 🔐 Test SSH sécurisé (ft_onion) ========="

# 1. Connexion SSH avec clé
echo -n "[1] Connexion SSH par clé... "
if ssh -o BatchMode=yes -o StrictHostKeyChecking=no -i "$SSH_KEY" -p $SSH_PORT $SSH_USER@localhost "echo OK" 2>/dev/null | grep -q "OK"; then
  echo "✅ Réussie"
else
  echo "❌ Échec"
fi

# 2. Connexion par mot de passe refusée
echo -n "[2] Connexion par mot de passe... "
if command -v sshpass >/dev/null 2>&1; then
  if sshpass -p "fakepass" ssh -o PreferredAuthentications=password -o PubkeyAuthentication=no -p $SSH_PORT $SSH_USER@localhost "echo OK" 2>/dev/null; then
    echo "❌ Mot de passe accepté (faille !)"
  else
    echo "✅ Refusée (ok)"
  fi
else
  echo "⚠️ sshpass non installé, test ignoré"
fi

# 3. Connexion root interdite
echo -n "[3] Connexion root... "
if ssh -o BatchMode=yes -i "$SSH_KEY" -p $SSH_PORT root@localhost "echo OK" 2>/dev/null; then
  echo "❌ Connexion root autorisée (faille !)"
else
  echo "✅ Refusée (ok)"
fi

# 4. sshd actif dans le conteneur
echo -n "[4] Service sshd actif... "
if docker exec $CONTAINER pgrep -x sshd >/dev/null; then
  echo "✅ Actif"
else
  echo "❌ Inactif"
fi

# 5. Permissions .ssh
echo -n "[5] Permissions .ssh/... "
ssh_perm=$(docker exec $CONTAINER stat -c "%a" /home/$SSH_USER/.ssh)
auth_perm=$(docker exec $CONTAINER stat -c "%a" $AUTHORIZED_FILE)
if [[ "$ssh_perm" == "700" && "$auth_perm" == "600" ]]; then
  echo "✅ OK ($ssh_perm/.ssh, $auth_perm/authorized_keys)"
else
  echo "❌ Mauvais droits ($ssh_perm/.ssh, $auth_perm/authorized_keys)"
fi

# 6. Clé publique présente dans authorized_keys
echo -n "[6] Clé publique présente dans authorized_keys... "
pubkey=$(cat "${SSH_KEY}.pub")
if docker exec $CONTAINER grep -q "$pubkey" "$AUTHORIZED_FILE"; then
  echo "✅ Oui"
else
  echo "❌ Non"
fi

# 7. sshd_config sécurisé ?
echo -n "[7] sshd_config sécurisé... "
sshd_config=$(docker exec $CONTAINER cat /etc/ssh/sshd_config)
if echo "$sshd_config" | grep -q "^PasswordAuthentication no" &&
   echo "$sshd_config" | grep -q "^PermitRootLogin no" &&
   echo "$sshd_config" | grep -q "^PubkeyAuthentication yes" ; then
   echo "✅ OK"
else
   echo "❌ Incomplet"
fi

echo "======== 🔒 Test Fail2Ban (dans conteneur) ========="

# 8. Vérifier que le service fail2ban tourne
echo -n "[8] Service fail2ban actif... "
if docker exec $CONTAINER pgrep -x fail2ban-server >/dev/null; then
  echo "✅ Oui"
else
  echo "❌ Non"
fi

# 9. Vérifier que la jail sshd est active
echo -n "[9] Jail sshd active... "
if docker exec $CONTAINER fail2ban-client status sshd 2>/dev/null | grep -q "Status for the jail: sshd"; then
  echo "✅ Oui"
else
  echo "❌ Non"
fi

# 10. Nombre d’échecs actuels
echo -n "[10] Tentatives échouées détectées... "
FAILS=$(docker exec $CONTAINER fail2ban-client status sshd 2>/dev/null | grep "Total failed" | awk '{print $NF}')
echo "🔎 $FAILS échecs"


# 11. Tentatives bruteforce avec une mauvaise clé
echo "[11] Forçage SSH avec clé invalide (3 échecs détectables)... "

FAKE_KEY="$HOME/.ssh/id_rsa_fake"
if [ ! -f "$FAKE_KEY" ]; then
  ssh-keygen -q -t rsa -N "" -f "$FAKE_KEY"
fi

for i in {1..3}; do
  ssh -i "$FAKE_KEY" -o BatchMode=yes -o PreferredAuthentications=publickey \
      -o PubkeyAuthentication=yes -o StrictHostKeyChecking=no \
      -p $SSH_PORT $SSH_USER@localhost "exit" >/dev/null 2>&1
done

sleep 5


# 12. Vérification bannissement
# 12. Vérification bannissement
echo -n "[12] Vérification bannissement... "
BANNED_IP=$(docker exec $CONTAINER fail2ban-client status sshd | grep 'Banned IP list' | cut -d: -f2 | xargs)

if [ -n "$BANNED_IP" ]; then
  echo "✅ $BANNED_IP bannie"
else
  echo "❌ IP NON bannie"
fi

# 13. Nettoyage : débannir IP pour tests suivants
echo -n "[13] Débannir IP... "
if [ -n "$BANNED_IP" ]; then
  docker exec $CONTAINER fail2ban-client set sshd unbanip "$BANNED_IP" >/dev/null 2>&1
  sleep 2
  if docker exec $CONTAINER fail2ban-client status sshd | grep -q "$BANNED_IP"; then
    echo "❌ Échec"
  else
    echo "✅ OK"
  fi
else
  echo "⏭️ Aucune IP à débannir"
fi

echo "=================================================="
