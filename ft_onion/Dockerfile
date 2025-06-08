FROM debian:bullseye

# Installer les services
RUN apt update && apt install -y \
    nginx \
    tor \
    rsyslog \
    fail2ban \
    openssh-server \
    curl \
    torsocks \
    iptables \
    && rm -rf /var/lib/apt/lists/*

    
RUN mkdir /var/run/sshd

# Créer un utilisateur non-root : onion (mot de passe désactivé, autorise la clé)
RUN useradd -m -s /bin/bash onion && \
    usermod -p '*' onion && \
    mkdir -p /home/onion/.ssh && \
    chown -R onion:onion /home/onion/.ssh && \
    chmod 700 /home/onion/.ssh
    
# Préparer SSH
RUN mkdir -p /var/lib/tor/hidden_service && \
    chown -R debian-tor:debian-tor /var/lib/tor/hidden_service && \
    chmod 700 /var/lib/tor/hidden_service

RUN echo "auth,authpriv.* /var/log/auth.log" > /etc/rsyslog.d/50-default.conf

# Activer le service de logs
RUN systemctl enable rsyslog

# Exposer les ports
EXPOSE 80 4242
