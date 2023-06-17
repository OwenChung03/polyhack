#ifndef AMM_SYSTEM_H
#define AMM_SYSTEM_H

#include <iostream>
#include <cstring>
#include <iomanip>
#include <cmath>
using namespace std;

const int MAX_NUM_PLAYERS = 10;
const int MAX_ID = 40;
const float FEE = 0.003;
const char pool_address[MAX_ID] = "bp66MyuPVVuuy6128xL77";

struct Player
{
    char address[MAX_ID];
    double usd_balance;
    double jpy_balance;
    double usd_staking;
    double jpy_staking;
    bool liquidity_provider;
};
struct Block
{
    Player *sender;
    Player *receiver;
    double usd_amount;
    double jpy_amount;
    Block *next;
};

bool connect_wallet(Player *&playerArray, const int numOfPlayers, char address[MAX_ID], double usd_balance, double jpy_balance);
bool create_block(Block *&genesisBlock, Player *sender, Player *receiver,double usd_amount, double jpy_amount, Block *next);
double quote_USD_price(double usd_reserve, double jpy_reserve, double dy);
double quote_JPY_price(double usd_reserve, double jpy_reserve, double du);
double reward_fee_in_usd(const Player player, int numOfPlayers, Player *playerArray, double fee_in_usd);
double reward_fee_in_jpy(const Player player, int numOfPlayers, Player *playerArray, double fee_in_jpy);

#endif