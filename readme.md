# AMM System Demo

This repository contains a demonstration of an Automated Market Maker (AMM) system implemented in C++. The AMM system allows users to connect their wallets, check balances, add liquidity, remove liquidity, and perform currency exchanges between U.S. dollars (USD) and Japanese yen (JPY).

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [Sources](#sources)
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
   git clone https://github.com/clchinkc/amm_system.git
   ```

2. Compile the C++ code using a C++ compiler (e.g., g++):

   ```shell
   g++ -o amm_system amm_system.cpp
   ```

## Usage

   After compiling the code and generating the executable file, run the AMM system by executing the following command:

    ./amm_system.exe

   Or if you want to use the input file to test the system, run the AMM system by executing the following command:

   On Mac:

    ./amm_system.exe < input1.txt

   On Windows:

    Get-Content .\input1.txt | .\amm_system.exe

   On Linux:

    cat input1.txt | ./amm_system.exe

   The output would be the same as the output1.txt file.

## Limitations

Limitations of the Existing AMM System:

1. Simplistic Market Model: The existing system overlooks market depth, order matching, and liquidity provision strategies. It only supports USD and JPY. A robust system incorporating all fiat currencies is needed for a realistic decentralized exchange.

2. Centralization: The system depends on a central liquidity pool, which contradicts the decentralized exchange ethos. Ideally, control should be evenly spread among multiple liquidity providers on-chain.

Potential Improvements:

1. Multi-token Support: The system should support trading between numerous tokens, including cryptocurrencies and stablecoins, to broaden its utility.

2. Advanced Price Calculation: Rather than using a rudimentary formula for price determination, implementing advanced pricing models, possibly using oracles or external price feeds, would improve accuracy and stability.

3. Improved User Interface: A user-friendly interface providing real-time market data, easy trading interfaces, and portfolio management tools could attract more users and improve their experience.

4. Decentralization: Transition to a decentralized model using smart contracts and blockchain technology to distribute control among liquidity providers, thereby eliminating the central authority dependency.

5. Sophisticated Market Models: Implementing refined market-making algorithms such as Constant Product Market Maker (CPMM) or Weighted Average Price (WAP) models could lead to efficient price discovery and handling of larger trading volumes.


## Contributing

Contributions to this project are welcome. To contribute, follow these steps:

1. Fork this repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Push the branch to your fork.
5. Submit a pull request to the main repository.

## Sources

The AMM system is inspired by the following sources:

1. [Uniswap V3 Whitepaper](https://uniswap.org/whitepaper-v3.pdf)
2. [Uniswap Whitepaper](https://uniswap.org/whitepaper.pdf)
3. [Uniswap Documentation](https://docs.uniswap.org/)
4. [Uniswap Liquidity Provision: An Online Learning Approach](https://arxiv.org/abs/2302.00610)
5. [Bancor V2.1 Protocol Health Report](https://blog.bancor.network/bancor-v2-1-protocol-health-report-january-2020-83338c904de0?gi=be4ee579726b)
6. [Bancor Whitepaper](https://whitepaper.io/document/52/bancor-whitepaper)
7. [SoK: Decentralized Exchanges (DEX) with Automated
Market Maker (AMM) Protocols](https://arxiv.org/pdf/2103.12732.pdf)

## License

This project is licensed under the [MIT License](https://choosealicense.com/licenses/mit/).

