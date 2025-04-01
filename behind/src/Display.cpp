#include "../include/Display.h"
#include <sstream>
#include <iomanip>

Display::Display() {
    // Initialize message containers
}

Display::~Display() {
    // Cleanup if needed
}

void Display::addMessage(const std::string& message, MessageType type) {
    // Add to general messages list
    messages.push_back(message);
    
    // Add to type-specific messages map
    messagesByType[type].push_back(message);
}

void Display::clearMessages() {
    // Clear all messages
    messages.clear();
    
    // Clear all type-specific messages
    for (auto& pair : messagesByType) {
        pair.second.clear();
    }
}

void Display::clearMessages(MessageType type) {
    // Clear messages of the specified type
    messagesByType[type].clear();
    
    // Rebuild general messages list
    messages.clear();
    for (const auto& pair : messagesByType) {
        for (const auto& msg : pair.second) {
            messages.push_back(msg);
        }
    }
}

std::vector<std::string> Display::getMessages() const {
    return messages;
}

std::vector<std::string> Display::getMessages(MessageType type) const {
    // Check if type exists in the map
    auto it = messagesByType.find(type);
    if (it != messagesByType.end()) {
        return it->second;
    }
    
    // Return empty vector if type not found
    return {};
}

std::string Display::displayAll() const {
    std::stringstream ss;
    
    if (messages.empty()) {
        ss << "No messages to display." << std::endl;
    } else {
        ss << "All Messages:" << std::endl;
        for (const auto& msg : messages) {
            ss << "  " << msg << std::endl;
        }
    }
    
    return ss.str();
}

std::string Display::displayByType(MessageType type) const {
    std::stringstream ss;
    
    // Get type-specific messages
    auto it = messagesByType.find(type);
    if (it == messagesByType.end() || it->second.empty()) {
        ss << "No " << getTypeString(type) << " messages to display." << std::endl;
    } else {
        ss << getTypeString(type) << " Messages:" << std::endl;
        for (const auto& msg : it->second) {
            ss << "  " << msg << std::endl;
        }
    }
    
    return ss.str();
}

std::string Display::getTypeString(MessageType type) {
    switch (type) {
        case MessageType::INFO:
            return "Info";
        case MessageType::WARNING:
            return "Warning";
        case MessageType::ERROR_MSG:
            return "Error";
        case MessageType::CRITICAL:
            return "Critical";
        default:
            return "Unknown";
    }
}

std::string Display::getStatusString() const {
    std::stringstream ss;
    ss << "Display Status:" << std::endl;
    ss << "  Total Messages: " << messages.size() << std::endl;
    
    // Count messages by type
    for (const auto& pair : messagesByType) {
        ss << "  " << getTypeString(pair.first) << " Messages: " << pair.second.size() << std::endl;
    }
    
    return ss.str();
}
