#include "amm_system.h"

using namespace std;

bool connect_wallet(Player *&playerArray, const int numOfPlayers, char address[MAX_ID], double usd_balance, double jpy_balance)
{
        for (int i = 0; i < numOfPlayers; ++i) {
            if (strcmp(playerArray[i].address,address)==0) {
                return false;
            }
        }
        for (int i = 0; i < numOfPlayers; ++i) {
            if (strcmp(playerArray[i].address,"")==0) {
                strcpy(playerArray[i].address,address);
                playerArray[i].usd_balance = usd_balance;
                playerArray[i].jpy_balance = jpy_balance;
                return true;
            }
        }
        return false;
}

bool create_block(Block *&genesisBlock, Player *sender, Player *receiver,double usd_amount, double jpy_amount, Block *next) {
    // create a new block and initialize its members
    Block *newBlock = new Block;
    newBlock->sender = sender;
    newBlock->receiver = receiver;
    newBlock->usd_amount = usd_amount;
    newBlock->jpy_amount = jpy_amount;
    newBlock->next = next;
    if (genesisBlock==nullptr) {
        genesisBlock = newBlock;
        return true;
    }
    for (Block *b = genesisBlock; b != nullptr; b=b->next) {
        if (b->next==nullptr) {
            b->next = newBlock;
            return true;
        }
    }
    return false;
}

double quote_USD_price(double usd_reserve, double jpy_reserve, double dy) {
    // compute constant k
    double k = usd_reserve*jpy_reserve;;
    double du = usd_reserve - k/(jpy_reserve+dy);
    return du/dy;
}

double quote_JPY_price(double usd_reserve, double jpy_reserve, double du) {
    // compute constant k
    double k = usd_reserve*jpy_reserve;;
    double dy = jpy_reserve - k/(usd_reserve+du);
    return dy/du;
}

double reward_fee_in_usd(const Player player, int numOfPlayers, Player *playerArray, double fee_in_usd) {
    double sum = 0;
    for (int i = 0; i < numOfPlayers; ++i) {
        if (playerArray[i].liquidity_provider) {
            sum += playerArray[i].usd_staking;
        }
    }
    if (sum==0) {
        return 0;
    }
    double reward = player.usd_staking*fee_in_usd/sum;
    return reward;
}

double reward_fee_in_jpy(const Player player, int numOfPlayers, Player *playerArray, double fee_in_jpy) {
    double sum = 0;
    for (int i = 0; i < numOfPlayers; ++i) {
        if (playerArray[i].liquidity_provider) {
            sum += playerArray[i].jpy_staking;
        }
    }
    if (sum==0) {
        return 0;
    }
    double reward = player.jpy_staking*fee_in_jpy/sum;
    return reward;
}

int main()
{
    enum MenuOption
    {
        OPTION_DISPLAY_POOL = 0,
        OPTION_CONNECT_WALLET,
        OPTION_CHECK_BALANCES,
        OPTION_ADD_LIQUIDITY,
        OPTION_REMOVE_LIQUIDITY,
        OPTION_BUY_USD,
        OPTION_BUY_JPY,
        OPTION_RECORD,
        OPTION_EXIT,
        MAX_MENU_OPTIONS
    };
    const int MAX_MENU_OPTIONS_LENGTH = 80;
    char menuOptions[MAX_MENU_OPTIONS][MAX_MENU_OPTIONS_LENGTH] = {
        "Display pool size and market price",
        "Connect wallet and set balances",
        "Check balances",
        "Add liquidity",
        "Remove liquidity",
        "Buy USD with JPY",
        "Buy JPY with USD",
        "Track record",
        "Exit"
        };
    int i, option;
    Player *playerArray  = nullptr;
    Block  *genesisBlock = nullptr;
    unsigned int numOfPlayers = 0;
    char address[MAX_ID] = "";
    double usd_balance = 0; // 0 U.S. dollar
    double jpy_balance = 0; // 0 Yen    
    double usd_reserve = 1000000;
    double jpy_reserve = 140000000;
    bool ret = false;
    double n=1;
    int colWidth=15;
    char msg[1000];
    double w = 0; double du = 0; double dy = 0; double k = 0;
    double fee_in_usd = 0; double fee_in_jpy = 0;
    double usd_to_jpy = 0; double jpy_to_usd = 0;
    if ( usd_reserve != 0 && jpy_reserve != 0 ) {
       usd_to_jpy = jpy_reserve / usd_reserve; 
       jpy_to_usd = usd_reserve / jpy_reserve;
    }
    Player pool = {};
    strcpy(pool.address, pool_address);
    pool.usd_balance = usd_reserve;
    pool.jpy_balance = jpy_reserve;
    pool.usd_staking = 0;
    pool.jpy_staking = 0;
    pool.liquidity_provider = false;
    cout << "=== Initialize the players ===" << endl;
    while (true)
    {
        cout << "Enter the number of players (1.." << MAX_NUM_PLAYERS << "): ";
        cin >> numOfPlayers;
        if (numOfPlayers == 0 || numOfPlayers > MAX_NUM_PLAYERS )
        {
            cout << "Invalid number of players" << endl;
            continue;
        }
        playerArray = new Player[numOfPlayers];
        for (int i = 0; i < numOfPlayers; ++i) {
            strcpy(playerArray[i].address,address);
            playerArray[i].usd_balance = usd_balance;
            playerArray[i].jpy_balance = jpy_balance;
            playerArray[i].usd_staking = 0;
            playerArray[i].jpy_staking = 0;
            playerArray[i].liquidity_provider = false;
        }
        break;
    }
    cout << "=== AMM system (DEMO) ===" << endl;
    while (true)
    {
        for (i = 0; i < MAX_MENU_OPTIONS; i++)
            cout << i + 1 << ": " << menuOptions[i] << endl; // shift by +1 when display
        cout << "Enter 1-9 to choose your action." << endl;
        cin >> option;
        option = option - 1;

        // The invalid menu option handling
        if (option < 0 || option >= MAX_MENU_OPTIONS)
        {
            cout << "Invalid option" << endl;
            continue;
        }

        if (option == OPTION_EXIT) {
            break;
        }

        switch (option)
        {
            case OPTION_DISPLAY_POOL:
                cout << "USD reserve: " << fixed << setprecision(5) << usd_reserve << " U.S. dollar." << endl;
                cout << "JPY reserve: " << fixed << setprecision(5) << jpy_reserve << " Yen." << endl;
                if (usd_to_jpy == 0) {
                    cout << "Exchange rate is currently not avaliable !" << endl;
                }
                else{
                    cout << "1 U.S. dollar is equivalent to " << fixed << setprecision(5) << usd_to_jpy << " Yen." << endl;
                    cout << "1 Yen         is equivalent to " << fixed << setprecision(5) << jpy_to_usd << " U.S. dollar." << endl; 
                }
                break;
            case OPTION_CONNECT_WALLET:
                cout << "Paste your address: ";
                cin >> address;
                cout << "Enter your USD balance: ";
                cin >> usd_balance;
                cout << "Enter your JPY balance: ";
                cin >> jpy_balance;
                ret = connect_wallet(playerArray, numOfPlayers, address, usd_balance, jpy_balance);
                if (ret == false) {
                    cout << "Failed to connect " << address << " !" << endl;
                }
                else {
                    cout << "Connection is successful !" << endl;
                }
                break;
            case OPTION_CHECK_BALANCES:
                //table header 
                cout << setfill('*') << setw(7.5*colWidth) << "*" << endl;
                cout << setfill(' ') << fixed;
                cout << setw(1.5*colWidth) << "Address" << setw(1.5*colWidth) << "U.S dollar" << setw(1.5*colWidth) << "Yen" << setw(1.5*colWidth) << "Staking USD" << setw(1.5*colWidth) << "Staking Yen" << endl; 
                cout << setfill('*') << setw(7.5*colWidth) << "*" << endl; 
                cout << setfill(' ') << fixed; 
                // create table of data
                cout << setw(1.5*colWidth) << pool.address << setw(1.5*colWidth) << pool.usd_balance << setw(1.5*colWidth) << pool.jpy_balance << endl;
                for (int i = 0; i < numOfPlayers; ++i) {
                    if (strcmp(playerArray[i].address,"")==0) {
                        break;
                    }
                    cout << setw(1.5*colWidth) << playerArray[i].address << setw(1.5*colWidth) << playerArray[i].usd_balance << setw(1.5*colWidth) << playerArray[i].jpy_balance << setw(1.5*colWidth) << playerArray[i].usd_staking << setw(1.5*colWidth) << playerArray[i].jpy_staking << endl;
                }
                break;
            case OPTION_ADD_LIQUIDITY:
                cout << "Enter your address: ";
                cin >> address;
                cout << "To become LP, you are required to stake equal amount of USD and JPY to the pool." << endl;
                cout << "Enter your desired total staking amount (in U.S. dollar), we will automatically deduct the corresponding amount from your USD balance and JPY balance" << endl;
                cin >> usd_balance;
                cout << "Based on the current market price, we will deduct " << fixed << setprecision(5) << usd_balance/2 << " U.S. dollar and " << (usd_balance/2)*usd_to_jpy << " Yen from your wallet." << endl;
                cout << "Type CONFIRM to proceed->";
                cin >> msg;
                if (strcmp(msg,"CONFIRM")!=0) {
                    cout << "Transaction failed. Pls retry." << endl;
                    break;
                }
                for (int i = 0; i < numOfPlayers; ++i) {
                    if (strcmp(playerArray[i].address,address)==0) {
                        if ((playerArray[i].usd_balance >= usd_balance/2) && 
                        (playerArray[i].jpy_balance >= (usd_balance/2)*usd_to_jpy)) {
                            // create block
                            if(create_block(genesisBlock, &playerArray[i], &pool, usd_balance/2, (usd_balance/2)*usd_to_jpy, nullptr)) {
                                // Update player and pool information
                                playerArray[i].usd_balance = playerArray[i].usd_balance - usd_balance/2;
                                playerArray[i].jpy_balance = playerArray[i].jpy_balance - (usd_balance/2)*usd_to_jpy;
                                playerArray[i].usd_staking += usd_balance/2;
                                playerArray[i].jpy_staking += (usd_balance/2)*usd_to_jpy;
                                playerArray[i].liquidity_provider = true;
                                usd_reserve = usd_reserve + usd_balance/2;
                                jpy_reserve = jpy_reserve + (usd_balance/2)*usd_to_jpy;
                                pool.usd_balance = usd_reserve;
                                pool.jpy_balance = jpy_reserve;
                                cout << "Transaction successed" << endl;
                                break;
                            }
                        }
                        cout << "Insufficient balance." << endl;
                        break;
                    }
                    if (i == numOfPlayers -1) {
                        cout << address << " does not exists." << endl;
                        break;
                    }
                }
                break;
            case OPTION_REMOVE_LIQUIDITY:
                cout << "Enter your address: ";
                cin >> address;
                for (int i = 0; i < numOfPlayers; ++i) {
                    if (strcmp(playerArray[i].address,address)==0) {
                        if (playerArray[i].usd_staking>0 && playerArray[i].jpy_staking>0) {
                            cout << "You have staked " << playerArray[i].usd_staking << " U.S.dollar and " << playerArray[i].jpy_staking << " Yen into the pool." << endl;
                            cout << "Please let us know the percentage you would like to unstake. Enter 0 to 100." << endl;
                            cin >> w;
                            if (w<0 || w>100) {
                                cout << "invalid input" << endl;
                                break;
                            }
                            else {
                                w = w/100.0;
                                if (playerArray[i].jpy_staking < playerArray[i].usd_staking*w*usd_to_jpy) {
                                    if(create_block(genesisBlock, &pool, &playerArray[i], playerArray[i].jpy_staking*w*jpy_to_usd, playerArray[i].jpy_staking*w, nullptr)) {
                                        double get_in_usd = reward_fee_in_usd(playerArray[i], numOfPlayers, playerArray, fee_in_usd);
                                        double get_in_jpy = reward_fee_in_jpy(playerArray[i], numOfPlayers, playerArray, fee_in_jpy);
                                        cout << "You earned " << get_in_usd*w << " U.S.dollar and " << get_in_jpy*w << " Yen in reward for staking." << endl;
                                        // Update player and pool information
                                        usd_reserve = usd_reserve - playerArray[i].jpy_staking*w*jpy_to_usd;
                                        jpy_reserve = jpy_reserve - playerArray[i].jpy_staking*w;
                                        pool.usd_balance = usd_reserve;
                                        pool.jpy_balance = jpy_reserve;
                                        playerArray[i].jpy_balance = playerArray[i].jpy_balance + playerArray[i].jpy_staking*w + get_in_jpy*w;
                                        playerArray[i].usd_balance = playerArray[i].usd_balance + playerArray[i].jpy_staking*w*jpy_to_usd + get_in_usd*w;
                                        playerArray[i].usd_staking -= playerArray[i].jpy_staking*w*jpy_to_usd;
                                        playerArray[i].jpy_staking -= playerArray[i].jpy_staking*w;
                                        if (playerArray[i].usd_staking==0 || playerArray[i].jpy_staking==0){
                                            playerArray[i].liquidity_provider = false;
                                        }
                                        cout << "Transaction successed" << endl;
                                        break;
                                    }
                                }
                                else {
                                    // create block
                                    if(create_block(genesisBlock, &pool, &playerArray[i], playerArray[i].usd_staking*w, playerArray[i].usd_staking*w*usd_to_jpy, nullptr)) {
                                        double get_in_usd = reward_fee_in_usd(playerArray[i], numOfPlayers, playerArray, fee_in_usd);
                                        double get_in_jpy = reward_fee_in_jpy(playerArray[i], numOfPlayers, playerArray, fee_in_jpy);
                                        cout << "You earned " << get_in_usd*w << " U.S.dollar and " << get_in_jpy*w << " Yen in reward for staking." << endl;
                                        // Update player and pool information
                                        usd_reserve = usd_reserve - playerArray[i].usd_staking*w;
                                        jpy_reserve = jpy_reserve - playerArray[i].usd_staking*w*usd_to_jpy;
                                        pool.usd_balance = usd_reserve;
                                        pool.jpy_balance = jpy_reserve;
                                        playerArray[i].jpy_balance = playerArray[i].jpy_balance + playerArray[i].usd_staking*w*usd_to_jpy + get_in_jpy*w;
                                        playerArray[i].usd_balance = playerArray[i].usd_balance + playerArray[i].usd_staking*w + get_in_usd*w;
                                        playerArray[i].jpy_staking -= playerArray[i].usd_staking*w*usd_to_jpy;
                                        playerArray[i].usd_staking -= playerArray[i].usd_staking*w;
                                        if (playerArray[i].usd_staking==0 || playerArray[i].jpy_staking==0){
                                            playerArray[i].liquidity_provider = false;
                                        }
                                        cout << "Transaction successed" << endl;
                                        break;
                                    }
                                }
                            }

                        }
                        else {
                            cout << address << " does not have any stake to the pool." << endl;
                            break;
                        }
                    }
                    if (i == numOfPlayers -1) {
                        cout << address << " does not exists." << endl;
                        break;
                    }
                }
                break;
            case OPTION_BUY_USD:
                cout << "Enter your address: ";
                cin >> address;
                cout << "Enter amount of Yen: ";
                cin >> dy;
                for (int i = 0; i < numOfPlayers; ++i) {
                    // address exists
                    if (strcmp(playerArray[i].address,address)==0) {
                        // enough USD balance in the wallet
                        if (playerArray[i].jpy_balance>=dy) {
                            // quote price and inform market impact
                            double quote_price = quote_USD_price(usd_reserve, jpy_reserve, dy);
                            cout << "We offer quotes at " << fixed << setprecision(5) << quote_price << " JPY/USD." << endl;
                            cout << "In other words, you will get " << fixed << setprecision(5) << quote_price*dy << " U.S.dollar for " << dy << " Yen. (NOTE: 0.3% fees are not yet included.)" << endl;
                            cout << "Price impact: " << fixed << setprecision(5) << (quote_price-jpy_to_usd)*100/jpy_to_usd << "%." << endl;
                            cout << "Type CONFIRM to proceed->";
                            cin >> msg;
                            if (strcmp(msg,"CONFIRM")!=0) {
                                cout << "Transaction failed. Pls retry." << endl;
                                break;
                            }
                            else {
                                // enough JPY reserve to handle the trade
                                // if using CPMM, the pool will always have enough reserve to handle trade
                                // price will adjust automatically for surge demand
                                if (quote_price*dy>usd_reserve) {
                                    cout << "Sorry, we don't have enough reserve to meet your trade." << endl;
                                    break;
                                }
                                else {
                                    // create block 1: player send JPY to the reserve
                                    // create block 2: reserve send USD to the player
                                    if (create_block(genesisBlock, &playerArray[i], &pool, 0, dy, nullptr) &&
                                    create_block(genesisBlock, &pool, &playerArray[i], quote_price*dy*(1-FEE), 0, nullptr)) {
                                        // Update player and pool information
                                        playerArray[i].jpy_balance = playerArray[i].jpy_balance - dy;
                                        jpy_reserve = jpy_reserve + dy;
                                        pool.jpy_balance = jpy_reserve;
                                        playerArray[i].usd_balance = playerArray[i].usd_balance + quote_price*dy*(1-FEE);
                                        usd_reserve = usd_reserve - quote_price*dy;
                                        pool.usd_balance = usd_reserve;
                                        // Update market price
                                        usd_to_jpy = jpy_reserve / usd_reserve; 
                                        jpy_to_usd = usd_reserve / jpy_reserve;
                                        // Update fee trasury
                                        fee_in_usd += quote_price*dy*FEE; 
                                        cout << "Transaction successed" << endl;
                                        break;
                                    }
                                    else {
                                        cout << "Unknown error!" << endl;
                                    }
                                }
                            }
                        }
                        else {
                            cout << address << " does not have sufficient USD." << endl;
                            break;                           
                        }
                    }
                    if (i == numOfPlayers -1) {
                        cout << address << " does not exists." << endl;
                        break;
                    }
                }
                break;
            case OPTION_BUY_JPY:
                cout << "Enter your address: ";
                cin >> address;
                cout << "Enter amount of USD: ";
                cin >> du;
                for (int i = 0; i < numOfPlayers; ++i) {
                    // address exists
                    if (strcmp(playerArray[i].address,address)==0) {
                        // enough USD balance in the wallet
                        if (playerArray[i].usd_balance>=du) {
                            // quote price and inform market impact
                            double quote_price = quote_JPY_price(usd_reserve, jpy_reserve, du);
                            cout << "We offer quotes at " << fixed << setprecision(5) << quote_price << " USD/JPY." << endl;
                            cout << "In other words, you will get " << fixed << setprecision(5) << quote_price*du << " Yen for " << du << " U.S.dollar. (NOTE: 0.3% fees are not yet included.)" << endl;
                            cout << "Price impact: " << fixed << setprecision(5) << (quote_price-usd_to_jpy)*100/usd_to_jpy << "%." << endl;
                            cout << "Type CONFIRM to proceed->";
                            cin >> msg;
                            if (strcmp(msg,"CONFIRM")!=0) {
                                cout << "Transaction failed. Pls retry." << endl;
                                break;
                            }
                            else {
                                // enough JPY reserve to handle the trade
                                // if using CPMM, the pool will always have enough reserve to handle trade
                                // price will adjust automatically for surge demand
                                if (quote_price*du>jpy_reserve) {
                                    cout << "Sorry, we don't have enough reserve to meet your trade." << endl;
                                    break;
                                }
                                else {
                                    // create block 1: player send USD to the reserve
                                    // create block 2: reserve send JPY to the player
                                    if (create_block(genesisBlock, &playerArray[i], &pool, du, 0, nullptr) &&
                                    create_block(genesisBlock, &pool, &playerArray[i], 0, quote_price*du*(1-FEE), nullptr)) {
                                        // Update player and pool information
                                        playerArray[i].usd_balance = playerArray[i].usd_balance - du;
                                        usd_reserve = usd_reserve + du;
                                        pool.usd_balance = usd_reserve;
                                        playerArray[i].jpy_balance = playerArray[i].jpy_balance + quote_price*du*(1-FEE);
                                        jpy_reserve = jpy_reserve - quote_price*du;
                                        pool.jpy_balance = jpy_reserve;
                                        // Update market price
                                        usd_to_jpy = jpy_reserve / usd_reserve; 
                                        jpy_to_usd = usd_reserve / jpy_reserve;
                                        // Update fee trasury
                                        fee_in_jpy += quote_price*du*FEE;         
                                        cout << "Transaction successed" << endl;
                                        break;
                                    }
                                    else {
                                        cout << "Unknown error!" << endl;
                                    }
                                }
                            }
                        }
                        else {
                            cout << address << " does not have sufficient USD." << endl;
                            break;                           
                        }
                    }
                    if (i == numOfPlayers -1) {
                        cout << address << " does not exists." << endl;
                        break;
                    }
                }
                break;
            case OPTION_RECORD:
                cout << setfill('*') << setw(8*colWidth) << "*" << endl;
                cout << setfill(' ') << fixed;
                cout << setw(2*colWidth) << "From" << setw(2*colWidth) << "To" << setw(2*colWidth) << "U.S dollar" << setw(2*colWidth) << "Yen" << endl; 
                cout << setfill('*') << setw(8*colWidth) << "*" << endl;
                cout << setfill(' ') << fixed;
                for (Block *b = genesisBlock; b != nullptr; b=b->next) {
                    cout << setw(2*colWidth) << b->sender->address << setw(2*colWidth) << b->receiver->address << setw(2*colWidth) << b->usd_amount << setw(2*colWidth) << b->jpy_amount << endl;
                }
        }
    }
    delete [] playerArray;
    delete genesisBlock;
    return 0;
}