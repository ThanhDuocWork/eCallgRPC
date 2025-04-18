#include "HMISrvFunc.h"


HMISrvFunc::HMISrvFunc () : mData (false)
{
}

void HMISrvFunc::initSocket ()
{
    if ((server_fd = socket (AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror ("Socket failed");
        exit (EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt (server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof (opt)))
    {
        perror ("setsockopt");
        close (server_fd);
        exit (EXIT_FAILURE);
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons (SOCKET_PORT);

    if (bind (server_fd, (struct sockaddr *)&address, sizeof (address)) < 0)
    {
        perror ("Bind failed");
        close (server_fd);
        exit (EXIT_FAILURE);
    }

    if (listen (server_fd, 3) < 0)
    {
        perror ("Listen");
        close (server_fd);
        exit (EXIT_FAILURE);
    }
}
void HMISrvFunc::setSocketCommand (const std::string &command)
{
    std::lock_guard<std::mutex> lock (mDataCommandMutex);
    // printLog_I ("Updating socket command: " + command);
    mSocketCommand = command;
    mData          = true;
    mDataCV.notify_all ();
    if (mCallback)
    {
        printLog_I ("Trigger callback with command: " + command);
        mCallback (command);
    }
}
//test function
const std::string HMISrvFunc::getSocketCommand ()
{
    std::unique_lock<std::mutex> lock (mDataCommandMutex);
    if (!mDataCV.wait_for (lock, std::chrono::seconds (5), [this] () { return mData; }))
    {
        printLog_E ("Timeout: No new socket data received!");
        return "";
    }

    mData = false;
    return mSocketCommand;
}

void HMISrvFunc::runServerSocketHMI ()
{
    while (true)
    {
        printLog_I ("Server running on port: %d", SOCKET_PORT);

        {
            // std::lock_guard<std::mutex> lock (mSocketMutex);
            mClientSocket = accept (server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
            if (mClientSocket < 0)
            {
                perror ("Accept failed");
                continue;
            }
        }

        printLog_I ("Client connected from IP: %s, Port: %d", inet_ntoa (address.sin_addr), ntohs (address.sin_port));
        while (true)
        {
            handleRevConmamndLCD ();

            {
                // std::lock_guard<std::mutex> lock (mSocketMutex);
                if (mClientSocket <= 0)
                {
                    printLog_I ("Client disconnected. Closing socket.");
                    break;
                }
            }
        }
        {
            // std::lock_guard<std::mutex> lock (mSocketMutex);
            close (mClientSocket);
            mClientSocket = -1;
        }
    }

    close (server_fd);
}

void HMISrvFunc::handleRevConmamndLCD ()
{
    char buffer[1024] = { 0 };
    int valread       = recv (mClientSocket, buffer, sizeof (buffer) - 1, 0);
    if (valread > 0)
    {
        std::string command (buffer);
        printLog_I ("Receive command from client: " + command);
        setSocketCommand (command);
    }
    else if (valread == 0)
    {
        printLog_I ("Client closed connection. Please reconnect!");
        close (mClientSocket);
        mClientSocket = -1;
    }
    else
    {
        perror ("Error receiving data from client");
    }
}

void HMISrvFunc::registerCallback (std::function<void (const std::string &)> callback)
{
    mCallback = callback;
}

HMISrvFunc *HMISrvFunc::getInstance ()
{
    static std::shared_ptr<HMISrvFunc> instance;
    if (instance == nullptr)
    {
        instance = std::make_shared<HMISrvFunc> ();
    }
    return instance.get ();
}