#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>
#include <map>

/**
 * @enum MessageType
 * @brief Represents different types of messages that can be displayed
 */
enum class MessageType {
    INFO = 0,
    WARNING = 1,
    ERROR_MSG = 2,
    CRITICAL = 3
};

/**
 * @class Display
 * @brief Manages display messages and warnings for the vehicle
 */
class Display {
private:
    std::vector<std::string> messages;
    std::map<MessageType, std::vector<std::string>> messagesByType;
    
public:
    /**
     * @brief Default constructor
     */
    Display();
    
    /**
     * @brief Destructor
     */
    ~Display();
    
    /**
     * @brief Add a message to the display
     * @param message Message text
     * @param type Message type
     */
    void addMessage(const std::string& message, MessageType type);
    
    /**
     * @brief Clear all messages
     */
    void clearMessages();
    
    /**
     * @brief Display current gear information
     * @param gear The current gear as string
     */
    void showGear(const std::string& gear);
    
    /**
     * @brief Clear messages of a specific type
     * @param type Message type to clear
     */
    void clearMessages(MessageType type);
    
    /**
     * @brief Get all messages
     * @return Vector of all messages
     */
    std::vector<std::string> getMessages() const;
    
    /**
     * @brief Get messages of a specific type
     * @param type Message type to get
     * @return Vector of messages of the specified type
     */
    std::vector<std::string> getMessages(MessageType type) const;
    
    /**
     * @brief Display all messages
     * @return Formatted string of all messages
     */
    std::string displayAll() const;
    
    /**
     * @brief Display messages of a specific type
     * @param type Message type to display
     * @return Formatted string of messages of the specified type
     */
    std::string displayByType(MessageType type) const;
    
    /**
     * @brief Get message type as string
     * @param type Message type
     * @return Message type string
     */
    static std::string getTypeString(MessageType type);
    
    /**
     * @brief Get display status as string
     * @return Status string
     */
    std::string getStatusString() const;

    void showWarning(const std::string& message);
    void showStatus(double speed, double batteryPercent, double distance);
};

#endif // DISPLAY_H
