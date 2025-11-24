typedef void (Command::*FtCommand)(std::string, int);

// Dans handleClientData :
void handleClientData(int client_fd) {
    // ... réception des données ...
    
    // Appel via la map :
    FtCommand func = CommandMap[command];
    (commandObject.*func)(params, client_fd);
    
    // Dans ft_pass_chan :
    void Command::ft_pass_chan(std::string params, int client_fd) {
        // Problème : comment accéder au client ?
        // Il faut aller chercher dans le serveur :
        Client* client = server->findClientByFd(client_fd);
        if (!client) return;
        
        // Maintenant on peut travailler...
        if (params != server->getPassword()) {
            // Envoyer erreur au client... mais comment ?
            server->sendToClient(client_fd, "ERR_PASSWDMISMATCH");
        }
    }
}

typedef void (Command::*FtCommand)(Client* client, std::string params, Server* server);

// Dans handleClientData :
void handleClientData(int client_fd) {
    // ... réception des données ...
    Client* client = findClientByFd(client_fd);
    
    // Appel direct et propre :
    FtCommand func = CommandMap[command];
    (commandObject.*func)(client, params, this); // 'this' = serveur
}

// Dans ft_pass_chan :
void Command::ft_pass_chan(Client* client, std::string params, Server* server) {
    // Accès DIRECT au client et au serveur
    if (params ! server->getPassword()) {
        // Envoi direct via le client ou serveur
        client->sendMessage("ERR_PASSWDMISMATCH");
        server->disconnectClient(client);
    } else {
        client->setAuthenticated(true);
        client->setState(WAITING_NICK);
    }
}