#pragma once

#include <mbedtls/aes.h>
#include <mbedtls/sha256.h>
#include <vector>
#include <map>
#include "Config.h"

enum class SecurityLevel {
    LOW,
    MEDIUM,
    HIGH,
    CUSTOM
};

struct SecurityConfig {
    bool enableEncryption;
    bool enableAuthentication;
    bool enableSecureBootload;
    bool enableTLS;
    int keyRotationInterval;
    int sessionTimeout;
    std::vector<String> allowedIPs;
    
    SecurityConfig() :
        enableEncryption(true),
        enableAuthentication(true),
        enableSecureBootload(true),
        enableTLS(true),
        keyRotationInterval(24 * 3600), // 24 heures
        sessionTimeout(3600)            // 1 heure
    {}
};

struct SecurityToken {
    String token;
    unsigned long expiry;
    String userID;
    std::vector<String> permissions;
};

class SecurityManager {
public:
    SecurityManager();
    bool begin();
    
    // Configuration
    void setSecurityLevel(SecurityLevel level);
    void setCustomConfig(const SecurityConfig& config);
    bool updateSecurityPolicy(const String& policy);
    
    // Encryption
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data);
    void rotateKeys();
    
    // Authentication
    bool authenticate(const String& username, const String& password);
    SecurityToken generateToken(const String& userID);
    bool validateToken(const SecurityToken& token);
    void revokeToken(const String& token);
    
    // Access Control
    bool checkPermission(const SecurityToken& token, const String& resource);
    void grantPermission(const String& userID, const String& resource);
    void revokePermission(const String& userID, const String& resource);
    
    // Secure Communication
    bool initSecureChannel();
    bool verifyConnection(const String& fingerprint);
    void closeSecureChannel();
    
    // Monitoring
    void logSecurityEvent(const String& event);
    std::vector<String> getSecurityLogs();
    bool checkIntegrity();
    
    // Data Protection
    bool backupSecurityData(const String& path);
    bool restoreSecurityData(const String& path);
    void sanitizeData();

private:
    SecurityConfig m_config;
    SecurityLevel m_level;
    mbedtls_aes_context m_aesCtx;
    std::vector<uint8_t> m_masterKey;
    std::map<String, SecurityToken> m_activeTokens;
    std::map<String, std::vector<String>> m_userPermissions;
    
    // Clés et certificats
    void generateMasterKey();
    void deriveSessionKey(const std::vector<uint8_t>& salt);
    bool loadCertificates();
    
    // Gestion des sessions
    void cleanExpiredTokens();
    bool isTokenExpired(const SecurityToken& token);
    
    // Utilitaires
    std::vector<uint8_t> generateSalt();
    String hashPassword(const String& password, const std::vector<uint8_t>& salt);
    bool verifyHash(const String& password, const String& hash, const std::vector<uint8_t>& salt);
    
    // Surveillance
    void monitorSecurityEvents();
    void detectIntrusion();
    void handleSecurityBreach();
    
    // Stockage sécurisé
    bool saveSecureData(const String& key, const std::vector<uint8_t>& data);
    std::vector<uint8_t> loadSecureData(const String& key);
    
    // Validation
    bool validateConfig(const SecurityConfig& config);
    bool validateCertificate(const String& cert);
    bool validatePermissions(const std::vector<String>& permissions);
};