import Eos from "eosjs";
import config from "./config";

const eos = Eos(config);
eos.getInfo({}).then(console.log);
eos.getBlock("3000").then(console.log);
eos
  .getCurrencyBalance("eosio.token", "eosteamhaske")
  .then(tokenBalance => console.log(tokenBalance));
/*
const options = {
  authorization: "eosteamhaske@active",
  user: "eosteamhaske",
  broadcast: true,
  sign: true
};
eos
  .contract("haskehelloc1")
  .then(contract => contract.hi("eosteamhaske", options))
  .then(console.log);
*/
const transfer = () => {
  eos
  .transaction({
    actions: [
      {
        account: "eosio.token",
        name: "transfer",
        authorization: [
          {
            actor: "eosteamhaske",
            permission: "active"
          }
        ],
        data: {
          from: "eosteamhaske",
          to: "user1",
          quantity: `2.0000 TOK`,
          memo: "some description of the transaction"
        }
      }
    ]
  })
  .then(result => {
    console.log('done')
  });
}
const api = "api";

export default api;
