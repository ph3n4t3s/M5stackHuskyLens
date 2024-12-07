#include "SecurityManager.h"
#include <SPIFFS.h>
#include <esp_random.h>
#include <mbedtls/base64.h>

SecurityManager::SecurityManager() : m_level(SecurityLevel::SEC_MEDIUM) {
    mbedtls_aes_init(&m_aesCtx);
}

bool SecurityManager::begin() {
    if (!SPIFFS.begin(true)) {
        return false;
    }
    
    generateMasterKey();
    loadCertificates();
    monitorSecurityEvents();
    
    return true;
}

void SecurityManager::setSecurityLevel(SecurityLevel level) {
    m_level = level;
    
    switch (level) {
        case SecurityLevel::SEC_LOW:
            m_config.enableEncryption = false;
            m_config.enableAuthentication = false;
            m_config.keyRotationInterval = 7 * 24 * 3600;  // 7 jours
            break;
            
        case SecurityLevel::SEC_MEDIUM:
            m_config.enableEncryption = true;
            m_config.enableAuthentication = true;
            m_config.keyRotationInterval = 24 * 3600;      // 1 jour
            break;
            
        case SecurityLevel::SEC_HIGH:
            m_config.enableEncryption = true;
            m_config.enableAuthentication = true;
            m_config.enableSecureBootload = true;
            m_config.enableTLS = true;
            m_config.keyRotationInterval = 3600;           // 1 heure
            break;
    }
}

std::vector<uint8_t> SecurityManager::encrypt(const std::vector<uint8_t>& data) {
    if (!m_config.enableEncryption) {
        return data;
    }
    
    std::vector<uint8_t> output(data.size() + 16);  // +16 pour le padding
    std::vector<uint8_t> iv(16);
    
    // Générer un IV aléatoire
    esp_fill_random(iv.data(), 16);
    
    // Chiffrer les données
    size_t outlen;
    mbedtls_aes_crypt_cbc(&m_aesCtx, MBEDTLS_AES_ENCRYPT,
                          data.size(), iv.data(),
                          data.data(), output.data());
    
    // Ajouter l'IV au début des données chiffrées
    output.insert(output.begin(), iv.begin(), iv.end());
    
    return output;
}

std::vector<uint8_t> SecurityManager::decrypt(const std::vector<uint8_t>& data) {
    if (!m_config.enableEncryption || data.size() < 16) {
        return data;
    }
    
    // Extraire l'IV
    std::vector<uint8_t> iv(data.begin(), data.begin() + 16);
    
    // Déchiffrer les données
    std::vector<uint8_t> output(data.size() - 16);
    size_t outlen;
    
    mbedtls_aes_crypt_cbc(&m_aesCtx, MBEDTLS_AES_DECRYPT,
                          data.size() - 16, iv.data(),
                          data.data() + 16, output.data());
    
    return output;
}

bool SecurityManager::authenticate(const String& username, const String& password) {
    if (!m_config.enableAuthentication) {
        return true;
    }
    
    // Charger les données utilisateur
    std::vector<uint8_t> storedData = loadSecureData("user_" + username);
    if (storedData.empty()) {
        return false;
    }
    
    // Vérifier le hash
    String storedHash((char*)storedData.data());
    std::vector<uint8_t> salt = generateSalt();
    
    return verifyHash(password, storedHash, salt);
}

SecurityToken SecurityManager::generateToken(const String& userID) {
    SecurityToken token;
    token.userID = userID;
    token.expiry = millis() + (m_config.sessionTimeout * 1000);
    
    // Générer un token aléatoire
    uint8_t randomBytes[32];
    esp_fill_random(randomBytes, 32);
    
    // Convertir en base64
    size_t outlen;
    mbedtls_base64_encode(nullptr, 0, &outlen,
                         randomBytes, 32);
    
    std::vector<uint8_t> base64Token(outlen);
    mbedtls_base64_encode(base64Token.data(), base64Token.size(), &outlen,
                         randomBytes, 32);
    
    token.token = String((char*)base64Token.data());
    
    // Charger les permissions de l'utilisateur
    auto it = m_userPermissions.find(userID);
    if (it != m_userPermissions.end()) {
        token.permissions = it->second;
    }
    
    // Stocker le token actif
    m_activeTokens[token.token] = token;
    
    return token;
}

bool SecurityManager::validateToken(const SecurityToken& token) {
    // Nettoyer les tokens expirés
    cleanExpiredTokens();
    
    // Vérifier si le token existe et n'est pas expiré
    auto it = m_activeTokens.find(token.token);
    if (it == m_activeTokens.end() || isTokenExpired(it->second)) {
        return false;
    }
    
    return true;
}

void SecurityManager::rotateKeys() {
    // Générer une nouvelle clé maître
    generateMasterKey();
    
    // Re-chiffrer toutes les données avec la nouvelle clé
    std::vector<String> dataKeys = {"config", "users", "permissions"};
    for (const auto& key : dataKeys) {
        auto data = loadSecureData(key);
        if (!data.empty()) {
            saveSecureData(key, data);
        }
    }
    
    // Journaliser l'événement
    logSecurityEvent("Key rotation performed");
}

bool SecurityManager::checkPermission(const SecurityToken& token, const String& resource) {
    if (!validateToken(token)) {
        return false;
    }
    
    // Vérifier les permissions
    for (const auto& perm : token.permissions) {
        if (perm == "*" || perm == resource) {
            return true;
        }
    }
    
    return false;
}

void SecurityManager::generateMasterKey() {
    m_masterKey.resize(32);  // 256 bits
    esp_fill_random(m_masterKey.data(), m_masterKey.size());
    
    // Initialiser le contexte AES
    mbedtls_aes_setkey_enc(&m_aesCtx, m_masterKey.data(), 256);
    mbedtls_aes_setkey_dec(&m_aesCtx, m_masterKey.data(), 256);
}

std::vector<uint8_t> SecurityManager::generateSalt() {
    std::vector<uint8_t> salt(16);
    esp_fill_random(salt.data(), salt.size());
    return salt;
}

void SecurityManager::cleanExpiredTokens() {
    auto now = millis();
    
    for (auto it = m_activeTokens.begin(); it != m_activeTokens.end();) {
        if (it->second.expiry < now) {
            it = m_activeTokens.erase(it);
        } else {
            ++it;
        }
    }
}

bool SecurityManager::isTokenExpired(const SecurityToken& token) {
    return token.expiry < millis();
}

bool SecurityManager::saveSecureData(const String& key, const std::vector<uint8_t>& data) {
    // Chiffrer les données
    auto encrypted = encrypt(data);
    
    // Sauvegarder dans SPIFFS
    File file = SPIFFS.open("/secure/" + key, "w");
    if (!file) {
        return false;
    }
    
    file.write(encrypted.data(), encrypted.size());
    file.close();
    
    return true;
}

std::vector<uint8_t> SecurityManager::loadSecureData(const String& key) {
    File file = SPIFFS.open("/secure/" + key, "r");
    if (!file) {
        return std::vector<uint8_t>();
    }
    
    // Lire les données chiffrées
    std::vector<uint8_t> encrypted(file.size());
    file.read(encrypted.data(), encrypted.size());
    file.close();
    
    // Déchiffrer et retourner
    return decrypt(encrypted);
}

void SecurityManager::monitorSecurityEvents() {
    // TODO: Implémenter la surveillance en temps réel
    // - Détecter les tentatives d'intrusion
    // - Surveiller les accès anormaux
    // - Vérifier l'intégrité du système
}

void SecurityManager::logSecurityEvent(const String& event) {
    // Format : timestamp|niveau|événement
    String logEntry = String(millis()) + "|" + 
                     String(static_cast<int>(m_level)) + "|" +
                     event + "\n";
    
    File file = SPIFFS.open("/secure/security.log", "a");
    if (file) {
        file.print(logEntry);
        file.close();
    }
}