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



CommandBuilder::CommandBuilder()
{
    
}

CommandBuilder::~CommandBuilder()
{

}

CommandBuilder* CommandBuilder::start()
{
    return this;
}

CommandBuilder* CommandBuilder::withCommandSource(CommandSource const& commandSource)
{
    this->commandSource = commandSource;
    
    return this;
}

CommandBuilder* CommandBuilder::withName(std::string const& name)
{
    this->name = name;
    
    return this;
}

CommandBuilder* CommandBuilder::withData(std::string const& data)
{
    this->data = data;
    
    return this;
}

Command CommandBuilder::build()
{
    return Command(commandSource, name, data);
}
