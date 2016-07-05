#ifndef COMMAND_H
#define COMMAND_H

#include <string>

enum class CommandSource { Server, Client };

/**
 * uadet client and server send commands between each other over the TCP 
 * connection. This class characterizes the abstraction of these commands
 * which are send as ascii text.
 * 
 * Example: a client sends the following message
 * 
 *     eval Mozilla/5.0 (Linux; Android EN) Firefox/44.0
 * 
 * When this message becomes a Command instance it will have these fields:
 * 
 *     source=CommandSource.Client
 *     name=eval
 *     data=Mozilla/5.0 (Linux; Android EN) Firefox/44.0
 *
 */
class Command
{
public:
    class Builder;
    
    Command(CommandSource const commandSource, std::string const& name, std::string const& data) : commandSource(commandSource), name(name), data(data) {};
    virtual ~Command();
    
    CommandSource getCommandSource();
    std::string getName();
    std::string getData();
    
    std::string toString();
private:
    CommandSource const commandSource;
    std::string const name;
    std::string const data;
};

class Command::Builder
{
public:
    Builder();
    
    Builder& withCommandSource(CommandSource const& commandSource);
    Builder& withName(std::string const& name);
    Builder& withData(std::string const& data);
    Command build();
    
private:
    CommandSource commandSource;
    std::string name;
    std::string data;
};

#endif /* COMMAND_H */