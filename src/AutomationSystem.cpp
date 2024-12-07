#include "AutomationSystem.h"
#include <SPIFFS.h>

AutomationSystem::AutomationSystem() : defaultCooldown(1000) {}

bool AutomationSystem::begin() {
    return SPIFFS.begin(true);
}

void AutomationSystem::update(const SensorData& data) {
    unsigned long now = millis();
    
    for (auto& rule : rules) {
        if (!rule.second.enabled) continue;
        
        // Vérifier le cooldown
        if (now - rule.second.lastTriggered < rule.second.cooldown) {
            continue;
        }
        
        // Évaluer les conditions
        if (evaluateConditions(rule.second, data)) {
            executeActions(rule.second, data);
            rule.second.lastTriggered = now;
            updateTriggerCount(rule.first);
            
            // Désactiver la règle si oneShot
            if (rule.second.oneShot) {
                rule.second.enabled = false;
            }
        }
    }
}

void AutomationSystem::addRule(const Rule& rule) {
    if (rule.cooldown == 0) {
        Rule newRule = rule;
        newRule.cooldown = defaultCooldown;
        rules[rule.name] = newRule;
    } else {
        rules[rule.name] = rule;
    }
}

void AutomationSystem::removeRule(const String& name) {
    rules.erase(name);
}

void AutomationSystem::enableRule(const String& name, bool enable) {
    auto it = rules.find(name);
    if (it != rules.end()) {
        it->second.enabled = enable;
    }
}

void AutomationSystem::clearRules() {
    rules.clear();
    triggerCounts.clear();
}

std::function<bool(const SensorData&)> AutomationSystem::objectDetected(const String& objectName) {
    return [objectName](const SensorData& data) {
        for (const auto& label : data.labels) {
            if (label == objectName) return true;
        }
        return false;
    };
}

std::function<bool(const SensorData&)> AutomationSystem::gestureDetected(const String& gestureName) {
    return [gestureName](const SensorData& data) {
        for (const auto& label : data.labels) {
            if (label == gestureName) return true;
        }
        return false;
    };
}

std::function<bool(const SensorData&)> AutomationSystem::positionInZone(const Point& center, float radius) {
    return [center, radius](const SensorData& data) {
        for (const auto& point : data.points) {
            float dx = point.x - center.x;
            float dy = point.y - center.y;
            float distance = sqrt(dx*dx + dy*dy);
            if (distance <= radius) return true;
        }
        return false;
    };
}

std::function<bool(const SensorData&)> AutomationSystem::objectCountAbove(int count) {
    return [count](const SensorData& data) {
        return data.objectCount > count;
    };
}

std::function<bool(const SensorData&)> AutomationSystem::confidenceAbove(float threshold) {
    return [threshold](const SensorData& data) {
        return data.confidence > threshold;
    };
}

std::function<void(const SensorData&)> AutomationSystem::changeMode(HuskyMode mode) {
    return [mode](const SensorData&) {
        // TODO: Implémenter le changement de mode
    };
}

std::function<void(const SensorData&)> AutomationSystem::captureImage(const String& filename) {
    return [filename](const SensorData&) {
        // TODO: Implémenter la capture d'image
    };
}

std::function<void(const SensorData&)> AutomationSystem::sendNotification(const String& message) {
    return [message](const SensorData&) {
        // TODO: Implémenter l'envoi de notification
    };
}

bool AutomationSystem::saveRules(const String& filename) {
    File file = SPIFFS.open(filename, "w");
    if (!file) return false;
    
    DynamicJsonDocument doc(16384);
    JsonArray rulesArray = doc.createNestedArray("rules");
    
    for (const auto& rule : rules) {
        rulesArray.add(serializeRule(rule.second));
    }
    
    serializeJson(doc, file);
    file.close();
    return true;
}

bool AutomationSystem::loadRules(const String& filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) return false;
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, file);
    if (error) return false;
    
    rules.clear();
    JsonArray rulesArray = doc["rules"];
    
    for (JsonObject ruleObj : rulesArray) {
        Rule rule = deserializeRule(ruleObj);
        rules[rule.name] = rule;
    }
    
    file.close();
    return true;
}

void AutomationSystem::setDefaultCooldown(unsigned long ms) {
    defaultCooldown = ms;
}

std::vector<String> AutomationSystem::getRuleNames() const {
    std::vector<String> names;
    for (const auto& rule : rules) {
        names.push_back(rule.first);
    }
    return names;
}

const Rule* AutomationSystem::getRule(const String& name) const {
    auto it = rules.find(name);
    if (it != rules.end()) {
        return &it->second;
    }
    return nullptr;
}

std::map<String, int> AutomationSystem::getTriggerCounts() const {
    return triggerCounts;
}

bool AutomationSystem::evaluateConditions(const Rule& rule, const SensorData& data) {
    for (const auto& condition : rule.conditions) {
        if (condition.evaluator && !condition.evaluator(data)) {
            return false;
        }
    }
    return true;
}

void AutomationSystem::executeActions(const Rule& rule, const SensorData& data) {
    for (const auto& action : rule.actions) {
        if (action.executor) {
            action.executor(data);
        }
    }
}

void AutomationSystem::updateTriggerCount(const String& ruleName) {
    triggerCounts[ruleName]++;
}

DynamicJsonDocument AutomationSystem::serializeRule(const Rule& rule) {
    DynamicJsonDocument doc(4096);
    doc["name"] = rule.name;
    doc["description"] = rule.description;
    doc["enabled"] = rule.enabled;
    doc["oneShot"] = rule.oneShot;
    doc["cooldown"] = rule.cooldown;
    
    JsonArray conditionsArray = doc.createNestedArray("conditions");
    for (const auto& condition : rule.conditions) {
        conditionsArray.add(serializeCondition(condition));
    }
    
    JsonArray actionsArray = doc.createNestedArray("actions");
    for (const auto& action : rule.actions) {
        actionsArray.add(serializeAction(action));
    }
    
    return doc;
}

Rule AutomationSystem::deserializeRule(const JsonObject& obj) {
    Rule rule;
    rule.name = obj["name"].as<String>();
    rule.description = obj["description"].as<String>();
    rule.enabled = obj["enabled"] | true;
    rule.oneShot = obj["oneShot"] | false;
    rule.cooldown = obj["cooldown"] | defaultCooldown;
    
    JsonArray conditionsArray = obj["conditions"];
    for (JsonVariant v : conditionsArray) {
        rule.conditions.push_back(deserializeCondition(v.as<String>()));
    }
    
    JsonArray actionsArray = obj["actions"];
    for (JsonVariant v : actionsArray) {
        rule.actions.push_back(deserializeAction(v.as<String>()));
    }
    
    return rule;
}

String AutomationSystem::serializeCondition(const Condition& condition) {
    String result = String(static_cast<int>(condition.type)) + ":";
    for (const auto& param : condition.parameters) {
        result += param + ",";
    }
    return result;
}

String AutomationSystem::serializeAction(const Action& action) {
    String result = String(static_cast<int>(action.type)) + ":";
    for (const auto& param : action.parameters) {
        result += param + ",";
    }
    return result;
}

Condition AutomationSystem::deserializeCondition(const String& str) {
    int separatorPos = str.indexOf(':');
    if (separatorPos < 0) return Condition(TriggerType::OBJECT_DETECTED);
    
    TriggerType type = static_cast<TriggerType>(str.substring(0, separatorPos).toInt());
    std::vector<String> params;
    
    String paramsStr = str.substring(separatorPos + 1);
    int startPos = 0;
    while (true) {
        int commaPos = paramsStr.indexOf(',', startPos);
        if (commaPos < 0) break;
        params.push_back(paramsStr.substring(startPos, commaPos));
        startPos = commaPos + 1;
    }
    
    return Condition(type, params);
}

Action AutomationSystem::deserializeAction(const String& str) {
    int separatorPos = str.indexOf(':');
    if (separatorPos < 0) return Action(ActionType::CHANGE_MODE);
    
    ActionType type = static_cast<ActionType>(str.substring(0, separatorPos).toInt());
    std::vector<String> params;
    
    String paramsStr = str.substring(separatorPos + 1);
    int startPos = 0;
    while (true) {
        int commaPos = paramsStr.indexOf(',', startPos);
        if (commaPos < 0) break;
        params.push_back(paramsStr.substring(startPos, commaPos));
        startPos = commaPos + 1;
    }
    
    return Action(type, params);
}