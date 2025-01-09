#include <iostream>
#include <string>
#include<fstream>
#include<math.h>
#include<iomanip>
#include<vector>
using namespace std;

struct sUser
{
    string Username;
    string Password;
    int Permissions;
    bool MarkForDelete = false;

};

enum enTransactionsMenueOptions {
    eDeposit = 1, eWithdraw = 2,
    eShowTotalBalance = 3, eShowMainMenue = 4
};
enum enManageUsersMenueOptions {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
    eUpdateUser = 4, FindUser = 5, eMainMenue = 6
};
enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5,
    eShowTransactionsMenue = 6, eManageUsers = 7, eLogout = 8
};
enum enMainMenuePermissions {
    eAll = -1, pListClietns = 1, pAddNewClient = 2, pDeleteClient = 4
    , pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

const string ClientFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

sUser CurrentUser;

void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUserMenue();
bool CheckAccessPermission(enMainMenuePermissions Permission);
void ShowAccessDenied();
void GoBackToMainMenue();
void Login();


struct sClient
{
    string AccoutNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector<string>SplitString(string S1, string Delim)
{
    vector<string>vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

sClient ConverLineToRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string>vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccoutNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

sUser ConverLineUserToRecord(string Line, string Sperator = "#//#")
{
    sUser User;
    vector<string>UserDate;

    UserDate = SplitString(Line, Sperator);

    User.Username = UserDate[0];
    User.Password = UserDate[1];
    User.Permissions = stoi(UserDate[2]);

    return User;
}

string ConverRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccoutNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

string ConverRecordUserToLine(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.Username + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}

bool ClientExistsByAccountNumber(string AccoutNumber, string FileName)
{
    vector<sClient>vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConverLineToRecord(Line);
            if (Client.AccoutNumber == AccoutNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);

        }
        MyFile.close();
    }

    return false;
}

bool ClientExistsByUserName(string Username, string FileName)
{
    vector<sUser>vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConverLineUserToRecord(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }
            vUsers.push_back(User);

        }
        MyFile.close();
    }

    return false;

}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccoutNumber);

    while (ClientExistsByAccountNumber(Client.AccoutNumber, ClientFileName))
    {
        cout << "\nClient with [" << Client.AccoutNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccoutNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

int ReadPermissionsToSet()
{
    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n ";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pListClietns;
    }

    cout << "\nAdd New Clinet? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pAddNewClient;
    }

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pDeleteClient;
    }
    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pUpdateClients;
    }

    cout << "\nFind Client? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pFindClient;
    }

    cout << "\nTransactions? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pTranactions;
    }

    cout << "\nManage User? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pManageUsers;
    }

    return Permissions;
}

sUser ReadNewUser()
{
    sUser User;

    cout << "Enter Username? ";
    getline(cin >> ws, User.Username);

    while (ClientExistsByUserName(User.Username, UsersFileName))
    {
        cout << "\nUser with[" << User.Username << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.Username);
    }

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;

}

vector<sClient>LoadClientsDataFromFile(string FileName)
{
    vector<sClient>vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConverLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<sUser>LoadUsersDateFromFile(string FileName)
{
    vector<sUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sUser User;
        while (getline(MyFile, Line))
        {
            User = ConverLineUserToRecord(Line);
            vUsers.push_back(User);

        }
    }

    return vUsers;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccoutNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintUserRecordLine(sUser User)
{
    cout << "| " << setw(15) << left << User.Username;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;

}

void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccoutNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pListClietns))
    {
        ShowAccessDenied();
        GoBackToMainMenue();
        return;
    }
    vector<sClient>vClients = LoadClientsDataFromFile(ClientFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Account Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;

    }
}

void ShowAllUsersScreen()
{
    vector<sUser>vUser = LoadUsersDateFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUser.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUser.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else
    {
        for (sUser User : vUser)
        {
            PrintUserRecordLine(User);
            cout << endl;
        }
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;

    }

}

void ShowTotalBalances()
{
    vector<sClient>vClients = LoadClientsDataFromFile(ClientFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Account Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        cout << "\t\t\t\t\t Total Balances = " << TotalBalances;

    }


}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number: " << Client.AccoutNumber;
    cout << "\nPin Code      : " << Client.PinCode;
    cout << "\nName          : " << Client.Name;
    cout << "\nPhone         : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintUserCard(sUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername      : " << User.Username;
    cout << "\nPassword      : " << User.Password;
    cout << "\nPermissions   : " << User.Permissions;
    cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient>vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccoutNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

bool FindUserByUsername(string Username, vector<sUser>vUsers, sUser& User)
{
    for (sUser C : vUsers)
    {
        if (C.Username == Username)
        {
            User = C;
            return true;
        }
    }

    return false;

}

bool FindUserByUserNameAndPassword(string UserName, string Password, sUser& User)
{
    vector<sUser>vUsers = LoadUsersDateFromFile(UsersFileName);
    for (sUser C : vUsers)
    {
        if (C.Username == UserName && C.Password == Password)
        {
            User = C;
            return true;
        }
    }

    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccoutNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

sUser ChangeUserRecord(string Username)
{

    sUser User;
    User.Username = Username;

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);
    User.Permissions = ReadPermissionsToSet();

    return User;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccoutNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;

}

bool MarkUserForDeleteByUsername(string Username, vector<sUser>& vUser)
{
    for (sUser& C : vUser)
    {
        if (C.Username == Username)
        {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;

}

vector<sClient>SaveClientsDataToFile(string FileName, vector<sClient>vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConverRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vClients;
}

vector<sUser>SaveUsersDataToFile(string FileName, vector<sUser>vUser)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (sUser C : vUser)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConverRecordUserToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vUser;

}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientFileName, ConverRecordToLine(Client));
}

void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConverRecordUserToLine(User));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more Clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New User:\n\n";
        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClinetByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client ? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientFileName, vClients);
            vClients = LoadClientsDataFromFile(ClientFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool DeleteUserByUsername(string Username, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {

        if (User.Username == "Admin")
        {
            cout << "\nYou Cannot Delete This User" << endl;
            return false;
        }
        PrintUserCard(User);
        cout << "\n\nAre you sure you want delete this User ? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            vUsers = LoadUsersDateFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Account Number (" << Username << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
    sClient Client;

    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient c : vClient)
            {
                if (c.AccoutNumber == AccountNumber)
                {
                    c = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
        }
        SaveClientsDataToFile(ClientFileName, vClient);

        cout << "\n\nClient Updata Successfully.";
        return true;
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateUserByUsername(string Username, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser& c : vUsers)
            {
                if (c.Username == Username)
                {
                    c = ChangeUserRecord(Username);
                    break;
                }
            }
        }
        SaveUsersDataToFile(UsersFileName, vUsers);

        cout << "\n\nUser Updata Successfully.";
        return true;
    }
    else
    {
        cout << "\nUser with Username  (" << Username << ") is Not Found!";
        return false;
    }
}

bool DepositBalanceTOClientByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClient)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n? ";
    cin >> Answer;

    if (Answer == 'Y' || Answer == 'y')
    {
        for (sClient& C : vClient)
        {
            if (C.AccoutNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientFileName, vClient);
                cout << "\n\nDone Successfully. New balance is :" << C.AccountBalance;

                return true;
            }

        }
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;

    return AccountNumber;
}

string ReadUserUserName()
{
    string Username = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> Username;

    return Username;
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue....";
    system("pause>0");
    ShowMainMenue();
}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDenied();
        GoBackToMainMenue();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector<sClient>vClients = LoadClientsDataFromFile(ClientFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClinetByAccountNumber(AccountNumber, vClients);
}

static void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";
    vector<sUser>vUsers = LoadUsersDateFromFile(UsersFileName);
    string Username = ReadUserUserName();
    DeleteUserByUsername(Username, vUsers);
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
    {
        ShowAccessDenied();
        GoBackToMainMenue();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClients = LoadClientsDataFromFile(ClientFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen()
{

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Users  Screen";
    cout << "\n-----------------------------------\n";
    vector <sUser> vUsers = LoadUsersDateFromFile(UsersFileName);
    string Username = ReadUserUserName();
    UpdateUserByUsername(Username, vUsers);
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDenied();
        GoBackToMainMenue();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    AddNewClients();
}

void ShowAddNewUserScreen()
{

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";
    AddNewUsers();

}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDenied();
        GoBackToMainMenue();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClients = LoadClientsDataFromFile(ClientFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber
        << "] is not found!";
}

void ShowFindUserScreen()
{

    cout << "\n-----------------------------------\n";
    cout << "\tFind Users Screen";
    cout << "\n-----------------------------------\n";
    vector <sUser> vUser = LoadUsersDateFromFile(UsersFileName);
    sUser User;
    string Username = ReadUserUserName();
    if (FindUserByUsername(Username, vUser, User))
        PrintUserCard(User);
    else
        cout << "\nUser with Username Number[" << Username
        << "] is not found!";
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    sClient Client;
    vector <sClient> vClients = LoadClientsDataFromFile(ClientFileName);
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist . \n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;
    DepositBalanceTOClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";
    sClient Client;
    vector <sClient> vClients = LoadClientsDataFromFile(ClientFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }
    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }
    DepositBalanceTOClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowTotalBalanesScreen()
{
    ShowTotalBalances();
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if ((CurrentUser.Permissions == enMainMenuePermissions::eAll))
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();
}

void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUserMenue();
}

short ReadManageUsersMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;

}

void PerfromManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption)
{
    switch (ManageUsersMenueOption)
    {
    case enManageUsersMenueOptions::eListUsers:
    {
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eAddNewUser:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eUpdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::FindUser:
    {
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenueOptions::eMainMenue:
    {
        ShowMainMenue();
        break;
    }
    }
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }
    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }
    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalanesScreen();
        GoBackToTransactionsMenue();
        break;
    }
    case enTransactionsMenueOptions::eShowMainMenue:
    {
        ShowMainMenue();
    }
    }
}

void ShowManageUserMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        ShowAccessDenied();
        GoBackToMainMenue();
        return;
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerfromManageUsersMenueOption((enManageUsersMenueOptions)ReadManageUsersMenueOption());

}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {
        ShowAccessDenied();
        GoBackToMainMenue();
        return;
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

void ShowAccessDenied()
{
    cout << "\n-----------------------------------\n";
    cout << "Access Denied,\nYou don't Have Permission to Do This\nPlease Conact Your Admin.";
    cout << "\n-----------------------------------\n";
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

sUser ReadUserNameAndPassword()
{
    sUser User;
    string UserName;
    string Password;

    cout << "Enter Username? ";
    cin >> User.Username;
    cout << "Enter Passord? ";
    cin >> User.Password;

    return User;
}

void LoginScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tLogin Screen";
    cout << "\n-----------------------------------\n";

}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();

        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        ShowTransactionsMenue();
        break;
    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUserMenue();
        break;

    case enMainMenueOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());


}

bool LoadUserInfo(string Username, string Passwerd)
{
    if (FindUserByUserNameAndPassword(Username, Passwerd, CurrentUser))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;

    string Username, Password;
    do
    {
        system("cls");

        LoginScreen();

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;
        cout << "'Enter Password? ";
        cin >> Password;

        LoginFaild = !LoadUserInfo(Username, Password);
    } while (LoginFaild);
    ShowMainMenue();


}

int main()
{
    Login();
    system("pause>0");
    return 0;
}
