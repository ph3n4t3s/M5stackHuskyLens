#pragma once

#include <vector>
#include <functional>
#include <map>
#include "Config.h"
#include <ArduinoJson.h>

enum class TriggerType {
    OBJECT_DETECTED,
    GESTURE_RECOGNIZED,
    POSITION_REACHED,
    TIME_ELAPSED,
    DISTANCE_THRESHOLD,
    OBJECT_COUNT,
    CONFIDENCE_LEVEL,
    CUSTOM_EVENT
};

enum class ActionType {
    CHANGE_MODE,
    CAPTURE_IMAGE,
    SEND_NOTIFICATION,
    TOGGLE_DISPLAY,
    START_RECORDING,
    STOP_RECORDING,
    RUN_SCRIPT,
    CUSTOM_ACTION
};

struct Condition {
    TriggerType type;
    std::vector<String> parameters;
    std::function<bool(const SensorData&)> evaluator;
    
    Condition(TriggerType t, const std::vector<String>& params = {})
        : type(t), parameters(params) {}
};

struct Action {
    ActionType type;
    std::vector<String> parameters;
    std::function<void(const SensorData&)> executor;
    
    Action(ActionType t, const std::vector<String>& params = {})
        : type(t), parameters(params) {}
};

struct Rule {
    String name;
    String description;
    bool enabled;
    bool oneShot;
    unsigned long cooldown;
    std::vector<Condition> conditions;
    std::vector<Action> actions;
    unsigned long lastTriggered;
    
    Rule() : enabled(true), oneShot(false), cooldown(0), lastTriggered(0) {}
};

class AutomationSystem {
public:
    AutomationSystem();
    bool begin();
    void update(const SensorData& data);
    
    // Gestion des règles
    void addRule(const Rule& rule);
    void removeRule(const String& name);
    void enableRule(const String& name, bool enable);
    void clearRules();
    
    // Prédicats communs
    static std::function<bool(const SensorData&)> objectDetected(const String& objectName);
    static std::function<bool(const SensorData&)> gestureDetected(const String& gestureName);
    static std::function<bool(const SensorData&)> positionInZone(const Point& center, float radius);
    static std::function<bool(const SensorData&)> objectCountAbove(int count);
    static std::function<bool(const SensorData&)> confidenceAbove(float threshold);
    
    // Actions communes
    static std::function<void(const SensorData&)> changeMode(HuskyMode mode);
    static std::function<void(const SensorData&)> captureImage(const String& filename);
    static std::function<void(const SensorData&)> sendNotification(const String& message);
    
    // Configuration
    bool saveRules(const String& filename);
    bool loadRules(const String& filename);
    void setDefaultCooldown(unsigned long ms);
    
    // État et statistiques
    std::vector<String> getRuleNames() const;
    const Rule* getRule(const String& name) const;
    std::map<String, int> getTriggerCounts() const;
    
private:
    std::map<String, Rule> rules;
    std::map<String, int> triggerCounts;
    unsigned long defaultCooldown;
    
    bool evaluateConditions(const Rule& rule, const SensorData& data);
    void executeActions(const Rule& rule, const SensorData& data);
    void updateTriggerCount(const String& ruleName);
    
    // Helpers pour la sérialisation
    DynamicJsonDocument serializeRule(const Rule& rule);
    Rule deserializeRule(const JsonObject& obj);
    String serializeCondition(const Condition& condition);
    String serializeAction(const Action& action);
    Condition deserializeCondition(const String& str);
    Action deserializeAction(const String& str);
};