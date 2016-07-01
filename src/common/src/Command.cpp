#include "Command.h"

// Command constructor in Command.h

Command::~Command()
{
    
}

CommandSource Command::getCommandSource()
{
    return commandSource;
}

std::string Command::getName()
{
    return name;
}

std::string Command::getData()
{
    return data;
}



Command::Builder::Builder()
{

}

Command::Builder& Command::Builder::withCommandSource(CommandSource const& commandSource)
{
    this->commandSource = commandSource;
    
    return *this;
}

Command::Builder& Command::Builder::withName(std::string const& name)
{
    this->name = name;
    
    return *this;
}

Command::Builder& Command::Builder::withData(std::string const& data)
{
    this->data = data;
    
    return *this;
}

Command Command::Builder::build()
{
    return Command(commandSource, name, data);
}
