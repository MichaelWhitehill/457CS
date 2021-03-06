//
// Created by michael on 10/4/18.
//

#include "client.h"



struct clientInfo{
    char *hostName = nullptr;
    std::string userName, configFile, testFile, logFile, Away, password, level; //config File & test File parameter will be string to existing file
    int port = 0;    //destination server port
    std::ofstream logFileWrite;   //string logFile name - file writer will be used by many function calls
}clientState;

//password, level, banned

//ALLOWED LEVELS:
//user, channelop, sysop, admin

static auto t = time(NULL); //var for current time

static std::map <std::string, ops> mapString;


int client::clientMain(int argc, char *argv[])
{
    //INITIALIZE MAPSTRING:
    initialize(mapString);

    if (argc < 2) {
        std::cerr<< "Incorrect usage: not enough arguments, Client minimum arguments: -c 'configFileName.conf' (in current directory) clt";
        exit(0);
    }else std::cout <<"[Starting Client]" << std::endl;


    parseArgsCmdLine(argc, argv); //Parsing manually entered args to populate config file path by default


   if(fileExists(clientState.configFile)&&!clientState.configFile.empty()){

       std::cout << "[Initializing Client from .config file]"<<std::endl;

       std::ifstream read = std::ifstream(clientState.configFile);

       if(!read.is_open()){
                error("ERROR: cannot open config File");
             exit(1);}


       for(std::string line; std::getline(read, line);){
        std::vector<std::string> parse = split(line);
        if(parse[0]=="-h"){
            std::vector<char> temp(parse[1].length()+1);
            strcpy(&temp[0], parse[1].c_str());
            char* pc = new char[parse[1].length()+1];
            clientState.hostName= strcpy(pc, &temp[0]); //TODO: remove redundant steps above?
        }


        else if(parse[0]=="-u"){ //username may contain spaces
            for (int i =1; i<parse.size();i++) {
                clientState.userName.append(parse[i]);
            }
        }
        else if(parse[0]=="-p"){clientState.port = stoi(parse[1]);}
        else if(parse[0]=="-t"){clientState.testFile = parse[1];}
        else if(parse[0]=="-L"){clientState.logFile = parse[1];}
        else if(parse[0]=="-P"){clientState.password = parse[1];}
        else if(parse[0]=="-a"){clientState.level = parse[1];}

            // else if(parse[0]==""){error("ERROR: you may have trailing new lines in .config file");}
        else error("ERROR: Incorrect formatting in .config file\n "
                   "Client args are in the form: -h hostname -u username (blank for no name) -p serverPort -c configFile -t testFile -L logFile -P password (@ for no pass) -a admin_level");
       }

       std::string uname = "[LOG]_";
       if(clientState.userName.empty())uname.append("Anon User: "+getTime());
       else {
           uname.append(clientState.userName);
           uname = std::regex_replace(uname, std::regex("^ +| +$|( ) +"), "$1");
       }
       clientState.logFile.append(uname);

   }
   else error("ERROR: Config file is unreadable");


   //create log file and set member var if file does not exist already: - **LogFile variable should include directory path!!

    if(!fileExists(clientState.logFile)){
       clientState.logFileWrite = std::ofstream(clientState.logFile);
       if(!clientState.logFileWrite.is_open()){error("ERROR: Unable to open log File for client"); exit(1);}
       else        std::cout<< "New Log File opened for writing\n" << std::endl;

       clientState.logFileWrite << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
               <<"Client hostName: " << clientState.hostName << std::endl
               << "Client Port: " << clientState.port << std::endl;


   }else {

       const char* remv = clientState.logFile.c_str();
       if(std::remove(remv)!=0){error("ERROR deleting existing logFile for client");}
       else {
           std::cout << "Old client log File successfully deleted - opening fresh file" << std::endl;
           clientState.logFileWrite = std::ofstream(clientState.logFile);
           std::cout<< "New Client Log File opened for writing" << std::endl;

           clientState.logFileWrite << "\n[" << getTime() << "] STARTING LOG FILE: "<< std::endl
                   <<"Client hostName: " << clientState.hostName << std::endl
                   << "Client Port: " << clientState.port <<"\n\n"<< std::endl;
       }
   }



//______________________________________________________________________________________________________
   //FILES HAVE BEEN READ AND OPENED - SETUP CONNECTION WITH SERVER:
    ssize_t errNo;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR: opening socket");
    struct hostent *server;
    server = gethostbyname(clientState.hostName);

    if (server == nullptr) { //also doubles as a check for the clientState struct var
        fprintf(stderr,"ERROR: no such host");
        exit(0);
    }


    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(static_cast<uint16_t>(clientState.port));
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    int disconnect = 0;
    std::thread listener = std::thread(client::listenAndPrint, sockfd, &disconnect);
    std::thread writer = std::thread(client::writeSock, sockfd, &disconnect);
    listener.join();
    writer.join();
    close(sockfd);

    //Closing logFile writer:
    clientState.logFileWrite.close();
    return 0;

}





void client::listenAndPrint(int sockFd, int* disconnect) {
    char buffer[2048];
    uint bufferSize = 2048;
    ssize_t errNo;
    while(true){
        memset(buffer, 0, 2048);
        errNo = read(sockFd,buffer,2048);
        if (errNo < 0)
            error("ERROR reading from socket");

        std::cout<< "RECIEVED: "<<buffer<<std::endl; //This freaks out in an infinite loop if there is another client opened and that client closes unexpectedly
        std::string recString = buffer;

        //TODO: create switch statement for RECIEVING messages:

        //std::endl automatically adds a \n and flushes the stream
        if (recString == "GOODBYE"){
            *disconnect = 1;
            clientState.logFileWrite << "[" << getTime() << "] RECV: " <<recString << "EXITING CLIENT" << std::endl;
            if(clientState.logFileWrite.bad() && fileExists(clientState.logFile)){
                error("ERROR: Client logFile has been opened but was not written to");}
            return;
        }else{
            clientState.logFileWrite << "[" << getTime() << "] RECV: " <<recString <<std::endl;
            if(clientState.logFileWrite.bad() && fileExists(clientState.logFile)){
                error("ERROR: Client logFile has been opened but was not written to");
            return;
            }
        }
    }
}






void client::writeSock(int sockFd, const int* disconnect) {

    ssize_t errNo;
    std::string input;
    ssize_t found = clientState.password.find('@');

    //Initial client setup:
    if((clientState.password == "@") && (!clientState.userName.empty())){//user has set a username but has not set a password - not allowed
        error("ERROR: registered users with a username must provide a password at login");
    }
    if(clientState.level.empty()){
        clientState.level = "user";
    }
    if((found>0) && (clientState.password.length()!=1)){
        error("ERROR: non empty passwords cannot contain the '@' symbol");
    }
        else {
        std::string info = makeMessage::INITIAL_SETTINGS(clientState.userName, clientState.password, clientState.level);
        errNo = write(sockFd, info.c_str(), info.size());
        if (errNo < 0)
            error("ERROR writing to socket in INITIAL_SETTINGS");
    }

    //convert input string to uppercase:


    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        if(*disconnect != 0)
            return;

        std::cout<<"\n(Use OP code QUIT to exit) \nPlease enter the OP code:\n\n";
        std::getline(std::cin, input);
        input = std::regex_replace(input, std::regex("^ +| +$|( ) +"), "$1"); //Trim any trailing or leading whitespace

        auto temp = input.c_str();
        std::string create;
        for(int i=0; i<input.length(); i++){

            create+=toupper(temp[i]);
        }


        auto it = mapString.find(create);
        if(create.empty() || it == mapString.end()){std::cout<<"ERROR: User input was invalid please enter a valid OP code, type HELP for usage"<<std::endl;}
        else{

            switch(it->second){

                case HELP: {
                    std::cout
                            << "Valid client operations are as follows: HELP,JSON,MSG,AWAY,INFO,INVITE,JOIN,KICK,KILL,KNOCK,NICK,NOTICE,PART,OPER,PASS,PRIVMSG,QUIT,SETNAME,TOPIC,USER,USERHOST,USERIP,USERS,WALLOPS,WHO,WHOIS,PING,TIME,RULES,VERSION, LOCK, UNLOCK,LIST, ISON, SILENCE\n See README for detailed usage instructions of each code" << std::endl;
                    break;
                }

                case JSON: { //Used for debug
                    std::cout << "Please enter the JSON string for testing Server: " << std::endl;
                    input = "";
                    std::getline(std::cin, input);
                    input += "\n";
                    std::cout << "Test Json method has been called, passing: " << input << " to server" << std::endl;
                    errNo = write(sockFd, input.c_str(), input.length());
                    if (errNo < 0)
                        error("ERROR writing to socket in JSON");
                    break;
                }

                case MSG: {
                    auto temp = makeMessage::MSG();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in MSG");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "MSG: " << temp << std::endl;
                    break;
                }

                case AWAY: {
                    auto temp = makeMessage::AWAY();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in AWAY");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "AWAY: " << temp << std::endl;

                    rapidjson::Document d;
                    d.Parse(temp.c_str());
                    if(d.IsObject()) {
                        assert(d.HasMember("message"));
                        clientState.Away = d["message"].GetString();
                    }
                    std::cout << "Client AWAY status message has been set to: " << clientState.Away << std::endl;
                    break;
                }

                case INVITE: {
                    auto temp = makeMessage::INVITE();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in INVITE");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "INVITE: " << temp << std::endl;
                    break;
                }

                case JOIN: {
                    auto temp = makeMessage::JOIN();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in JOIN");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "JOIN: " << temp << std::endl;
                    break;
                }

                case KICK: {
                    auto temp = makeMessage::KICK();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in KICK");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "KICK: " << temp << std::endl;
                    break;
                }

                case KILL:{
                    auto temp = makeMessage::KILL();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in KILL");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "KILL: " << temp << std::endl;
                    break;
                }

                case SETNAME:{
                    auto temp = makeMessage::SETNAME();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in SETNAME");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "SETNAME: " << temp << std::endl;
                    break;
                }

                case INFO:{
                    auto temp = makeMessage::PARSE("INFO");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in INFO");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "INFO: " << temp << std::endl;
                    break;
                }

                case PRIVMSG:{
                    auto temp = makeMessage::PRIVMSG();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in PRIVMSG");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "PRIVMSG: " << temp << std::endl;
                    break;
                }

                case PING:{
                    auto temp = makeMessage::PARSE("PING");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in PING");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "PING: " << temp << std::endl;
                    break;
                }

                case OPER:{
                    auto temp = makeMessage::OPER();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in OPER");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "OPER: " << temp << std::endl;
                    break;
                }

                case USERS:{
                    auto temp = makeMessage::PARSE("USERS");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in USERS");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "USERS: " << temp << std::endl;
                    break;
                }

                case TIME:{
                    auto temp = makeMessage::PARSE("TIME");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in TIME");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "TIME: " << temp << std::endl;
                    break;
                }

                case QUIT:{
                    auto temp = makeMessage::PARSE("QUIT");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in QUIT");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "QUIT: " << temp << std::endl;
                    break;
                }

                case RULES:{
                    auto temp = makeMessage::PARSE("RULES");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in RULES");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "RULES: " << temp << std::endl;
                    break;
                }

                case VERSION:{
                    auto temp = makeMessage::PARSE("VERSION");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in VERSION");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "VERSION" << temp << std::endl;
                    break;
                }

                case LOCK:{
                    auto temp = makeMessage::LOCK();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in LOCK");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "LOCK" << temp << std::endl;
                    break;
                }

                case UNLOCK:{
                    auto temp = makeMessage::UNLOCK();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in UNLOCK");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "UNLOCK" << temp << std::endl;
                    break;
                }

                case TOPIC:{
                    auto temp = makeMessage::TOPIC();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in TOPIC");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "TOPIC" << temp << std::endl;
                    break;
                }

                case LIST:{
                    auto temp = makeMessage::PARSE("LIST");
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in LIST");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "LIST" << temp << std::endl;
                    break;
                }

                case PART:{
                    auto temp = makeMessage::PART();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in PART");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "PART" << temp << std::endl;
                    break;
                }

                case ISON:{
                    auto temp = makeMessage::ISON();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in ISON");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "ISON" << temp << std::endl;
                    break;
                }

                case SILENCE:{
                    auto temp = makeMessage::SILENCE();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in SILENCE");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "SILENCE" << temp << std::endl;
                    break;
                }

                case PASS:{
                    auto temp = makeMessage::PASS();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in PASS");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "PASS" << temp << std::endl;
                    break;
                }

                case KNOCK:{
                    auto temp = makeMessage::KNOCK();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in KNOCK");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "KNOCK" << temp << std::endl;
                    break;
                }

                case NICK:{
                    auto temp = makeMessage::SETNAME();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in NICK");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "NICK" << temp << std::endl;
                    break;
                }

                case NOTICE: {
                    auto temp = makeMessage::NOTICE();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in NOTICE");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "NOTICE" << temp << std::endl;
                    break;
                }

                case WALLOPS: {
                    auto temp = makeMessage::WALLOPS();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in WALLOPS");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "WALLOPS" << temp << std::endl;
                    break;
                }

                case WHO:{
                    auto temp = makeMessage::WHO();
                    errNo = write(sockFd, temp.c_str(), temp.size());
                    if (errNo < 0)
                        error("ERROR writing to socket in WHO");
                    clientState.logFileWrite << "[" << getTime() << "] SENT: " << "WHO: " << temp << std::endl;
                    break;
                }




                default:
                    std::cout << "ERROR: Client OP code not recognized, type HELP for usage instructions" << std::endl;
                    break;

            }
            input = "";

        }

        errNo = write(sockFd, create.c_str(), create.size());
        if (errNo < 0)
            error("ERROR writing to socket");
    }
}









void client::parseArgsCmdLine(int argc, char *argv[]) {
    //processing arguments with getOpt:

    //Run switch statement below regardless of # of args provided so that configFile variable in struct is populated in either case:
    std::cout<<"\n[Client Variables set by manual args:]"<<std::endl;

    int option;
    while ((option = getopt(argc, argv, "h:u:p:c:t:L:")) != -1) {
        switch (option) {
            case 'h' :
                clientState.hostName = optarg;
                std::cout << "Client hostName: " << clientState.hostName << std::endl;
                break;
            case 'u' :
                clientState.userName = optarg;
                std::cout << "Client userName: " << clientState.userName << std::endl;
                break;
            case 'p' :
                clientState.port = atoi(optarg);
                std::cout << "Client Port: " << clientState.port << std::endl;
                break;
            case 'c' :
                clientState.configFile = optarg;
                std::cout << "Config File name: " << clientState.configFile << std::endl;
                break;
            case 't' :
                clientState.testFile = optarg;
                std::cout << "Test File name: " << clientState.testFile << std::endl;
                break;
            case 'L' : {
                clientState.logFile = optarg;
               // std::cout << "Log File has been opened under name: " << optarg << std::endl;
                break;
            }
            case 'P': {
                clientState.password = optarg;
                std::cout << "password has been specified and is : " << clientState.password << std::endl;
                break;
            }
            case 'a':{
                clientState.level = optarg;
                std::cout << "users admin level is : " << clientState.level << std::endl;
                break;
            }
            default  :
                error("ERROR: Incorrect usage: Incorrect arguments, Client args are in the form: -h hostname -u username (@ for no name) -p serverPort -c configFile -t testFile -L logFile -P password (@ for no pass) -a admin_level");
                exit(0);
        }
    }

}

bool client::fileExists(std::string &Filename) {
    std::ifstream file(Filename);
    if(!file)
        return false;
    else
        return true;
}

std::string client::getTime() {
    t = time(NULL);
    struct tm* curtime = localtime(&t);
    std::string ctime = asctime(curtime);
    ctime.erase(std::remove(ctime.begin(),ctime.end(),'\n'),ctime.end());

    return ctime;
}

void client::initialize(std::map <std::string, ops>& mapString) {

    mapString["MSG"] = MSG;
    mapString["AWAY"] = AWAY;
    mapString["JSON"] = JSON;
    mapString["HELP"] = HELP;
    mapString["INVITE"] = INVITE;
    mapString["JOIN"] = JOIN;
    mapString["KICK"] = KICK;
    mapString["KILL"] = KILL;
    mapString["KNOCK"] = KNOCK;
    mapString["NICK"] = NICK;
    mapString["NOTICE"] = NOTICE;
    mapString["PART"] = PART;
    mapString["OPER"] = OPER;
    mapString["PASS"] = PASS;
    mapString["PRIVMSG"] = PRIVMSG;
    mapString["QUIT"] = QUIT;
    mapString["SETNAME"] = SETNAME;
    mapString["TOPIC"] = TOPIC;
    mapString["USER"] = USER;
    mapString["USERHOST"] = USERHOST;
    mapString["USERIP"] = USERIP;
    mapString["USERS"] = USERS;
    mapString["WALLOPS"] = WALLOPS;
    mapString["WHO"] = WHO;
    mapString["WHOIS"] = WHOIS;
    mapString["INFO"] = INFO;
    mapString["PING"] = PING;
    mapString["TIME"] = TIME;
    mapString["RULES"] = RULES;
    mapString["VERSION"] = VERSION;
    mapString["LOCK"] = LOCK;
    mapString["UNLOCK"] = UNLOCK;
    mapString["LIST"]= LIST;
    mapString["ISON"]= ISON;
    mapString["SILENCE"]=SILENCE;

}


