import Eos from "eosjs";
import config from "./config";

const eos = Eos(config);

const getInfo = () => eos.getInfo().then(info => info);

const getBlock = blockNumber =>
  eos.getBlock(blockNumber).then(blockInfo => blockInfo);

//"eosio.token" , "eosteamhaske"
const getBalance = (tokenName, accountName) =>
  eos
    .getCurrencyBalance(tokenName, accountName)
    .then(tokenBalance => tokenBalance);

// "eosio.token", "eosteamhaske", "eosteamhaske", "user1", `2.0000 TOK`, memo
const transfer = (account, actor, from, to, quantity, memo = "memo") =>
  eos
    .transaction({
      actions: [
        {
          account: account,
          name: "transfer",
          authorization: [
            {
              actor: actor,
              permission: "active"
            }
          ],
          data: {
            from: from,
            to: to,
            quantity: quantity,
            memo: memo
          }
        }
      ]
    })
    .then(result => result);

//const registerUser
//const userHistory
//const applyLoan
//const payBackLoan

const api = {
  getInfo,
  getBlock,
  getBalance,
  transfer
};

export default api;
