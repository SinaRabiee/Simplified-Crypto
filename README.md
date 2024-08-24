# Simplified Crypto

## Outline

In this project we are going to implement a simple program to simulate what is happening in **cryptocurrencies**. 
We are going to implement 2 classes one of them is called `Server` and the other one is the `Client`. Unlike famous cryptocurrencies we are going to use a centralized server to keep track of our clients and transactions, the clients on the other hand can use the server to transfer money to each other and most importantly mine transactions to receive rewards.


# Server Class

- **clients**
This member variable will map each client to its wallet. The wallet is the amount of money the client has.

 - **add_client**
This function will create a new *Client* with the specified `id`. If this `id` already exists, the server should add a random 4 digit number at the end of it automatically.<br />
**::UPDATE::** each client should be assigned with 5 coins at the begining.<br />
**note.** do not use *srand* for your random numbers.


 - **get_client**
Using this function you can get a pointer to a Client using its `id`.


 - **get_wallet**
Using this function will return the wallet value of the client with username `id`.


 - **parse_trx**
Each transaction has 3 properties: **i)** id of the sender **ii)** id of the receiver **iii)** value of money to transfer.
We will show each transaction with a string, concatenating each of these properties with a `-`. For example if *ali* sends *1.5* coins to *hamed* the transaction will be shown by a string `"ali-hamed-1.5"`.
This function will parse this string format and outputting each property separately, if the string is not standard you should throw a runtime error.

- **add_pending_trx**
Each Client  can add a pending transaction using the transaction format described in the above section.
Only accept a pending transaction by authenticating the sender's signature and if he has enough money in his wallet.
**note.** define the below variable <ins>outside</ins> the `Server` class and save the pending transactions in it.
	```cpp
	std::vector<std::string> pending_trxs;
	```
		

- **mine**
As mentioned in the TA class each transaction has a pending state until it has been mined and to mine transactions you first need to put your pending transactions together:
For example if you have 3 transactions like `"ali-hamed-1.5"`, `"mhmd-maryam-2.25"`, and `"mahi-navid-0.5"`; You will get one final string as: `"ali-hamed-1.5mhmd-maryam-2.25mahi-navid-0.5"`. We call this string the *mempool*.
You will also add a number called *nonce* at the end of this string. To mine the transactions the server will generate the mempool and asks each Client for a nonce and calculates the *sha256* of the final string. For each nonce if the generated *sha256* has 4 zeros <ins>in a row</ins> in the first 10 numbers, then the mine is successful and the client who called the correct nonce will be awarded with 6.25 coins.
after a successful mine of the pending transactions, all the transactions will be removed from pending and the effect of them will be applied on the clients.<br />
**::UPDATE::** instead of 4 zeros use 3 zeros in a row so it wont take time for your runs.<br />
**note.** after a successful mine, print the id of the miner and return the associate nonce.



# Client Class

- **Constructor**
Creates an object of Client and assigning the specified variables using the inputs. Also generate RSA keys for the client (public and private keys).


- **get_id**
Returns the Client's id.


- **get_publickey**
Returns the Client's public key.


- **get_wallet**
Returns the amount of money the client has.


- **:UPDATE:: sign**
signs the input with the private key and returns the signature.

- **transfer_money**
Creates a transaction in the server according to its inputs. To create a transaction use the specified string format described in above sections and sign the final transaction string with your private key. use both your signature and your transaction signature to create a pending transaction in the Server using *add_pending_trx* function.


 - **generate_nonce**
Returns a random number as a nonce so the server uses it for mining.
