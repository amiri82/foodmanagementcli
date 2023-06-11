#include <iostream>
#include <string>
#include <ctime>
#include <limits>


#define ADMIN_USER 1
#define NORMAL_USER 2
#define NON_LOGIN_USER 0
#define print_divisor cout << "===========================" << endl
#define DORM 1
#define CENTRAL 2
using namespace std;

struct Food{
    string code;
    string name;
    float originalQuantity;
    float quantityLeft;
    int price;
    tm date;
};

struct Buy{
    Food* food;
    int price;
    float quantity;
};

struct Transaction{
    int quantity;
    string sourceUser;
    string destUser;
    tm date;
};

struct User{
    string username;
    string password;
    string firstName;
    string lastName;
    string phoneNumber;
    string emailAddress;
    string subject;
    string grade;
    Buy* buyList;
    Transaction* transactionList;
    int maxBuySize;
    int buySize;
    int maxTransactionSize;
    int transactionSize;
    int balance;
    int usedForgotCard;

};

void addTransaction(User* source, User* dest, int quantity);
void addBuy(Food* food, float quantity);
Transaction* changeSize(Transaction* &list, int* maxSize, int currentSize);
Buy* changeSize(Buy* &list, int* maxSize, int currentSize);
Food* showChooseFood(Food* search, int size);
void transactBalance();
void increaseBalance();
bool showUserProfile();
void showUserInfo();
bool showDiningMenu();
User* currentUser = nullptr;
void showLoginPage();
void addFood();
int login(int userType);
int usersArraySize = 100;
void showUserPanel();
void showAdminPanel();
int adminsArraySize = 10;
int usersSize = 0;
int adminsSize = 0;
void logOut();
int changePassword();
User* usersList = new User[100];
User* adminsList = new User[10];
Food* dormFoods = new Food[2];
Food* centralFoods = new Food[2];
int currentUserType = NON_LOGIN_USER;
int dormFoodSize = 0;
int centralFoodSize = 0;
int centralArraySize = 2;
int dormArraySize = 2;
void registerUser(int userType);
bool programExit = false;
User* changeSize(User* &list, int *maxSize, int currentSize);
tm changeDate();
void dateChangingMenu();
void addFood();
Food* changeSize(Food* &list, int *maxSize, int currentSize);
string generateCode(tm date);
void reserveFood();

time_t now;
tm* currentDate;

int main() {
    ::now = time(&now);
    ::currentDate = localtime(&now);
    while(!programExit){
        while(::currentUserType == NON_LOGIN_USER && !programExit){
            cout << "You have to login first!" << endl;
            showLoginPage();
            print_divisor;
            int choice;
            while(!(cin >> choice)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout << "Invalid Input! " << endl;
                print_divisor;
            }
            getchar();
            switch(choice){
                case 1:
                    ::currentUserType = login(ADMIN_USER);
                    break;
                case 2:
                    ::currentUserType = login(NORMAL_USER);
                    break;
                case 3:
                    registerUser(ADMIN_USER);
                    break;
                case 4:
                    registerUser(NORMAL_USER);
                    break;
                case 5:
                    ::programExit = true;
                    break;
                default:
                    cout << "Invalid Choice " << endl;
                    break;
            }
            print_divisor;
        }
        if(currentUserType == ADMIN_USER){
            showAdminPanel();
        }
        else if (currentUserType == NORMAL_USER){
            showUserPanel();
        }
   }
}

void registerUser(int userType){
    User* append  = userType == ADMIN_USER ? adminsList : usersList;
    int *size = userType == ADMIN_USER ? &adminsSize : &usersSize;
    int* maxSize = userType == ADMIN_USER ? &adminsArraySize : &usersArraySize;
    if(*size == *maxSize - 1)
        ::usersList = changeSize(append,maxSize, *size);
    User* currentUser = &append[*size];
    cout << "Enter username:" << endl;
    getline(cin,currentUser -> username);
    cout << "Enter password: " << endl;
    getline(cin,currentUser -> password);
    cout << "Enter Firstname: " << endl;
    getline(cin,currentUser -> firstName);
    cout << "Enter Lastname: " <<endl;
    getline(cin,currentUser -> lastName);
    cout << "Enter Phone Number:" << endl;
    getline(cin,currentUser -> phoneNumber);
    cout << "Enter Email Address:" << endl;
    getline(cin,currentUser -> emailAddress);
    if(userType == NORMAL_USER){
        cout << "Enter Subject:" << endl;
        getline(cin,currentUser -> subject);
        cout << "Enter Grade:" << endl;
        getline(cin,currentUser -> grade);
        currentUser -> buySize = 0;
        currentUser -> maxBuySize = 10;
        currentUser -> transactionSize = 0;
        currentUser -> maxTransactionSize = 2;
        currentUser -> buyList = new Buy[10];
        currentUser -> transactionList = new Transaction[2];
        currentUser -> balance = 0;
        currentUser -> usedForgotCard = 0;
    }
    *size += 1;
}

void generateForgotCode(){
    if(currentUser -> usedForgotCard >= 3){
        cout << "You have used all your chances to get your code " << endl;
        return;
    }
    if(currentUser -> buySize < 1){
        cout << "You have no reservation" << endl;
        return;
    }
    int i = 0;
    bool exit = false;
    cout << "a. go to next meal" << endl;
    cout << "d. go to previous meal" << endl;
    cout << "s. select current meal" << endl;
    cout << "e. terminate" << endl;
    Food* chosenMeal;
    char* date = new char[20];
    while(!exit){
        chosenMeal = currentUser -> buyList[i].food;
        strftime(date,20,"%F",&chosenMeal -> date);
        cout << i+1 << ". " << chosenMeal -> name << " Time: " << date << endl;
        char choice;
        cin >> choice;
        switch(choice){
            case 'a':
                if(i < currentUser -> buySize - 2)
                    i++;
                else
                    cout << "last meal!" << endl;
                print_divisor;
                break;
            case 'd':
                if(i > 0)
                    i--;
                else
                    cout << "first meal" << endl;
                print_divisor;
                break;
            case 's':
                exit = true;
                break;
            case 'e':
                exit =true;
                cout << "Terminated" << endl;
                return;
        }
    }
    time(&::now);
    ::currentDate = localtime(&::now);
    srand(difftime(mktime(&chosenMeal -> date), ::now));
    int random = rand() % 100000000;
    cout << "Your forgot card code is: " << random << endl;
    print_divisor;
    currentUser -> usedForgotCard++;
    getchar();
}

void showLoginPage(){
    cout << "Enter 1. Login as admin" << endl;
    cout << "Enter 2. Login as user" << endl;
    cout << "Enter 3. Register Admin User " << endl;
    cout << "Enter 4. Register Normal User" << endl;
    cout << "Enter 5. Exit" << endl;
}
int login(int userType){
    User* search = userType == ADMIN_USER ? adminsList : usersList;
    int size = userType == ADMIN_USER ? adminsSize : usersSize;
    string username;
    string password;
    cout << "Enter your username: " << endl;
    getline(cin,username);
    cout << "Enter your password:" << endl;
    getline(cin,password);
    for(int i = 0 ; i < size ; i++){
        if(search[i].username == username){
            if(search[i].password == password){
                currentUser = &search[i];
                cout << "Hello " << currentUser -> firstName << endl;
                return userType;
            }
            else
                break;
        }
    }
    cout << "incorrect username or password" << endl;
    return NON_LOGIN_USER;
}

void logOut(){
    ::currentUserType = NON_LOGIN_USER;
    ::currentUser = nullptr;
}

User* changeSize(User* &list, int* maxSize, int currentSize){
    User* newRes = new User[*maxSize * 2];
    for(int i = 0 ; i < currentSize ; i++)
        newRes[i] = list[i];
    delete[] list;
    list = newRes;
    *maxSize *= 2;
    return newRes;
}

void showAdminPanel() {
    bool exit = false;
    while (!exit) {
        cout << "Enter 1: Add food " << endl;
        cout << "Enter 2: Change Password" << endl;
        cout << "Enter 3: Log out" << endl;
        cout << "Enter 4: Exit" << endl;
        int choice;
        while(!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Invalid Input!" << endl;
            print_divisor;
        }
        getchar();
        switch (choice) {
            case 1:
                addFood();
                break;
            case 2:
                changePassword();
                break;
            case 3:
                logOut();
                exit = true;
                break;
            case 4:
                exit = true;
                ::programExit = true;
                break;
        }
        print_divisor;
    }
}

void showUserPanel() {
    bool exit = false;
    while (!exit) {
    cout << "Enter 1: User profile" << endl;
    cout << "Enter 2: Dining system" << endl;
    cout << "Enter 3: Log out" << endl;
    cout << "Enter 4: Exit" << endl;
    int choice;
    while(!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Invalid Input!" << endl;
        print_divisor;
    }
    getchar();
    switch (choice) {
        case 1:
            print_divisor;
            exit = showUserProfile();
            break;
        case 2:
            print_divisor;
            exit = showDiningMenu();
            break;
        case 3:
            logOut();
            exit = true;
            break;
        case 4:
            exit = true;
            ::programExit = true;
            break;
        }
    }
}

int changePassword() {
    string oldPass;
    string newPass;
    cout << "Enter your old password:" << endl;
    getline(cin, oldPass);
    cout << "Enter new password:" << endl;
    getline(cin, newPass);
    if (oldPass == currentUser -> password) {
        currentUser -> password = newPass;
        return 1;
    }
    else {
        cout << "Incorrect password" << endl;
        return -1;
    }
}


bool showUserProfile() {
    bool exit = false;
    while (!exit) {
    cout << "Enter 1: Show user information" << endl;
    cout << "Enter 2: Chagne password" << endl;
    cout << "Enter 3: Go to previous menu" << endl;
    cout << "Enter 4: Log out" << endl;
    cout << "Enter 5: Exit" << endl;
    int choice;
    while(!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Invalid Input!" << endl;
        print_divisor;
    }
    getchar();
    print_divisor;
    switch (choice) {
        case 1:
            showUserInfo();
            break;
        case 2:
            changePassword();
            break;
        case 3:
            exit = true;
            break;
        case 4:
            logOut();
            return true;
        case 5:
            ::programExit = true;
            return true;
        }
    print_divisor;
    }
    return false;
}


void showUserInfo(){
    cout << "Username: " << currentUser -> username << endl;
    cout << "First name: " << currentUser -> firstName << endl;
    cout << "Last name: " << currentUser -> lastName << endl;
    cout << "Phone number: " << currentUser -> phoneNumber << endl;
    cout << "Email address: " << currentUser -> emailAddress << endl;
    cout << "Subject: " << currentUser -> subject << endl;
    cout << "Grade: " << currentUser -> grade << endl;
    cout << "Balance : " << currentUser -> balance << endl;
}
void generatePurchaseReport(){
    Buy* buyList = currentUser -> buyList;
    char* date = new char[20];
    if(currentUser -> buySize < 1){
        cout << "No Records" << endl;
        return;
    }
    for(int i = 0 ; i < currentUser -> buySize ; i++){
        string time;
        Buy* current = &buyList[i];
        strftime(date,20,"%F",&current -> food -> date);
        cout << i + 1 << ". " << current -> food -> name << " Time: " << date  << " Quantity: " << current -> quantity << " Price : " << current -> price << endl;
    }
    delete[] date;
}

void generateFinancialReport(){
    Transaction* transactionList = currentUser -> transactionList;
    char* date = new char[20];
    for(int i = 0 ; i < currentUser -> transactionSize ; i++){
        Transaction* current = &transactionList[i];
        strftime(date,20,"%F",&current -> date);
        cout << i + 1 << ". Time: " << date;
        if(current -> quantity < 0){
            cout << " Type: Transaction to " << current -> destUser;
        }else{
            if(current -> destUser == current -> sourceUser)
                cout << " Type: Account Charge";
            else
                cout << " Transaction from " << current -> sourceUser;
        }
        cout << " Amount: " << (current -> quantity > 0 ? current -> quantity : -current -> quantity) << endl;
    }
}

bool showDiningMenu() {
    bool exit = false;
    while (!exit) {
        cout << "Enter 1: Reserve food" << endl;
        cout << "Enter 2: Increase balance" << endl;
        cout << "Enter 3: Forgot Card" << endl;
        cout << "Enter 4: Balance transaction" << endl;
        cout << "Enter 5: Purchase report" << endl;
        cout << "Enter 6: Financial report" << endl;
        cout << "Enter 7: Previous page" << endl;
        cout << "Enter 8. Log Out" << endl;
        cout << "Enter 9: Exit" << endl;
        int choice;
        while(!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Invalid Input!" << endl;
            print_divisor;
        }
        getchar();
        switch (choice) {
            case 1:
                reserveFood();
                break;
            case 2:
                increaseBalance();
                break;
            case 3:
                generateForgotCode();
                break;
            case 4:
                transactBalance();
                break;
            case 5:
                generatePurchaseReport();
                break;
            case 6:
                generateFinancialReport();
                break;
            case 7:
                exit = true;
                break;
            case 8:
                exit = true;
                logOut();
                return true;
            case 9:
                ::programExit = true;
                return true;
        }
        print_divisor;
    }
    return false;
}

void increaseBalance() {
   int toBeAdded;
   cout << "How much credit to be added?" << endl;
   while(!(cin >> toBeAdded)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Invalid Input!" << endl;
        print_divisor;
   }
   getchar();
   if(toBeAdded <= 0){
       cout << "Can not add negative or zero amounts of credit" << endl;
       return;
   }
   currentUser -> balance += toBeAdded;
   addTransaction(currentUser,currentUser,toBeAdded);
}

void addFood() {
    cout << "Enter 1: Dorm food" << endl;
    cout << "Enter 2: Central food" << endl;
    int choice;
    cin >> choice;
    getchar();
    Food* append = choice == DORM ? dormFoods : centralFoods;
    int* size = choice == DORM ? &dormFoodSize : &centralFoodSize;
    int* maxSize = choice == DORM ? &dormArraySize : &centralArraySize;
    if (*size == *maxSize -1)
        changeSize((choice == DORM ? dormFoods : centralFoods), maxSize , *size);
    Food* currentFood = &append[*size];
    cout << "Enter food name: " << endl;
    getline(cin, currentFood -> name);
    cout << "Enter quantity: " << endl;
    cin >> currentFood -> originalQuantity;
    currentFood -> quantityLeft = currentFood -> originalQuantity;
    cout << "Enter price: " << endl;
    cin >> currentFood -> price;
    tm date = changeDate();
    currentFood -> date = date;
    currentFood -> code = generateCode(date);
    *size += 1;
}

Food* changeSize(Food* &list, int* maxSize, int currentSize){
    Food* newRes = new Food[*maxSize * 2];
    for(int i = 0 ; i < currentSize ; i++)
        newRes[i] = list[i];
    delete[] list;
    list = newRes;
    *maxSize *= 2;
    return newRes;
}

Buy* changeSize(Buy* &list, int* maxSize, int currentSize){
    Buy* newRes = new Buy[*maxSize * 2];
    for(int  i = 0 ; i < currentSize ; i++)
        newRes[i] = list[i];
    delete[] list;
    list = newRes;
    *maxSize *= 2;
    return newRes;
}

Transaction* changeSize(Transaction* &list, int* maxSize, int currentSize){
    Transaction* newRes = new Transaction[*maxSize * 2];
    for(int  i = 0 ; i < currentSize ; i++)
        newRes[i] = list[i];
    delete[] list;
    list = newRes;
    *maxSize *= 2;
    return newRes;
}

void dateChangingMenu() {
    cout << "Press 'a' for next day" << endl;
    cout << "Press 'd' for previous day" << endl;
    cout << "Press 's' for select" << endl;
}

tm changeDate() {
    time_t now = time(&now);
    tm finalTime = *localtime(&now);
    dateChangingMenu();
    bool exit = false;
    while (!exit) {
        char key;
        cout << asctime(&finalTime);
        cin >> key;
        switch (key) {
            case 'd':
                finalTime. tm_mday += -1;
                mktime(&finalTime);
                print_divisor;
                break;
            case 'a':
                finalTime.tm_mday += +1;
                mktime(&finalTime);
                print_divisor;
                break;
            case 's':
                exit = true;
                mktime(&finalTime);
                break;
        }
    }
    getchar();

    return finalTime;
}

bool checkReserved(Food* food){
    for(int i = 0 ; i < currentUser -> buySize ; i++){
        if (currentUser -> buyList[i].food == food)
            return true;
    }
    return false;
}

void reserveFood() {
    cout << "Enter 1: Dorm food" << endl;
    cout << "Enter 2: Central food" << endl;
    int choice;
    cin >> choice;
    getchar();
    Food* search = choice == DORM ? dormFoods : centralFoods;
    int size = choice == DORM ? dormFoodSize : centralFoodSize;
    Food* chosenFood = showChooseFood(search, size);
    if(chosenFood == nullptr){
        cout << "Terminated" << endl;
        return;
    }
    cout << "Choose your preference:" << endl;
    cout << "1. Full Meal" << endl;
    cout << "2. Half meal" << endl;
    cin >> choice;
    getchar();
    float quantity = choice == 1 ? 1 : 0.5;
    if(chosenFood -> quantityLeft < quantity)
        cout << "Sorry there is only a half meal available. Try Again" << endl;
    else{
        if(chosenFood -> price > currentUser -> balance)
            cout << "Not Enough Money. Please charge your account and try again" << endl;
        else if(checkReserved(chosenFood))
            cout << "Already reserved." << endl;
        else{
            chosenFood -> quantityLeft -= quantity;
            currentUser -> balance -= (chosenFood -> price) * quantity;
            addBuy(chosenFood, quantity);
        }
    }
}

void addBuy(Food* food, float quantity){
    if(currentUser -> maxBuySize == currentUser -> buySize - 1)
        changeSize(currentUser -> buyList, &currentUser -> maxBuySize, currentUser -> buySize);
    Buy* changer = &currentUser -> buyList[currentUser -> buySize];
    changer -> food = food;
    changer -> quantity = quantity;
    changer -> price = (int)(quantity * (food -> price));
    currentUser -> buySize += 1;
}

Food* showChooseFood(Food* search, int size) {
    time_t finalT = now;
    tm finalTime = *(localtime(&finalT));
    dateChangingMenu();
    cout << "Press 'e' for termination" << endl;
    Food* currentFood = nullptr;
    bool exit = false;
    while (!exit) {
        string code = generateCode(finalTime);
        char key;
        cout << asctime(&finalTime);
        for(int i = 0 ; i < size ; i++){
            if(search[i].code == code)
                currentFood = &search[i];
        }
        if(currentFood != nullptr)
            cout << "Food Name: " << currentFood -> name << " Full Price: " << currentFood -> price << " Quantity Left: " << currentFood -> quantityLeft <<endl;
        else
            cout << "No food is available for this day" << endl;
        cin >> key;
        switch (key) {
            case 'd':
                finalTime.tm_mday += -1;
                mktime(&finalTime);
                currentFood = nullptr;
                break;
            case 'a':
                finalTime.tm_mday += +1;
                mktime(&finalTime);
                currentFood = nullptr;
                break;
            case 's':
                if(currentFood == nullptr)
                    cout << "there is no food available for this day. choose another day" << endl;
                else{
                    tm firstReservable = *::currentDate;
                    firstReservable.tm_mday += 2;
                    mktime(&firstReservable);
                    if(code.compare(generateCode(firstReservable)) >= 0){
                        if(currentFood -> quantityLeft == 0)
                            cout << "Sold Out!" << endl;
                        else
                            return currentFood;
                    }else{
                        cout << "You can only reserve foods 2days ahead or more! Choose another day" << endl;
                    }
                }
                break;
            case 'e':
                currentFood = nullptr;
                exit = true;
                break;
        }
        print_divisor;
    }
    getchar();
    return currentFood;
}

void transactBalance(){
    cout << "Enter destination username: " << endl;
    string destUsername;
    getline(cin,destUsername);
    User* foundUser = nullptr;
    for(int i = 0 ; i < ::usersSize ; i++){
        if(usersList[i].username == destUsername){
            foundUser = &usersList[i];
        }
    }
    if(foundUser == nullptr){
        cout << "username not found" <<endl;
        return;
    }
    cout << "How much of a transaction do you want to make?" << endl;
    int toBeExchanged;
    while(!(cin >> toBeExchanged)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Invalid Input!" << endl;
        print_divisor;
    }
    getchar();
    if(toBeExchanged > currentUser -> balance){
        cout << "Not enough balance!" << endl;
        return;
    }
    if(toBeExchanged <= 0){
        cout << "can not exchange negative or zero amount" << endl;
        return;
    }
    foundUser -> balance += toBeExchanged;
    currentUser -> balance -= toBeExchanged;
    addTransaction(currentUser, foundUser, toBeExchanged);
}

void addTransaction(User* source, User* dest, int quantity){
    Transaction* changer;
    time(&::now);
    ::currentDate = localtime(&now);
    tm time = *currentDate;
    if(source != dest){
        if(source -> maxTransactionSize - 1 == source -> transactionSize)
            changeSize(source -> transactionList, &source -> maxTransactionSize, source -> transactionSize);
        changer = &source -> transactionList[source -> transactionSize];
        changer -> quantity = -quantity;
        changer -> sourceUser = source -> username;
        changer -> destUser = dest -> username;
        changer -> date = time;
        source -> transactionSize += 1;
    }
    if(dest -> maxTransactionSize - 1 == dest -> transactionSize)
        changeSize(dest -> transactionList, &dest -> maxTransactionSize, dest -> transactionSize);
    changer = &dest -> transactionList[dest -> transactionSize];
    changer -> quantity = quantity;
    changer -> sourceUser = source -> username;
    changer -> destUser = dest -> username;
    changer -> date = time;
    dest -> transactionSize += 1;
}

string generateCode(tm date){
    int year = date.tm_year;
    int month = date.tm_mon + 1;
    int day = date.tm_mday;
    return to_string(year) + (month < 10 ? "0" : "") + to_string(month) + (day < 10 ? "0" : "") + to_string(day);
}
