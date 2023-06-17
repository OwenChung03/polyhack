# AMM System Demo

This repository contains a demonstration of an Automated Market Maker (AMM) system implemented in C++. The AMM system allows users to connect their wallets, check balances, add liquidity, remove liquidity, and perform currency exchanges between U.S. dollars (USD) and Japanese yen (JPY).

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

The AMM system is designed to facilitate decentralized currency exchanges between USD and JPY using a liquidity pool. It utilizes a block-based approach to record transactions and calculate liquidity, providing a transparent and efficient trading mechanism.

The system includes the following components:

- `Player`: Represents a user's wallet with balances and staking information.
- `Block`: Stores information about a transaction, including sender, receiver, and amounts.
- `Connect Wallet`: Allows users to connect their wallets and set initial balances.
- `Add Liquidity`: Enables users to become liquidity providers by staking equal amounts of USD and JPY to the pool.
- `Remove Liquidity`: Allows liquidity providers to unstake their funds from the pool.
- `Buy USD` and `Buy JPY`: Enables users to exchange currencies based on the current market price.
- `Track Record`: Provides a transaction history of the system.

## Features

The AMM system provides the following features:

- Displaying the pool size and market price.
- Connecting a wallet and setting balances.
- Checking balances of wallets and the pool.
- Adding liquidity to the pool.
- Removing liquidity from the pool.
- Buying USD with JPY.
- Buying JPY with USD.
- Tracking transaction records.

## Installation

To run the AMM system locally, follow these steps:

1. Clone the repository:

   ```shell
   git clone https://github.com/your_username/amm_system.git
   ```

2. Compile the C++ code using a C++ compiler (e.g., g++):

   ```shell
   g++ -o amm_system amm_system.cpp
   ```

## Usage

   After compiling the code and generating the executable file, run the AMM system by executing the following command:

    ./amm_system



## Contributing

Contributions to this project are welcome. To contribute, follow these steps:

1. Fork this repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Push the branch to your fork.
5. Submit a pull request to the main repository.

## License

This project is licensed under the MIT License.

